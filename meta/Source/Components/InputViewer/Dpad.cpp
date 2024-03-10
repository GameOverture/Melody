#include "pch.h"
#include "Dpad.h"

Dpad::Dpad(const HyNodePath &gatePath, const HyNodePath &pressPath, const HyNodePath &buttonsPath, HyEntity2d *pParent /*= nullptr*/) :
	HyEntity2d(pParent),
	m_Gate(gatePath, this),
	m_BallFollow(pressPath, this),
	m_Ball(pressPath, this),
	m_eOldBallPos(JSGATE_Neutral)
{
	m_Gate.SetDisplayOrder(DISPLAYORDER_Gate);
	m_BallFollow.alpha.Set(0.4f);
	for(int i = 0; i < NUM_JSGATES; ++i)
	{
		m_ButtonOverlays[i].Init(buttonsPath, this);
		m_ButtonOverlays[i].pos.Set(JSGatePositions[i]);
		m_ButtonOverlays[i].SetVisible(false);
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
		m_Ball.pos.Set(JSGatePositions[eCurBallPos]);
		m_BallFollow.pos.Set(JSGatePositions[m_eOldBallPos]);
		m_BallFollow.pos.Tween(JSGatePositions[eCurBallPos].x, JSGatePositions[eCurBallPos].y, 0.1f, HyTween::Linear);

		m_PathList.push_back(HY_NEW GatePath(m_eOldBallPos, eCurBallPos, this));

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
