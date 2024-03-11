#include "pch.h"
#include "StreetFighter.h"

StreetFighter::StreetFighter(HyEntity2d *pParent /*= nullptr*/) :
	IController(pParent),
	m_Dpad(HyNodePath("InputViewer/StreetFighter/DpadGate"), HyNodePath("InputViewer/StreetFighter/DpadBall"), HyNodePath("InputViewer/StreetFighter/Buttons"), 85.0f, 75.0f, HyColor::Red, 16.0f, this),
	m_DriveImpact(this),
	m_DriveParry(this),
	m_ButtonLP("InputViewer/StreetFighter/Buttons", this),
	m_ButtonMP("InputViewer/StreetFighter/Buttons", this),
	m_ButtonHP("InputViewer/StreetFighter/Buttons", this),
	m_ButtonLK("InputViewer/StreetFighter/Buttons", this),
	m_ButtonMK("InputViewer/StreetFighter/Buttons", this),
	m_ButtonHK("InputViewer/StreetFighter/Buttons", this),
	m_uiButtonFlags(0),
	m_AssignOverlayBG(this),
	m_AssignOverlayText("", "MainText", this),
	m_fpAssignControllerFunc(nullptr)
{
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

	pos.Set(1200.0f, 80.0f);
	scale.Set(0.75f, 0.75f);
	EnableMouseInput(); // When mouse input is detected, listen for all controllers and assign a new game pad
	SetDisplayOrder(DISPLAYORDER_Buttons);
}

/*virtual*/ StreetFighter::~StreetFighter()
{
}

/*virtual*/ void StreetFighter::ApplyInputs() /*override*/
{
	// DPAD
	uint32 uiDPadFlags = 0;
	if(HyEngine::Input().IsActionDown(FIGHTSTICK_Up))
		uiDPadFlags |= DPad_Up;
	if(HyEngine::Input().IsActionDown(FIGHTSTICK_Down))
		uiDPadFlags |= DPad_Down;
	if(HyEngine::Input().IsActionDown(FIGHTSTICK_Left))
		uiDPadFlags |= DPad_Left;
	if(HyEngine::Input().IsActionDown(FIGHTSTICK_Right))
		uiDPadFlags |= DPad_Right;

	m_Dpad.ApplyInput(uiDPadFlags);

	// BUTTONS
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
			m_Dpad.SetButtonPress(BTNSTATE_LP, true);
		}
		else if(m_uiButtonFlags & BTNFLAG_LP && !(uiNewButtonFlags & BTNFLAG_LP))
		{
			m_ButtonLP.SetTint(HyColor(128, 128, 128));
			m_ButtonLP.scale.Tween(1.0f, 1.0f, 0.1f, HyTween::QuadOut);
			m_Dpad.SetButtonPress(BTNSTATE_LP, false);
		}

		if(!(m_uiButtonFlags & BTNFLAG_MP) && uiNewButtonFlags & BTNFLAG_MP)
		{
			m_ButtonMP.SetTint(HyColor::White);
			m_ButtonMP.scale.Tween(fButtonPressScale, fButtonPressScale, 0.1f, HyTween::QuadOut);
			m_Dpad.SetButtonPress(BTNSTATE_MP, true);
		}
		else if(m_uiButtonFlags & BTNFLAG_MP && !(uiNewButtonFlags & BTNFLAG_MP))
		{
			m_ButtonMP.SetTint(HyColor(128, 128, 128));
			m_ButtonMP.scale.Tween(1.0f, 1.0f, 0.1f, HyTween::QuadOut);
			m_Dpad.SetButtonPress(BTNSTATE_MP, false);
		}

		if(!(m_uiButtonFlags & BTNFLAG_HP) && uiNewButtonFlags & BTNFLAG_HP)
		{
			m_ButtonHP.SetTint(HyColor::White);
			m_ButtonHP.scale.Tween(fButtonPressScale, fButtonPressScale, 0.1f, HyTween::QuadOut);
			m_Dpad.SetButtonPress(BTNSTATE_HP, true);
		}
		else if(m_uiButtonFlags & BTNFLAG_HP && !(uiNewButtonFlags & BTNFLAG_HP))
		{
			m_ButtonHP.SetTint(HyColor(128, 128, 128));
			m_ButtonHP.scale.Tween(1.0f, 1.0f, 0.1f, HyTween::QuadOut);
			m_Dpad.SetButtonPress(BTNSTATE_HP, false);
		}

		if(!(m_uiButtonFlags & BTNFLAG_LK) && uiNewButtonFlags & BTNFLAG_LK)
		{
			m_ButtonLK.SetTint(HyColor::White);
			m_ButtonLK.scale.Tween(fButtonPressScale, fButtonPressScale, 0.1f, HyTween::QuadOut);
			m_Dpad.SetButtonPress(BTNSTATE_LK, true);
		}
		else if(m_uiButtonFlags & BTNFLAG_LK && !(uiNewButtonFlags & BTNFLAG_LK))
		{
			m_ButtonLK.SetTint(HyColor(128, 128, 128));
			m_ButtonLK.scale.Tween(1.0f, 1.0f, 0.1f, HyTween::QuadOut);
			m_Dpad.SetButtonPress(BTNSTATE_LK, false);
		}

		if(!(m_uiButtonFlags & BTNFLAG_MK) && uiNewButtonFlags & BTNFLAG_MK)
		{
			m_ButtonMK.SetTint(HyColor::White);
			m_ButtonMK.scale.Tween(fButtonPressScale, fButtonPressScale, 0.1f, HyTween::QuadOut);
			m_Dpad.SetButtonPress(BTNSTATE_MK, true);
		}
		else if(m_uiButtonFlags & BTNFLAG_MK && !(uiNewButtonFlags & BTNFLAG_MK))
		{
			m_ButtonMK.SetTint(HyColor(128, 128, 128));
			m_ButtonMK.scale.Tween(1.0f, 1.0f, 0.1f, HyTween::QuadOut);
			m_Dpad.SetButtonPress(BTNSTATE_MK, false);
		}

		if(!(m_uiButtonFlags & BTNFLAG_HK) && uiNewButtonFlags & BTNFLAG_HK)
		{
			m_ButtonHK.SetTint(HyColor::White);
			m_ButtonHK.scale.Tween(fButtonPressScale, fButtonPressScale, 0.1f, HyTween::QuadOut);
			m_Dpad.SetButtonPress(BTNSTATE_HK, true);
		}
		else if(m_uiButtonFlags & BTNFLAG_HK && !(uiNewButtonFlags & BTNFLAG_HK))
		{
			m_ButtonHK.SetTint(HyColor(128, 128, 128));
			m_ButtonHK.scale.Tween(1.0f, 1.0f, 0.1f, HyTween::QuadOut);
			m_Dpad.SetButtonPress(BTNSTATE_HK, false);
		}

		m_uiButtonFlags = uiNewButtonFlags;
	}
}

/*virtual*/ void StreetFighter::OnMouseClicked() /*override*/
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
