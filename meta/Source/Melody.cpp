#include "pch.h"
#include "Melody.h"

Melody::Melody(HarmonyInit &initStruct) :
	HyEngine(initStruct),
	m_pCamera(HyEngine::Window().CreateCamera2d()),
	m_pCameraCtrlPanel(HyEngine::Window(1).CreateCamera2d()),
	m_ColorKeyBg(),
	m_VgMusic(),
	m_Monitor(),
	m_MessageCycle(m_Monitor),
	m_InputViewer(),
	m_Crt(m_VgMusic, m_Monitor, m_MessageCycle, m_InputViewer),
	m_CtrlPanel(m_Crt)
{
	m_pCameraCtrlPanel->pos.Set(0.0f, 2000.0f);

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
	m_ColorKeyBg.SetTint(HyColor::Orange);
	m_ColorKeyBg.SetAsBox(1920.0f, 1080.0f);
	m_ColorKeyBg.pos.Set(0.0f, 0.0f);
	//m_ColorKeyBg.SetAsBox(312.0f, 139.0f);	// Fight stick camera
	//m_ColorKeyBg.pos.Set(804.0f, 0.0f);		// Fight stick camera

	//m_LiveSplitMaskStroke.UseWindowCoordinates();
	//m_LiveSplitMaskStroke.SetWireframe(true);
	//m_LiveSplitMaskStroke.SetAsBox(MISC_WIDTH + DIVIDER_WIDTH, HyEngine::Window(0).GetHeight());
	//m_LiveSplitMaskStroke.pos.Set(-(MISC_WIDTH + DIVIDER_WIDTH), 0);
	//m_LiveSplitMaskStroke.SetTint(HyColor::Black);
	//m_LiveSplitMaskStroke.SetDisplayOrder(DISPLAYORDER_LiveSplitMask + 1);
	//m_LiveSplitMaskStroke.SetVisible(false);

	//m_LiveSplitStencil.AddMask(m_LiveSplitMask);
	//m_LiveSplitStencil.SetAsInvertedMask();

	m_CtrlPanel.UseWindowCoordinates(1);

	m_VgMusic.Load();
	m_VgMusic.PopulateCtrlPanel(m_CtrlPanel);

	m_Crt.pos.Set(HyEngine::Window(0).GetWidthF(-0.5f), HyEngine::Window(0).GetHeightF(-0.5f));
	m_Crt.Load();
	m_Crt.SetVisible(false);
	m_Crt.PopulateCtrlPanel(m_CtrlPanel);

	m_Monitor.UseWindowCoordinates();
	m_Monitor.SetDisplayOrder(DISPLAYORDER_MessageCycle);
	m_Monitor.Load();
	m_Monitor.SetVisible(false);
	m_Monitor.pos.Set(-MISC_WIDTH - 100, HyEngine::Window(0).GetHeight() - MISC_HEIGHT);
	m_Monitor.PopulateCtrlPanel(m_CtrlPanel);

	m_InputViewer.UseWindowCoordinates();
	m_InputViewer.Load();
	m_InputViewer.SetVisible(false);
	m_InputViewer.PopulateCtrlPanel(m_CtrlPanel);

	m_HeartBeat.UseWindowCoordinates();
	m_HeartBeat.Load();
	m_HeartBeat.SetVisible(false);
	m_HeartBeat.PopulateCtrlPanel(m_CtrlPanel);

	m_MessageCycle.UseWindowCoordinates();
	m_MessageCycle.Load();
	m_MessageCycle.SetDisplayOrder(DISPLAYORDER_MessageCycle);
	m_MessageCycle.PopulateCtrlPanel(m_CtrlPanel);

	m_CtrlPanel.Load();

	//m_DebugRetroCaptureArea.UseWindowCoordinates();
	//m_DebugRetroCaptureArea.SetAsBox(1280.0f, 900.0f);
	//m_DebugRetroCaptureArea.SetWireframe(true);
	//m_DebugRetroCaptureArea.pos.Set(LIVESPLIT_WIDTH + DIVIDER_WIDTH + 48, LOWERTHIRD_HEIGHT);
	//m_DebugRetroCaptureArea.SetDisplayOrder(DISPLAYORDER_DEBUG);
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
