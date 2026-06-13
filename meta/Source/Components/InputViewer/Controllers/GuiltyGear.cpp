#include "pch.h"
#include "GuiltyGear.h"

GuiltyGear::GuiltyGear(HyEntity2d *pParent /*= nullptr*/) :
	IController(pParent),
	m_Dpad(HyNodePath("InputViewer/StreetFighter/DpadGate"), HyNodePath("InputViewer/StreetFighter/DpadBall"), HyNodePath("InputViewer/GuiltyGear/Buttons"), 85.0f, 75.0f, HyColor::Red, 16.0f, this),
	m_ButtonP("InputViewer/GuiltyGear/Buttons", this),
	m_ButtonK("InputViewer/GuiltyGear/Buttons", this),
	m_ButtonS("InputViewer/GuiltyGear/Buttons", this),
	m_ButtonHS("InputViewer/GuiltyGear/Buttons", this),
	m_ButtonDU("InputViewer/GuiltyGear/Buttons", this),
	m_ButtonDA("InputViewer/GuiltyGear/Buttons", this),
	//m_ButtonFD("InputViewer/GuiltyGear/Buttons", this),
	m_uiButtonFlags(0)
{
	m_ButtonP.pos.Set(142.0f, -50.0f);
	m_ButtonP.SetState(BTNSTATE_P);
	m_ButtonP.SetTint(HyColor(128, 128, 128));

	m_ButtonK.pos.Set(142.0f, 50.0f);
	m_ButtonK.SetState(BTNSTATE_K);
	m_ButtonK.SetTint(HyColor(128, 128, 128));
	
	m_ButtonS.pos.Set(242.0f, 50.0f);
	m_ButtonS.SetState(BTNSTATE_S);
	m_ButtonS.SetTint(HyColor(128, 128, 128));

	m_ButtonHS.pos.Set(342.0f, 50.0f);
	m_ButtonHS.SetState(BTNSTATE_HS);
	m_ButtonHS.SetTint(HyColor(128, 128, 128));

	m_ButtonDU.pos.Set(422.0f, 0.0f);
	m_ButtonDU.SetState(BTNSTATE_DU);
	m_ButtonDU.SetTint(HyColor(128, 128, 128));

	m_ButtonDA.pos.Set(242.0f, -50.0f);
	m_ButtonDA.SetState(BTNSTATE_DA);
	m_ButtonDA.SetTint(HyColor(128, 128, 128));

	//m_ButtonFD.pos.Set(342.0f, -50.0f);
	//m_ButtonFD.SetState(BTNSTATE_FD);
	//m_ButtonFD.SetTint(HyColor(128, 128, 128));

	b2AABB sceneAabb = GetSceneAABB();
	m_AssignOverlayBG.SetAsBox(0, sceneAabb.upperBound.x - sceneAabb.lowerBound.x, sceneAabb.upperBound.y - sceneAabb.lowerBound.y, 0.0f);
	m_AssignOverlayBG.pos.Offset(-105.0f, -120.0f);
	m_AssignOverlayBG.SetVisible(false);
	m_AssignOverlayText.SetVisible(false);
	m_AssignOverlayText.SetAlignment(HYALIGN_Center);
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

/*virtual*/ GuiltyGear::~GuiltyGear()
{
}

/*virtual*/ void GuiltyGear::ApplyInputs() /*override*/
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

	uint32 uiNewButtonFlags = 0;
	if(HyEngine::Input().IsActionDown(FIGHTSTICK_LK))
		uiNewButtonFlags |= BTNFLAG_P;

	if(HyEngine::Input().IsActionDown(FIGHTSTICK_LP))
		uiNewButtonFlags |= BTNFLAG_K;

	if(HyEngine::Input().IsActionDown(FIGHTSTICK_MP))
		uiNewButtonFlags |= BTNFLAG_S;

	if(HyEngine::Input().IsActionDown(FIGHTSTICK_HP))
		uiNewButtonFlags |= BTNFLAG_HS;

	if(HyEngine::Input().IsActionDown(FIGHTSTICK_DriveImpact))
		uiNewButtonFlags |= BTNFLAG_DU;

	if(HyEngine::Input().IsActionDown(FIGHTSTICK_MK))
		uiNewButtonFlags |= BTNFLAG_DA;

	//if(HyEngine::Input().IsActionDown(FIGHTSTICK_Parry))
	//	uiNewButtonFlags |= BTNFLAG_FD;

	if(m_uiButtonFlags != uiNewButtonFlags)
	{
		const float fButtonPressScale = 1.15f;
		if(!(m_uiButtonFlags & BTNFLAG_P) && uiNewButtonFlags & BTNFLAG_P)
		{
			m_ButtonP.SetTint(HyColor::White);
			m_ButtonP.scale.Tween(fButtonPressScale, fButtonPressScale, 0.1f, HyTween::QuadOut);
			m_Dpad.SetButtonPress(BTNSTATE_P, true);
		}
		else if(m_uiButtonFlags & BTNFLAG_P && !(uiNewButtonFlags & BTNFLAG_P))
		{
			m_ButtonP.SetTint(HyColor(128, 128, 128));
			m_ButtonP.scale.Tween(1.0f, 1.0f, 0.1f, HyTween::QuadOut);
			m_Dpad.SetButtonPress(BTNSTATE_P, false);
		}

		if(!(m_uiButtonFlags & BTNFLAG_K) && uiNewButtonFlags & BTNFLAG_K)
		{
			m_ButtonK.SetTint(HyColor::White);
			m_ButtonK.scale.Tween(fButtonPressScale, fButtonPressScale, 0.1f, HyTween::QuadOut);
			m_Dpad.SetButtonPress(BTNSTATE_K, true);
		}
		else if(m_uiButtonFlags & BTNFLAG_K && !(uiNewButtonFlags & BTNFLAG_K))
		{
			m_ButtonK.SetTint(HyColor(128, 128, 128));
			m_ButtonK.scale.Tween(1.0f, 1.0f, 0.1f, HyTween::QuadOut);
			m_Dpad.SetButtonPress(BTNSTATE_K, false);
		}

		if(!(m_uiButtonFlags & BTNFLAG_S) && uiNewButtonFlags & BTNFLAG_S)
		{
			m_ButtonS.SetTint(HyColor::White);
			m_ButtonS.scale.Tween(fButtonPressScale, fButtonPressScale, 0.1f, HyTween::QuadOut);
			m_Dpad.SetButtonPress(BTNSTATE_S, true);
		}
		else if(m_uiButtonFlags & BTNFLAG_S && !(uiNewButtonFlags & BTNFLAG_S))
		{
			m_ButtonS.SetTint(HyColor(128, 128, 128));
			m_ButtonS.scale.Tween(1.0f, 1.0f, 0.1f, HyTween::QuadOut);
			m_Dpad.SetButtonPress(BTNSTATE_S, false);
		}

		if(!(m_uiButtonFlags & BTNFLAG_HS) && uiNewButtonFlags & BTNFLAG_HS)
		{
			m_ButtonHS.SetTint(HyColor::White);
			m_ButtonHS.scale.Tween(fButtonPressScale, fButtonPressScale, 0.1f, HyTween::QuadOut);
			m_Dpad.SetButtonPress(BTNSTATE_HS, true);
		}
		else if(m_uiButtonFlags & BTNFLAG_HS && !(uiNewButtonFlags & BTNFLAG_HS))
		{
			m_ButtonHS.SetTint(HyColor(128, 128, 128));
			m_ButtonHS.scale.Tween(1.0f, 1.0f, 0.1f, HyTween::QuadOut);
			m_Dpad.SetButtonPress(BTNSTATE_HS, false);
		}

		if(!(m_uiButtonFlags & BTNFLAG_DU) && uiNewButtonFlags & BTNFLAG_DU)
		{
			m_ButtonDU.SetTint(HyColor::White);
			m_ButtonDU.scale.Tween(fButtonPressScale, fButtonPressScale, 0.1f, HyTween::QuadOut);
			m_Dpad.SetButtonPress(BTNSTATE_DU, true);
		}
		else if(m_uiButtonFlags & BTNFLAG_DU && !(uiNewButtonFlags & BTNFLAG_DU))
		{
			m_ButtonDU.SetTint(HyColor(128, 128, 128));
			m_ButtonDU.scale.Tween(1.0f, 1.0f, 0.1f, HyTween::QuadOut);
			m_Dpad.SetButtonPress(BTNSTATE_DU, false);
		}

		if(!(m_uiButtonFlags & BTNFLAG_DA) && uiNewButtonFlags & BTNFLAG_DA)
		{
			m_ButtonDA.SetTint(HyColor::White);
			m_ButtonDA.scale.Tween(fButtonPressScale, fButtonPressScale, 0.1f, HyTween::QuadOut);
			m_Dpad.SetButtonPress(BTNSTATE_DA, true);
		}
		else if(m_uiButtonFlags & BTNFLAG_DA && !(uiNewButtonFlags & BTNFLAG_DA))
		{
			m_ButtonDA.SetTint(HyColor(128, 128, 128));
			m_ButtonDA.scale.Tween(1.0f, 1.0f, 0.1f, HyTween::QuadOut);
			m_Dpad.SetButtonPress(BTNSTATE_DA, false);
		}

		//if(!(m_uiButtonFlags & BTNFLAG_FD) && uiNewButtonFlags & BTNFLAG_FD)
		//{
		//	m_ButtonFD.SetTint(HyColor::White);
		//	m_ButtonFD.scale.Tween(fButtonPressScale, fButtonPressScale, 0.1f, HyTween::QuadOut);
		//	m_Dpad.SetButtonPress(BTNSTATE_FD, true);
		//}
		//else if(m_uiButtonFlags & BTNFLAG_FD && !(uiNewButtonFlags & BTNFLAG_FD))
		//{
		//	m_ButtonFD.SetTint(HyColor(128, 128, 128));
		//	m_ButtonFD.scale.Tween(1.0f, 1.0f, 0.1f, HyTween::QuadOut);
		//	m_Dpad.SetButtonPress(BTNSTATE_FD, false);
		//}

		m_uiButtonFlags = uiNewButtonFlags;
	}
}

/*virtual*/ void GuiltyGear::OnMouseClicked() /*override*/
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