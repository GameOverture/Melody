#include "pch.h"
#include "Wheel.h"

#define fSAFETY_MARGIN 1.0f // The fSAFETY_MARGIN is to ensure the ticker clicks over when calculating near miss
#define fMAX_FLINGBACK_DUR 2.5f

Wheel::SpinEnt::SpinEnt(const WheelInit &initStructRef) :
	m_WedgeAngleList(initStructRef.wedgeDegreeThicknessList)
{
	m_Frame.Init(initStructRef.frame.m_sPrefix.c_str(), initStructRef.frame.m_sName.c_str(), nullptr);
	m_Notches.Init(initStructRef.notches.m_sPrefix.c_str(), initStructRef.notches.m_sName.c_str(), this);

	if(initStructRef.pWedgeWeightTable == nullptr)
	{
		HyLogWarning("Wheel took a WheelInit with nullptr weight table");
		return;
	}

	if(m_WedgeAngleList.empty() == false)
	{
		HyAssert(m_WedgeAngleList.size() == initStructRef.pWedgeWeightTable->GetNumEntries(), "LgWheel ctor took a LgWheelInit with a mismatching number of wedgeDegreeThicknessList and entires in weight table");
		float fTotal = 0.0f;
		for(uint32 i = 0; i < m_WedgeAngleList.size(); ++i)
			fTotal += m_WedgeAngleList[i];
		HyAssert(fTotal == 360.0f, "LgWheel ctor took a LgWheelInit with all wedgeDegreeThicknessList values not totaling 360.0f");
	}

	uint32 uiNumWedges = static_cast<uint32>(initStructRef.pWedgeWeightTable->GetNumEntries());
	float fRotationOffset = 0.0f;
	for(uint32 i = 0; i < uiNumWedges; ++i)
	{
		HyEntityWrapper2d<HySprite2d> *pNewWedge = HY_NEW HyEntityWrapper2d<HySprite2d>(initStructRef.wedges.m_sPrefix.c_str(), initStructRef.wedges.m_sName.c_str());
		ChildAppend(*pNewWedge);
		pNewWedge->rot.Set(fRotationOffset);

		pNewWedge->GetLeaf().SetState(initStructRef.pWedgeWeightTable->GetEntryByIndex(i)->m_iType);
		pNewWedge->GetLeaf().SetAnimPause(true);

		if(initStructRef.wedgeDecal.IsUsed())
		{
			HySprite2d *pNewWedgeDecal = HY_NEW HySprite2d();
			pNewWedgeDecal->Init(initStructRef.wedgeDecal.m_sPrefix.c_str(), initStructRef.wedgeDecal.m_sName.c_str(), pNewWedge);
			pNewWedgeDecal->SetVisible(false);
			m_WedgeDecalList.push_back(pNewWedgeDecal);
		}

		m_WedgeList.push_back(pNewWedge);

		if(i + 1 < uiNumWedges)
		{
			if(m_WedgeAngleList.empty())
				fRotationOffset -= 360.0f / uiNumWedges;
			else
			{
				fRotationOffset -= m_WedgeAngleList[i] * 0.5f;
				fRotationOffset -= m_WedgeAngleList[i + 1] * 0.5f;
			}
		}
	}

	ChildAppend(m_Frame);
	ChildAppend(m_Notches);
}

/*virtual*/ Wheel::SpinEnt::~SpinEnt()
{
	for(auto iter = m_WedgeTextMap.begin(); iter != m_WedgeTextMap.end(); ++iter)
	{
		for(uint32 i = 0; i < iter->second.size(); ++i)
			delete iter->second[i];
		iter->second.clear();
	}

	for(uint32 i = 0; i < static_cast<uint32>(m_WedgeDecalList.size()); ++i)
		delete m_WedgeDecalList[i];

	for(uint32 i = 0; i < static_cast<uint32>(m_WedgeList.size()); ++i)
		delete m_WedgeList[i];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Wheel::Wheel(const WheelInit &initStructRef, HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_Wheel, pParent),
	m_SpinEnt(initStructRef),
	m_Ticker(initStructRef.ticker.m_sPrefix.c_str(), initStructRef.ticker.m_sName.c_str()),
	m_pWeightTable(initStructRef.pWedgeWeightTable),
	m_bSpinClockwise(true),
	m_bIsClockwise(m_bSpinClockwise),
	m_fLastDegLocation(0.0f),
	m_fSpinWindUpAmt(initStructRef.spinWindUpAmt),
	m_fSpinWindUpDuration(initStructRef.spinWindUpDuration),
	m_fSpinStartDuration(initStructRef.spinStartDuration),
	m_fSpinSpeed(initStructRef.spinSpeed),
	m_fSpinDuration(initStructRef.spinDuration),
	m_fSpinDegsToStop(initStructRef.spinDegsToStop),
	m_fElapsedTime(0.0f),
	m_eState(STATE_Idle),
	m_bForceContinueSpin(false),
	m_bSlammed(false),
	m_uiCurIndex(0),
	m_uiDestinationIndex(0),
#if LG_USE_XACT
	m_iSndID_Ticker(-1),
#endif
	m_fpTickerFlingCallback(nullptr),
	m_pTickerFlingCallbackParam(nullptr),
	m_fTickerNotchHalfWidthDegs(initStructRef.tickerNotchWidthDegs * 0.5f),
	m_fTickerTiltAmtDeg(initStructRef.tickerDegTiltAmt),
	m_fTickerLengthDeg(initStructRef.tickerLengthDeg),
	m_eTickerState(TICKERSTATE_Idle),
	m_fTickerPos(0.0f),
	m_fTickerNotchPercentage(1.0f)
{
	m_Base.Init(initStructRef.base.m_sPrefix.c_str(), initStructRef.base.m_sName.c_str(), this);
	ChildAppend(m_SpinEnt);
	ChildAppend(m_Ticker);
	m_Hub.Init(initStructRef.hub.m_sPrefix.c_str(), initStructRef.hub.m_sName.c_str(), this);

	for(uint32 i = 0; i < static_cast<uint32>(m_SpinEnt.m_WedgeList.size()); ++i)
	{
		HyAssert(GetWedgeAngle(i) * 0.5f > (m_fTickerNotchHalfWidthDegs + m_fTickerLengthDeg + fSAFETY_MARGIN), "Wedge index '" << i << "' is not wide enough for the ticker to fling into");
	}

	m_Ticker.pos.Set(initStructRef.tickerOffset);
	float fTickerDegreeLocation = glm::degrees(atan2(m_Ticker.pos.Y(), m_Ticker.pos.X()));
	fTickerDegreeLocation -= 90.0f; // Our 0.0 is at the top, not to the right like in mathematics

	m_Ticker.rot.Set(fTickerDegreeLocation);
	for(uint32 i = 0; i < m_SpinEnt.ChildCount(); ++i)
		m_SpinEnt.ChildGet(i)->rot.Offset(fTickerDegreeLocation);
}

/*virtual*/ Wheel::~Wheel()
{
}

/*virtual*/ void Wheel::Show(float fDuration) /*override*/
{
	SetVisible(true);
}

/*virtual*/ void Wheel::Hide(float fDuration) /*override*/
{
	SetVisible(false);
}

void Wheel::NudgeCounterClockwise(float fDegrees)
{
	if(IsIdle())
		m_SpinEnt.rot.Offset(fDegrees);
}

void Wheel::NudgeClockwise(float fDegrees)
{
	if(IsIdle())
		m_SpinEnt.rot.Offset(-fDegrees);
}

HyEntityWrapper2d<HySprite2d> &Wheel::GetWedge(uint32 uiWedgeIndex)
{
	HyAssert(uiWedgeIndex < static_cast<uint32>(m_SpinEnt.m_WedgeList.size()), "Wheel::GetWedge was passed an invalid index : " << uiWedgeIndex);
	return *m_SpinEnt.m_WedgeList[uiWedgeIndex];
}

HySprite2d &Wheel::GetWedgeDecal(uint32 uiWedgeIndex)
{
	HyAssert(uiWedgeIndex < static_cast<uint32>(m_SpinEnt.m_WedgeDecalList.size()), "Wheel::GetWedgeDecal was passed an invalid index : " << uiWedgeIndex);
	return *m_SpinEnt.m_WedgeDecalList[uiWedgeIndex];
}

HyEntityWrapper2d<HySprite2d> &Wheel::GetTicker()
{
	return m_Ticker;
}

HySprite2d &Wheel::GetNotches()
{
	return m_SpinEnt.m_Notches;
}

HySprite2d &Wheel::GetHub()
{
	return m_Hub;
}

HySprite2d &Wheel::GetFrame()
{
	return m_SpinEnt.m_Frame;
}

std::vector<HyText2d *> Wheel::SetTextOnWedge(uint32 uiWedgeIndex, std::string sText, std::string sPrefix, std::string sName, bool bVerticalText, float fEdgeMargin, float fVerticalScaleOffset /*= 0.12f*/, float fVerticalPosOffset /*= 0.0f*/)
{
	bool bAlreadyLoaded = true;//IsLoaded(); // TODO: Fix this
	auto iter = m_SpinEnt.m_WedgeTextMap.find(uiWedgeIndex);

	// If text exists then delete it and have it be recreated
	if(iter != m_SpinEnt.m_WedgeTextMap.end())
	{
		for(uint32 i = 0; i < iter->second.size(); ++i)
			delete iter->second[i];
		iter->second.clear();

		m_SpinEnt.m_WedgeTextMap.erase(iter);
	}

	std::vector<HyText2d *> newTextList;

	float fPosY = m_SpinEnt.m_WedgeList[uiWedgeIndex]->GetLeaf().GetFrameHeight() - fEdgeMargin;
	float fScale = 1.0f;

	uint32 uiNumTexts = bVerticalText ? static_cast<uint32>(sText.size()) : 1;
	for(uint32 i = 0; i < uiNumTexts; ++i)
	{
		HyText2d *pText = HY_NEW HyText2d();
		pText->Init(sPrefix.c_str(), sName.c_str(), m_SpinEnt.m_WedgeList[uiWedgeIndex]);

		if(bVerticalText)
		{
			std::string sLetter(1, sText[i]);
			pText->SetText(sLetter);
		}
		else
			pText->SetText(sText);

		fPosY -= pText->GetHeight() + fVerticalPosOffset;
		fScale -= fVerticalScaleOffset;

		pText->pos.Set(0.0f, fPosY);
		pText->scale.Set(fScale, fScale);
		//pText->SetDisplayOrder(m_SpinEnt.m_Frame.GetDisplayOrder());
		pText->SetAsLine();
		pText->SetAlignment(HYALIGN_Center);

		newTextList.push_back(pText);
	}

	if(bAlreadyLoaded)
	{
		for(uint32 i = 0; i < static_cast<uint32>(newTextList.size()); ++i)
			newTextList[i]->Load();
	}

	m_SpinEnt.m_WedgeTextMap[uiWedgeIndex] = newTextList;

	return m_SpinEnt.m_WedgeTextMap[uiWedgeIndex];
}

bool Wheel::IsIdle()
{
	return m_eState == STATE_Idle;
}

bool Wheel::IsStopping()
{
	return m_eState == STATE_Stopping;
}

uint32 Wheel::GetNumWedges()
{
	return static_cast<uint32>(m_SpinEnt.m_WedgeList.size());
}

void Wheel::ForceContinueSpin(bool bForce)
{
	m_bForceContinueSpin = bForce;
}

uint32 Wheel::GetCurrentIndex()
{
	return m_uiCurIndex;
}

HyAnimFloat &Wheel::GetRotation()
{
	return m_SpinEnt.rot;
}

const WeightTable *Wheel::GetWeightTable()
{
	return m_pWeightTable;
}

int Wheel::GetWedgeValue(uint32 uiWedgeIndex)
{
	return m_pWeightTable->GetEntryByIndex(uiWedgeIndex)->m_iValue;
}

float Wheel::GetWedgeAngle(uint32 uiWedgeIndex)
{
	HyAssert(uiWedgeIndex < static_cast<uint32>(m_SpinEnt.m_WedgeList.size()), "Wheel::GetWedgeAngle was passed an invalid index : " << uiWedgeIndex);

	if(m_SpinEnt.m_WedgeAngleList.empty())
		return 360.0f / GetNumWedges();

	return m_SpinEnt.m_WedgeAngleList[uiWedgeIndex];
}

void Wheel::Reset(uint32 uiWedgeIndex)
{
	m_eState = STATE_Idle;

	float fResetDegree = 0.0f;
	for(uint32 i = 0; i < uiWedgeIndex; ++i)
		fResetDegree += GetWedgeAngle(i);

	m_SpinEnt.rot.Set(fResetDegree);
}

uint32 Wheel::GetDestinationIndex()
{
	return m_uiDestinationIndex;
}

void Wheel::PrepSpin(uint32 uiDestinationWedgeIndex)
{
	HyAssert(uiDestinationWedgeIndex < static_cast<uint32>(m_SpinEnt.m_WedgeList.size()), "Wheel::PrepSpin was passed an invalid destination index : " << uiDestinationWedgeIndex);
	m_uiDestinationIndex = uiDestinationWedgeIndex;
}

bool Wheel::BeginSpin(bool bDoWindUp /*= true*/)
{
	if(m_eState != STATE_Idle)
	{
		HyLogWarning("Wheel::BeginSpin invoked when wheel was not in idle state");
		return false;
	}

	m_bSlammed = false;

	HyLog("Starting Wheel Spin with destination: " << m_uiDestinationIndex);

	if(bDoWindUp)
		m_eState = STATE_StartWindUp;
	else
		m_eState = STATE_Spinning;

	return true;
}

void Wheel::SetTickerFlingCallback(std::function<void(Wheel *, void *)> func, void *pParam)
{
	m_fpTickerFlingCallback = func;
	m_pTickerFlingCallbackParam = pParam;
}

void Wheel::Slam()
{
	if(m_bForceContinueSpin)
		return;

	m_bSlammed = true;
}

/*virtual*/ void Wheel::OnUpdate() /*override*/
{
	float fTempRot = GetRotation().Get();
	NormalizeRotation(fTempRot);
	const float fCUR_ROTATION = fTempRot;

	if(m_fLastDegLocation < fCUR_ROTATION)
		m_bIsClockwise = false;
	else if(m_fLastDegLocation > fCUR_ROTATION)
		m_bIsClockwise = true;

	switch(m_eState)
	{
	case STATE_Idle:
		break;

	case STATE_StartWindUp:
		m_fTickerNotchPercentage = 1.0f;

		m_SpinEnt.rot.TweenOffset(m_bSpinClockwise ? m_fSpinWindUpAmt : -m_fSpinWindUpAmt, m_fSpinWindUpDuration, HyTween::QuadInOut);
		m_eState = STATE_WindingUp;
		break;

	case STATE_WindingUp:
		if(m_SpinEnt.rot.IsAnimating() == false || m_bSlammed)
		{
			m_fTickerNotchPercentage = 0.0f;

			float fStartUpDist = 0.5f * (m_fSpinSpeed / m_fSpinStartDuration) * (m_fSpinStartDuration * m_fSpinStartDuration);
			m_SpinEnt.rot.TweenOffset(m_bSpinClockwise ? -fStartUpDist : fStartUpDist, m_fSpinStartDuration, HyTween::QuadIn);

			m_eState = STATE_StartSpin;
		}
		break;

	case STATE_StartSpin:
		if(m_SpinEnt.rot.IsAnimating() == false || m_bSlammed)
		{
			m_fElapsedTime = 0.0f;
			m_eState = STATE_Spinning;
		}
		break;

	case STATE_Spinning:
		m_SpinEnt.rot.Offset((m_fSpinSpeed * HyEngine::DeltaTime()) * (m_bSpinClockwise ? -1.0f : 1.0f));

		m_fElapsedTime += HyEngine::DeltaTime();
		if(m_fElapsedTime >= m_fSpinDuration || m_bSlammed)
		{
			if(m_bForceContinueSpin == false)
				m_eState = STATE_PrepStop;
		}
		break;

	case STATE_PrepStop: {
		// Find which degree our actual destination is
		float fDestinationDegree = 0.0f;
		for(uint32 i = 0; i < m_uiDestinationIndex; ++i)
		{
			fDestinationDegree += GetWedgeAngle(i) * 0.5f;
			fDestinationDegree += GetWedgeAngle(i + 1) * 0.5f;
		}

		// Then offset it by how many degrees we want the stop to take
		fDestinationDegree += m_bSpinClockwise ? m_fSpinDegsToStop : -m_fSpinDegsToStop;
		NormalizeRotation(fDestinationDegree);

		float fOffsetAmt = (m_fSpinSpeed * HyEngine::DeltaTime()) * (m_bSpinClockwise ? -1.0f : 1.0f);
		if((m_bSpinClockwise && m_fLastDegLocation > fDestinationDegree && (fCUR_ROTATION + fOffsetAmt) <= fDestinationDegree) ||
			(m_bSpinClockwise == false && m_fLastDegLocation < fDestinationDegree && (fCUR_ROTATION + fOffsetAmt) >= fDestinationDegree))
		{
			float fHalfDestWedgeAngle = GetWedgeAngle(m_uiDestinationIndex) * 0.5f;
			float fMin = 0.0f;
			float fMax = 0.0f;
			if(m_bSpinClockwise)
			{
				fMin = (fHalfDestWedgeAngle - (m_fTickerNotchHalfWidthDegs + m_fTickerLengthDeg + fSAFETY_MARGIN)) * -1.0f;
				fMax = fHalfDestWedgeAngle + m_fTickerNotchHalfWidthDegs + m_fTickerLengthDeg - fSAFETY_MARGIN;
			}
			else
			{
				fMin = (fHalfDestWedgeAngle + m_fTickerNotchHalfWidthDegs + m_fTickerLengthDeg - fSAFETY_MARGIN) * -1.0f;
				fMax = fHalfDestWedgeAngle - (m_fTickerNotchHalfWidthDegs + m_fTickerLengthDeg + fSAFETY_MARGIN);
			}

			float fRange = HyRand::Range(fMin, fMax);
			HyLog("Wheel Stop Range: [" << fMin << " -> " << fMax << "] = " << fRange);

			m_fDegsLeftForStop = m_fSpinDegsToStop + fRange;

			m_SpinEnt.rot.Set(fDestinationDegree);
			m_eState = STATE_Stopping;
		}
		else
			m_SpinEnt.rot.Offset(fOffsetAmt);
		break; }

	case STATE_Stopping: {
		float fOffsetAmt = (m_fSpinSpeed * HyEngine::DeltaTime()) * (m_bSpinClockwise ? -1.0f : 1.0f);
		fOffsetAmt *= HyMath::Clamp(m_fDegsLeftForStop / m_fSpinDegsToStop, 0.025f, 1.0f);

		m_fTickerNotchPercentage = 1.0f - (m_fDegsLeftForStop / m_fSpinDegsToStop);

		m_SpinEnt.rot.Offset(fOffsetAmt);
		m_fDegsLeftForStop -= abs(fOffsetAmt);

		if(m_fDegsLeftForStop <= 0)
		{
			m_SpinEnt.rot.Set(fCUR_ROTATION);

			m_bSlammed = m_bForceContinueSpin = false;

			if(m_eTickerState == TICKERSTATE_Idle)
				m_eState = STATE_Idle;
			else
			{
				float fFlingBackDist = 0.0f;
				if(m_eTickerState == TICKERSTATE_Right)
				{
					if(m_fTickerPos >= 0.0f)
						fFlingBackDist = m_fTickerNotchHalfWidthDegs - m_fTickerPos + 1.0f; // +1 is for a little physics effect
					else
						fFlingBackDist = abs(m_fTickerPos) + (m_fTickerNotchHalfWidthDegs * 0.75f); // Will still hang on the notch a little
				}
				else
				{
					if(m_fTickerPos <= 0.0f)
						fFlingBackDist = m_fTickerNotchHalfWidthDegs - abs(m_fTickerPos) + 1.0f; // +1 is for a little physics effect
					else
						fFlingBackDist = m_fTickerPos + (m_fTickerNotchHalfWidthDegs * 0.75f); // Will still hang on the notch a little

					fFlingBackDist *= -1.0f;
				}

				m_SpinEnt.rot.TweenOffset(fFlingBackDist, fMAX_FLINGBACK_DUR /** (abs(fFlingBackDist) / ((m_fTickerNotchHalfWidthDegs*2.0f)+m_fTickerLengthDeg))*/, HyTween::QuadInOut);
				m_eState = STATE_FlingBack;
			}
		}
		break; }

	case STATE_FlingBack:
		if(m_SpinEnt.rot.IsAnimating() == false)
			m_eState = STATE_Idle;
		break;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Set ticker orientation
	switch(m_eTickerState)
	{
	case TICKERSTATE_Idle: {
		float fDeg = 360.0f;
		for(uint32 i = 0; i < GetNumWedges(); ++i)
		{
			fDeg -= GetWedgeAngle(i) * 0.5f;
			if(m_bIsClockwise)
			{
				if(fCUR_ROTATION > fDeg && fCUR_ROTATION < (fDeg + m_fTickerNotchHalfWidthDegs))
				{
					m_fTickerPos = fCUR_ROTATION - fDeg;
					m_eTickerState = TICKERSTATE_Right;
					break;
				}
			}
			else
			{
				if(fCUR_ROTATION > (fDeg - m_fTickerNotchHalfWidthDegs) && fCUR_ROTATION < fDeg)
				{
					m_fTickerPos = fCUR_ROTATION - fDeg;
					m_eTickerState = TICKERSTATE_Left;
					break;
				}
			}
			fDeg -= GetWedgeAngle((i + 1) % GetNumWedges()) * 0.5f;
		}
		break; }

	case TICKERSTATE_Right: {
		float fSpinOffset = (fCUR_ROTATION - m_fLastDegLocation);
		if(0.0f != fSpinOffset)
		{
			m_fTickerPos += fSpinOffset;
			if(m_fTickerPos > m_fTickerNotchHalfWidthDegs)
			{
				m_Ticker.GetLeaf().rot.Set(0.0f);

				m_fTickerPos = 0.0f;
				m_eTickerState = TICKERSTATE_Idle;
			}
			else if(m_fTickerPos < -(m_fTickerLengthDeg + m_fTickerNotchHalfWidthDegs) * m_fTickerNotchPercentage)
			{
#if LG_USE_XACT
				if(m_iSndID_Ticker > -1)
					GetSndBank()->Play(m_iSndID_Ticker);
#endif

				if(m_Ticker.GetLeaf().rot.Get() < m_fTickerTiltAmtDeg * 0.5f)
					m_Ticker.GetLeaf().rot.Set(m_fTickerTiltAmtDeg * 0.5f);
				m_Ticker.GetLeaf().rot.Tween(0.0f, 0.25f, HyTween::BounceOut);

				m_fTickerPos = 0.0f;
				m_eTickerState = TICKERSTATE_Idle;

				if(m_fpTickerFlingCallback)
					m_fpTickerFlingCallback(this, m_pTickerFlingCallbackParam);
			}
			else
			{
				if(m_fTickerPos >= 0.0f)
					m_Ticker.GetLeaf().rot.Set((m_fTickerTiltAmtDeg * 0.8f) * (1.0f - (m_fTickerPos / m_fTickerNotchHalfWidthDegs)));
				else if(m_fTickerPos >= -m_fTickerNotchHalfWidthDegs)
					m_Ticker.GetLeaf().rot.Set((m_fTickerTiltAmtDeg * 0.8f) + ((m_fTickerTiltAmtDeg * 0.1f) * (abs(m_fTickerPos) / m_fTickerNotchHalfWidthDegs)));
				else if(m_fTickerPos >= -(m_fTickerLengthDeg + m_fTickerNotchHalfWidthDegs))
					m_Ticker.GetLeaf().rot.Set((m_fTickerTiltAmtDeg * 0.9f) + ((m_fTickerTiltAmtDeg * 0.1f) * (abs(m_fTickerPos + m_fTickerNotchHalfWidthDegs) / m_fTickerLengthDeg)));
			}
		}
		break; }

	case TICKERSTATE_Left: {
		float fSpinOffset = (fCUR_ROTATION - m_fLastDegLocation);
		if(0.0f != fSpinOffset)
		{
			m_fTickerPos += fSpinOffset;
			if(m_fTickerPos < -m_fTickerNotchHalfWidthDegs)
			{
				m_Ticker.GetLeaf().rot.Set(0.0f);

				m_fTickerPos = 0.0f;
				m_eTickerState = TICKERSTATE_Idle;
			}
			else if(m_fTickerPos > (m_fTickerLengthDeg + m_fTickerNotchHalfWidthDegs) * m_fTickerNotchPercentage)
			{
#if LG_USE_XACT
				if(m_iSndID_Ticker > -1)
					GetSndBank()->Play(m_iSndID_Ticker);
#endif

				if(m_Ticker.GetLeaf().rot.Get() > m_fTickerTiltAmtDeg * -0.5f)
					m_Ticker.GetLeaf().rot.Set(m_fTickerTiltAmtDeg * -0.5f);
				m_Ticker.GetLeaf().rot.Tween(0.0f, 0.25f, HyTween::BounceOut);

				m_fTickerPos = 0.0f;
				m_eTickerState = TICKERSTATE_Idle;

				if(m_fpTickerFlingCallback)
					m_fpTickerFlingCallback(this, m_pTickerFlingCallbackParam);
			}
			else
			{
				if(m_fTickerPos <= 0.0f)
					m_Ticker.GetLeaf().rot.Set((-m_fTickerTiltAmtDeg * 0.8f) * (1.0f - (abs(m_fTickerPos) / m_fTickerNotchHalfWidthDegs)));
				else if(m_fTickerPos <= m_fTickerNotchHalfWidthDegs)
					m_Ticker.GetLeaf().rot.Set((-m_fTickerTiltAmtDeg * 0.8f) + ((-m_fTickerTiltAmtDeg * 0.1f) * (m_fTickerPos / m_fTickerNotchHalfWidthDegs)));
				else if(m_fTickerPos <= (m_fTickerLengthDeg + m_fTickerNotchHalfWidthDegs))
					m_Ticker.GetLeaf().rot.Set((-m_fTickerTiltAmtDeg * 0.9f) + ((-m_fTickerTiltAmtDeg * 0.1f) * ((m_fTickerPos - m_fTickerNotchHalfWidthDegs) / m_fTickerLengthDeg)));
			}
		}
		break; }
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Set current wedge index
	float fSpinRot = GetRotation().Get();
	for(uint32 i = 0; i < GetNumWedges(); ++i)
	{
		if(i == 0)
		{
			if(fSpinRot < GetWedgeAngle(i) * 0.5f || fSpinRot >(360.0f - (GetWedgeAngle(i) * 0.5f)))
			{
				m_uiCurIndex = i;
				break;
			}
			else
				fSpinRot -= GetWedgeAngle(i) * 0.5f;
		}
		else
		{
			if(fSpinRot < GetWedgeAngle(i))
			{
				m_uiCurIndex = i;
				break;
			}
			else
				fSpinRot -= GetWedgeAngle(i);
		}
	}

	m_fLastDegLocation = fCUR_ROTATION;
}

void Wheel::NormalizeRotation(float &fRotOut)
{
	while(fRotOut >= 360.0f)
		fRotOut -= 360.0f;
	while(fRotOut < 0.0f)
		fRotOut += 360.0f;
}
