#include "pch.h"
#include "FightStick.h"

FightStick::FightStick(HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_FightStick, pParent),
	m_Gate("FightStick", "JoystickGate", this),
	m_BallFollow("FightStick", "JoystickBall", this),
	m_Ball("FightStick", "JoystickBall", this),
	m_eOldBallPos(JSGATE_Neutral),
	m_DriveImpact(this),
	m_DriveParry(this),
	m_ButtonLP("FightStick", "ButtonsPressed", this),
	m_ButtonMP("FightStick", "ButtonsPressed", this),
	m_ButtonHP("FightStick", "ButtonsPressed", this),
	m_ButtonLK("FightStick", "ButtonsPressed", this),
	m_ButtonMK("FightStick", "ButtonsPressed", this),
	m_ButtonHK("FightStick", "ButtonsPressed", this),
	m_uiButtonFlags(0),
	m_AssignOverlayBG(this),
	m_AssignOverlayText("", "MainText", this),
	m_fpAssignControllerFunc(nullptr)
{
	m_Gate.SetDisplayOrder(DISPLAYORDER_Gate);
	m_BallFollow.alpha.Set(0.4f);
	for(int i = 0; i < NUM_JSGATES; ++i)
	{
		m_ButtonOverlays[i].Init("FightStick", "ButtonsPressed", this);
		m_ButtonOverlays[i].pos.Set(JSGatePositions[i]);
		m_ButtonOverlays[i].SetVisible(false);
	}

	m_DriveImpact.SetAsBox(75.0f, 200.0f);
	m_DriveImpact.SetTint(HyColor::Red);
	m_DriveImpact.pos.Set(305.0f, -100.0f);
	m_DriveImpact.SetVisible(false);

	m_DriveParry.SetAsBox(75.0f, 200.0f);
	m_DriveParry.SetTint(HyColor::Green);
	m_DriveParry.pos.Set(205.0f, -100.0f);
	m_DriveParry.SetVisible(false);

	m_ButtonLP.pos.Set(142.0f, 50.0f);
	m_ButtonLP.SetTint(HyColor(128, 128, 128));
	
	m_ButtonMP.pos.Set(242.0f, 50.0f);
	m_ButtonMP.SetState(BTNSTATE_MP);
	m_ButtonMP.SetTint(HyColor(128, 128, 128));

	m_ButtonHP.pos.Set(342.0f, 50.0f);
	m_ButtonHP.SetState(BTNSTATE_HP);
	m_ButtonHP.SetTint(HyColor(128, 128, 128));

	m_ButtonLK.pos.Set(142.0f, -50.0f);
	m_ButtonLK.SetState(BTNSTATE_LK);
	m_ButtonLK.SetTint(HyColor(128, 128, 128));

	m_ButtonMK.pos.Set(242.0f, -50.0f);
	m_ButtonMK.SetState(BTNSTATE_MK);
	m_ButtonMK.SetTint(HyColor(128, 128, 128));

	m_ButtonHK.pos.Set(342.0f, -50.0f);
	m_ButtonHK.SetState(BTNSTATE_HK);
	m_ButtonHK.SetTint(HyColor(128, 128, 128));

	b2AABB sceneAabb = GetSceneAABB();
	m_AssignOverlayBG.SetAsBox(sceneAabb.upperBound.x - sceneAabb.lowerBound.x, sceneAabb.upperBound.y - sceneAabb.lowerBound.y);
	m_AssignOverlayBG.pos.Offset(-105.0f, -120.0f);
	m_AssignOverlayBG.SetVisible(false);
	m_AssignOverlayText.SetVisible(false);
	m_AssignOverlayText.SetTextAlignment(HYALIGN_Center);
	m_AssignOverlayText.pos.Offset(145.0f, 0.0f);

	m_fpAssignControllerFunc = [this](IHyController *pController, bool bIsAxis, int32 iKey, HyControllerInputValue value)
	{
		if(bIsAxis || pController->IsGamePad() == false)
			return;

		HyGamePad *pNewGamePad = static_cast<HyGamePad *>(pController);
		m_AssignOverlayText.SetText(pNewGamePad->GetName());

		m_AssignOverlayBG.alpha.Tween(0.0f, 1.0f, HyTween::Linear, 0.42f, [](IHyNode *pThis) { pThis->SetVisible(false); });
		m_AssignOverlayText.alpha.Tween(0.0f, 1.0f, HyTween::Linear, 0.42f, [](IHyNode *pThis) { pThis->SetVisible(false); });

		HyEngine::Input().AssignGamePad(pNewGamePad);
		HyEngine::Input().SetControllerInputListener(nullptr);
	};

	EnableMouseInput(); // When mouse input is detected, listen for all controllers and assign a new game pad

	SetDisplayOrder(DISPLAYORDER_FIGHTSTICK);
}

/*virtual*/ FightStick::~FightStick()
{
}

void FightStick::SetButtonPress(ButtonState eButtonState, bool bPressed)
{
	m_ButtonOverlays[m_eOldBallPos].SetVisible(true);
	m_ButtonOverlays[m_eOldBallPos].SetState(eButtonState);

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

/*virtual*/ void FightStick::OnUpdate() /*override*/
{
	// Joystick
	if(m_PathList.empty() == false)
	{
		if(m_PathList.front()->alpha.Get() == 0.0f)
		{
			delete m_PathList.front();
			m_PathList.pop_front();
		}
	}
	enum DPadFlags
	{
		DPad_Up = 0x1,
		DPad_Down = 0x2,
		DPad_Left = 0x4,
		DPad_Right = 0x8
	};
	uint32 uiDPadFlags = 0;
	if(HyEngine::Input().IsActionDown(FIGHTSTICK_Up))
		uiDPadFlags |= DPad_Up;
	if(HyEngine::Input().IsActionDown(FIGHTSTICK_Down))
		uiDPadFlags |= DPad_Down;
	if(HyEngine::Input().IsActionDown(FIGHTSTICK_Left))
		uiDPadFlags |= DPad_Left;
	if(HyEngine::Input().IsActionDown(FIGHTSTICK_Right))
		uiDPadFlags |= DPad_Right;

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
			SetButtonPress(static_cast<ButtonState>(m_ButtonOverlays[m_eOldBallPos].GetState()), false);

		m_eOldBallPos = eCurBallPos;
	}

	// Buttons
	uint32 uiNewButtonFlags = 0;
	if(HyEngine::Input().IsActionDown(FIGHTSTICK_LK))
		uiNewButtonFlags |= BTNFLAG_LK;
	
	if(HyEngine::Input().IsActionDown(FIGHTSTICK_MK))
		uiNewButtonFlags |= BTNFLAG_MK;

	if(HyEngine::Input().GetGamePadAxis(HYPADAXIS_TriggerRight) > 0.5f) // FIGHTSTICK_HK
		uiNewButtonFlags |= BTNFLAG_HK;

	if(HyEngine::Input().IsActionDown(FIGHTSTICK_LP))
		uiNewButtonFlags |= BTNFLAG_LP;

	if(HyEngine::Input().IsActionDown(FIGHTSTICK_MP))
		uiNewButtonFlags |= BTNFLAG_MP;

	if(HyEngine::Input().IsActionDown(FIGHTSTICK_HP))
		uiNewButtonFlags |= BTNFLAG_HP;

	if(HyEngine::Input().IsActionDown(FIGHTSTICK_DriveImpact))
	{
		m_DriveImpact.SetVisible(true);
		uiNewButtonFlags |= (BTNFLAG_HP | BTNFLAG_HK);
	}
	else
		m_DriveImpact.SetVisible(false);

	if(HyEngine::Input().IsActionDown(FIGHTSTICK_Parry))
	{
		m_DriveParry.SetVisible(true);
		uiNewButtonFlags |= (BTNFLAG_MP | BTNFLAG_MK);
	}
	else
		m_DriveParry.SetVisible(false);

	if(m_uiButtonFlags != uiNewButtonFlags)
	{
		const float fButtonPressScale = 1.15f;

		if(!(m_uiButtonFlags & BTNFLAG_LP) && uiNewButtonFlags & BTNFLAG_LP)
		{
			m_ButtonLP.SetTint(HyColor::White);
			m_ButtonLP.scale.Tween(fButtonPressScale, fButtonPressScale, 0.1f, HyTween::QuadOut);
			SetButtonPress(BTNSTATE_LP, true);
		}
		else if(m_uiButtonFlags & BTNFLAG_LP && !(uiNewButtonFlags & BTNFLAG_LP))
		{
			m_ButtonLP.SetTint(HyColor(128, 128, 128));
			m_ButtonLP.scale.Tween(1.0f, 1.0f, 0.1f, HyTween::QuadOut);
			SetButtonPress(BTNSTATE_LP, false);
		}

		if(!(m_uiButtonFlags & BTNFLAG_MP) && uiNewButtonFlags & BTNFLAG_MP)
		{
			m_ButtonMP.SetTint(HyColor::White);
			m_ButtonMP.scale.Tween(fButtonPressScale, fButtonPressScale, 0.1f, HyTween::QuadOut);
			SetButtonPress(BTNSTATE_MP, true);
		}
		else if(m_uiButtonFlags & BTNFLAG_MP && !(uiNewButtonFlags & BTNFLAG_MP))
		{
			m_ButtonMP.SetTint(HyColor(128, 128, 128));
			m_ButtonMP.scale.Tween(1.0f, 1.0f, 0.1f, HyTween::QuadOut);
			SetButtonPress(BTNSTATE_MP, false);
		}

		if(!(m_uiButtonFlags & BTNFLAG_HP) && uiNewButtonFlags & BTNFLAG_HP)
		{
			m_ButtonHP.SetTint(HyColor::White);
			m_ButtonHP.scale.Tween(fButtonPressScale, fButtonPressScale, 0.1f, HyTween::QuadOut);
			SetButtonPress(BTNSTATE_HP, true);
		}
		else if(m_uiButtonFlags & BTNFLAG_HP && !(uiNewButtonFlags & BTNFLAG_HP))
		{
			m_ButtonHP.SetTint(HyColor(128, 128, 128));
			m_ButtonHP.scale.Tween(1.0f, 1.0f, 0.1f, HyTween::QuadOut);
			SetButtonPress(BTNSTATE_HP, false);
		}

		if(!(m_uiButtonFlags & BTNFLAG_LK) && uiNewButtonFlags & BTNFLAG_LK)
		{
			m_ButtonLK.SetTint(HyColor::White);
			m_ButtonLK.scale.Tween(fButtonPressScale, fButtonPressScale, 0.1f, HyTween::QuadOut);
			SetButtonPress(BTNSTATE_LK, true);
		}
		else if(m_uiButtonFlags & BTNFLAG_LK && !(uiNewButtonFlags & BTNFLAG_LK))
		{
			m_ButtonLK.SetTint(HyColor(128, 128, 128));
			m_ButtonLK.scale.Tween(1.0f, 1.0f, 0.1f, HyTween::QuadOut);
			SetButtonPress(BTNSTATE_LK, false);
		}

		if(!(m_uiButtonFlags & BTNFLAG_MK) && uiNewButtonFlags & BTNFLAG_MK)
		{
			m_ButtonMK.SetTint(HyColor::White);
			m_ButtonMK.scale.Tween(fButtonPressScale, fButtonPressScale, 0.1f, HyTween::QuadOut);
			SetButtonPress(BTNSTATE_MK, true);
		}
		else if(m_uiButtonFlags & BTNFLAG_MK && !(uiNewButtonFlags & BTNFLAG_MK))
		{
			m_ButtonMK.SetTint(HyColor(128, 128, 128));
			m_ButtonMK.scale.Tween(1.0f, 1.0f, 0.1f, HyTween::QuadOut);
			SetButtonPress(BTNSTATE_MK, false);
		}

		if(!(m_uiButtonFlags & BTNFLAG_HK) && uiNewButtonFlags & BTNFLAG_HK)
		{
			m_ButtonHK.SetTint(HyColor::White);
			m_ButtonHK.scale.Tween(fButtonPressScale, fButtonPressScale, 0.1f, HyTween::QuadOut);
			SetButtonPress(BTNSTATE_HK, true);
		}
		else if(m_uiButtonFlags & BTNFLAG_HK && !(uiNewButtonFlags & BTNFLAG_HK))
		{
			m_ButtonHK.SetTint(HyColor(128, 128, 128));
			m_ButtonHK.scale.Tween(1.0f, 1.0f, 0.1f, HyTween::QuadOut);
			SetButtonPress(BTNSTATE_HK, false);
		}

		m_uiButtonFlags = uiNewButtonFlags;
	}
}

/*virtual*/ void FightStick::OnMouseClicked() /*override*/
{
	if(m_AssignOverlayBG.IsVisible() == false)
	{
		m_AssignOverlayBG.SetVisible(true);
		m_AssignOverlayBG.alpha.Set(0.5f);
		m_AssignOverlayText.SetVisible(true);
		m_AssignOverlayText.alpha.Set(1.0f);
		m_AssignOverlayText.SetText("Set Controller!");

		HyEngine::Input().SetControllerInputListener(m_fpAssignControllerFunc);
	}
}
