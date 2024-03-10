#include "pch.h"
#include "Melody.h"

Melody::Melody(HarmonyInit &initStruct) :
	HyEngine(initStruct),
	m_pCamera(HyEngine::Window().CreateCamera2d()),
	m_pCameraCtrlPanel(HyEngine::Window(1).CreateCamera2d()),
	m_ColorKeyBg(),
	m_InputViewer(),
	m_VgMusic(),
	m_Brb(),
	m_MessageCycle(m_Brb),
	m_Crt(m_VgMusic, m_MessageCycle),
	m_CtrlPanel(m_Crt)
{
	//HyEngine::Input().MapBtn(INPUT_ExitGame, HYKEY_Escape);
	//
	//HyEngine::Input().MapBtn(INPUT_GotoIntro, HYKEY_1);
	//HyEngine::Input().MapBtn(INPUT_GotoLowerThird, HYKEY_2);
	//HyEngine::Input().MapBtn(INPUT_GotoBreak, HYKEY_3);
	//HyEngine::Input().MapBtn(INPUT_GotoWheel, HYKEY_4);
	//HyEngine::Input().MapBtn(INPUT_GotoOutro, HYKEY_6);
	//HyEngine::Input().MapBtn(INPUT_CrtPowerToggle, HYKEY_P);
	//HyEngine::Input().MapBtn(INPUT_CrtChannelUp, HYKEY_W);
	//HyEngine::Input().MapBtn(INPUT_CrtChannelDown, HYKEY_S);

	//HyEngine::Input().MapBtn(INPUT_Dance, HYKEY_Z);
	//HyEngine::Input().MapBtn(INPUT_DanceAlt, HYKEY_X);
	//HyEngine::Input().MapBtn(INPUT_DanceStop, HYKEY_C);
	//HyEngine::Input().MapBtn(INPUT_DanceShimmy, HYKEY_V);

	//HyEngine::Input().MapBtn(INPUT_VgMusicPlay, HYKEY_F);
	//HyEngine::Input().MapBtn(INPUT_VgMusicStop, HYKEY_G);
	//HyEngine::Input().MapBtn(INPUT_GlobalVolumeDown, HYKEY_Minus);
	//HyEngine::Input().MapBtn(INPUT_GlobalVolumeUp, HYKEY_Equal);

	HyEngine::Input().MapGamePadBtn(FIGHTSTICK_LK, HYPAD_A);
	HyEngine::Input().MapGamePadBtn(FIGHTSTICK_MK, HYPAD_B);
	//FIGHTSTICK_HK, // On Axis 'HYPADAXIS_TriggerRight'
	HyEngine::Input().MapGamePadBtn(FIGHTSTICK_LP, HYPAD_X);
	HyEngine::Input().MapGamePadBtn(FIGHTSTICK_MP, HYPAD_Y);
	HyEngine::Input().MapGamePadBtn(FIGHTSTICK_HP, HYPAD_RightBumper);
	HyEngine::Input().MapGamePadBtn(FIGHTSTICK_Start, HYPAD_Start);
	HyEngine::Input().MapGamePadBtn(FIGHTSTICK_Select, HYPAD_Back);
	HyEngine::Input().MapGamePadBtn(FIGHTSTICK_Up, HYPAD_DpadUp);
	HyEngine::Input().MapGamePadBtn(FIGHTSTICK_Down, HYPAD_DpadDown);
	HyEngine::Input().MapGamePadBtn(FIGHTSTICK_Left, HYPAD_DpadLeft);
	HyEngine::Input().MapGamePadBtn(FIGHTSTICK_Right, HYPAD_DpadRight);
	HyEngine::Input().MapGamePadBtn(FIGHTSTICK_DriveImpact, HYPAD_LeftBumper);
	HyEngine::Input().MapGamePadBtn(FIGHTSTICK_Parry, HYPAD_LeftThumbStick);

	HyEngine::Input().SetControllerBackgroundInput(true);

	m_ColorKeyBg.UseWindowCoordinates();
	m_ColorKeyBg.SetDisplayOrder(-999999);
	m_ColorKeyBg.SetTint(HyColor::Orange/*HyColor::Magenta*/);
	m_ColorKeyBg.SetAsBox(1920.0f, 1080.0f);
	m_ColorKeyBg.pos.Set(0.0f, 0.0f);
	//m_ColorKeyBg.SetAsBox(312.0f, 139.0f);	// Fight stick camera
	//m_ColorKeyBg.pos.Set(804.0f, 0.0f);		// Fight stick camera

	m_RetroLeftSide.UseWindowCoordinates();
	//m_RetroLeftSide.SetWireframe(true);
	//m_RetroLeftSide.SetAsBox(548.0f, 310.0f);
	//m_RetroLeftSide.pos.Set(0.0f, HyEngine::Window(0).GetHeightF() - 310.0f);
	m_RetroLeftSide.SetAsBox(548.0f, HyEngine::Window(0).GetHeightF());
	m_RetroLeftSide.pos.Set(0.0f, 0.0f);
	m_RetroLeftSide.SetVisible(false);

	m_RetroLeftSideStencil.AddMask(m_RetroLeftSide);
	m_RetroLeftSideStencil.SetAsInvertedMask();

	//m_RetroCaptureArea.UseWindowCoordinates();
	//m_RetroCaptureArea.SetAsBox(1144.0f, 898.0f);
	//m_RetroCaptureArea.SetWireframe(true);
	//m_RetroCaptureArea.pos.Set(544.0f + 116.0f, 150.0f);
	//m_RetroCaptureArea.SetDisplayOrder(DISPLAYORDER_DEBUG);

	m_CtrlPanel.UseWindowCoordinates(1);

	m_VgMusic.Load();
	m_VgMusic.PopulateCtrlPanel(m_CtrlPanel);

	m_Brb.UseWindowCoordinates();
	m_Brb.Load();
	m_Brb.SetVisible(false);
	m_Brb.scale.Set(0.5f, 0.5f);
	m_Brb.pos.Set(-500.0f, HyEngine::Window(0).GetHeightF() - 264.0f);
	m_Brb.PopulateCtrlPanel(m_CtrlPanel);

	m_Crt.UseWindowCoordinates();
	m_Crt.Load();
	m_Crt.SetVisible(false);
	m_Crt.SetStencil(&m_RetroLeftSideStencil);
	m_Crt.PopulateCtrlPanel(m_CtrlPanel);

	m_InputViewer.UseWindowCoordinates();
	m_InputViewer.pos.Set(1200.0f, 80.0f);
	m_InputViewer.scale.Set(0.75f, 0.75f);
	m_InputViewer.Load();
	m_InputViewer.SetVisible(false);
	m_InputViewer.PopulateCtrlPanel(m_CtrlPanel);

	m_HeartBeat.UseWindowCoordinates();
	m_HeartBeat.Load();
	m_HeartBeat.SetVisible(false);
	m_HeartBeat.PopulateCtrlPanel(m_CtrlPanel);

	//m_NESController.UseWindowCoordinates();
	//m_NESController.Load();
	//m_NESController.SetVisible(false);
	//m_NESController.PopulateCtrlPanel(m_CtrlPanel);

	m_MessageCycle.UseWindowCoordinates();
	m_MessageCycle.Load();
	m_MessageCycle.SetDisplayOrder(DISPLAYORDER_MessageCycle);
	m_MessageCycle.PopulateCtrlPanel(m_CtrlPanel);

	m_CtrlPanel.Load();
}

Melody::~Melody()
{
}

/*virtual*/ bool Melody::OnUpdate() /*override*/
{
	//if(HyEngine::Input().IsActionReleased(INPUT_GlobalVolumeDown))
	//{
	//	HyEngine::Audio().SetGlobalVolume(HyMath::Clamp(HyEngine::Audio().GetGlobalVolume() - 0.05f, 0.0f, 1.0f));
	//	m_Crt.SetVolume(HyEngine::Audio().GetGlobalVolume());
	//}
	//if(HyEngine::Input().IsActionReleased(INPUT_GlobalVolumeUp))
	//{
	//	HyEngine::Audio().SetGlobalVolume(HyMath::Clamp(HyEngine::Audio().GetGlobalVolume() + 0.05f, 0.0f, 1.0f));
	//	m_Crt.SetVolume(HyEngine::Audio().GetGlobalVolume());
	//}

	//return !HyEngine::Input().IsActionReleased(INPUT_ExitGame);

	return true;
}
