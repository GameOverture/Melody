#include "pch.h"
#include "Dpad.h"

Dpad::Dpad(const HyNodePath &gatePath, const HyNodePath &pressPath, const HyNodePath &buttonsPath, float fOrthoDist, float fDiagDist, HyColor pathColor, float fPathRadius, HyEntity2d *pParent /*= nullptr*/) :
	HyEntity2d(pParent),
	m_PathColor(pathColor),
	m_fPathRadius(fPathRadius),
	m_Gate(gatePath, this),
	m_BallFollow(pressPath, this),
	m_Ball(pressPath, this),
	m_eOldBallPos(JSGATE_Neutral)
{
	HySetVec(m_GatePositions[JSGATE_Neutral], 0.0f, 0.0f);
	HySetVec(m_GatePositions[JSGATE_Up], 0.0f, fOrthoDist);
	HySetVec(m_GatePositions[JSGATE_Down], 0.0f, -fOrthoDist);
	HySetVec(m_GatePositions[JSGATE_Left], -fOrthoDist, 0.0f);
	HySetVec(m_GatePositions[JSGATE_Right], fOrthoDist, 0.0f);
	HySetVec(m_GatePositions[JSGATE_UpLeft], -fDiagDist, fDiagDist);
	HySetVec(m_GatePositions[JSGATE_UpRight], fDiagDist, fDiagDist);
	HySetVec(m_GatePositions[JSGATE_DownLeft], -fDiagDist, -fDiagDist);
	HySetVec(m_GatePositions[JSGATE_DownRight], fDiagDist, -fDiagDist);

	m_Gate.SetDisplayOrder(DISPLAYORDER_Gate);
	
	m_BallFollow.alpha.Set(0.4f);
	m_BallFollow.SetDisplayOrder(DISPLAYORDER_BallTop - 1);
	m_Ball.SetDisplayOrder(DISPLAYORDER_BallTop);

	for(int i = 0; i < NUM_JSGATES; ++i)
	{
		m_ButtonOverlays[i].Init(buttonsPath, this);
		m_ButtonOverlays[i].pos.Set(m_GatePositions[i]);
		m_ButtonOverlays[i].SetVisible(false);
		m_ButtonOverlays[i].SetDisplayOrder(DISPLAYORDER_Buttons);
	}
}

/*virtual*/ Dpad::~Dpad()
{
}

void Dpad::ApplyInput(uint32 uiDPadFlags)
{
	JSGatePoint eCurBallPos = JSGATE_Neutral;
	if(uiDPadFlags & DPad_Up)
	{
		if(uiDPadFlags & DPad_Left)
			eCurBallPos = JSGATE_UpLeft;
		else if(uiDPadFlags & DPad_Right)
			eCurBallPos = JSGATE_UpRight;
		else
			eCurBallPos = JSGATE_Up;
	}
	else if(uiDPadFlags & DPad_Down)
	{
		if(uiDPadFlags & DPad_Left)
			eCurBallPos = JSGATE_DownLeft;
		else if(uiDPadFlags & DPad_Right)
			eCurBallPos = JSGATE_DownRight;
		else
			eCurBallPos = JSGATE_Down;
	}
	else if(uiDPadFlags & DPad_Left)
		eCurBallPos = JSGATE_Left;
	else if(uiDPadFlags & DPad_Right)
		eCurBallPos = JSGATE_Right;
	else
		eCurBallPos = JSGATE_Neutral;

	if(m_eOldBallPos != eCurBallPos)
	{
		m_Ball.pos.Set(m_GatePositions[eCurBallPos]);
		m_BallFollow.pos.Set(m_GatePositions[m_eOldBallPos]);
		m_BallFollow.pos.Tween(m_GatePositions[eCurBallPos].x, m_GatePositions[eCurBallPos].y, 0.1f, HyTween::Linear);

		m_PathList.push_back(HY_NEW GatePath(m_GatePositions[m_eOldBallPos], m_GatePositions[eCurBallPos], m_PathColor, m_fPathRadius, this));

		if(m_ButtonOverlays[m_eOldBallPos].IsVisible())
			SetButtonPress(m_ButtonOverlays[m_eOldBallPos].GetState(), false);

		m_eOldBallPos = eCurBallPos;
	}
}

void Dpad::SetButtonPress(int iButtonState, bool bPressed)
{
	m_ButtonOverlays[m_eOldBallPos].SetVisible(true);
	m_ButtonOverlays[m_eOldBallPos].SetState(iButtonState);

	m_ButtonOverlays[m_eOldBallPos].alpha.Set(1.0f);
	if(bPressed)
	{
		m_ButtonOverlays[m_eOldBallPos].scale.Set(0.9f);
		m_ButtonOverlays[m_eOldBallPos].scale.Tween(1.0f, 1.0f, 0.1f, HyTween::QuadOut);
	}
	else
	{
		float fRemainingDur = m_ButtonOverlays[m_eOldBallPos].scale.GetAnimRemainingDuration();
		m_ButtonOverlays[m_eOldBallPos].scale.Tween(0.8f, 0.8f, 0.1f, HyTween::QuadOut, fRemainingDur);
		m_ButtonOverlays[m_eOldBallPos].alpha.Tween(0.0f, 0.1f, HyTween::Linear, fRemainingDur, [](IHyNode *pThis) { pThis->SetVisible(false); });
	}
}

/*virtual*/ void Dpad::OnUpdate() /*override*/
{
	// Cleanup old paths
	if(m_PathList.empty() == false)
	{
		if(m_PathList.front()->alpha.Get() == 0.0f)
		{
			delete m_PathList.front();
			m_PathList.pop_front();
		}
	}
}
