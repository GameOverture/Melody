#include "pch.h"
#include "Melody.h"

Melody::Melody(HarmonyInit &initStruct) :
	HyEngine(initStruct),
	m_pCamera(HyEngine::Window().CreateCamera2d()),
	m_pCameraCtrlPanel(HyEngine::Window(1).CreateCamera2d()),
	m_ColorKeyBg(),
	m_FightStick(),
	m_VgMusic(),
	m_Crt(m_VgMusic),
	m_CtrlPanel()
{
	HyEngine::Input().MapBtn(INPUT_ExitGame, HYKEY_Escape);
	
	HyEngine::Input().MapBtn(INPUT_GotoIntro, HYKEY_1);
	HyEngine::Input().MapBtn(INPUT_GotoLowerThird, HYKEY_2);
	HyEngine::Input().MapBtn(INPUT_GotoBreak, HYKEY_3);
	HyEngine::Input().MapBtn(INPUT_GotoWheel, HYKEY_4);
	HyEngine::Input().MapBtn(INPUT_GotoOutro, HYKEY_6);
	HyEngine::Input().MapBtn(INPUT_CrtPowerToggle, HYKEY_P);
	HyEngine::Input().MapBtn(INPUT_CrtChannelUp, HYKEY_W);
	HyEngine::Input().MapBtn(INPUT_CrtChannelDown, HYKEY_S);

	HyEngine::Input().MapBtn(INPUT_Dance, HYKEY_Z);
	HyEngine::Input().MapBtn(INPUT_DanceAlt, HYKEY_X);
	HyEngine::Input().MapBtn(INPUT_DanceStop, HYKEY_C);
	HyEngine::Input().MapBtn(INPUT_DanceShimmy, HYKEY_V);

	HyEngine::Input().MapBtn(INPUT_VgMusicPlay, HYKEY_F);
	HyEngine::Input().MapBtn(INPUT_VgMusicStop, HYKEY_G);
	HyEngine::Input().MapBtn(INPUT_VgMusicVolumeDown, HYKEY_Minus);
	HyEngine::Input().MapBtn(INPUT_VgMusicVolumeUp, HYKEY_Equal);

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
	m_ColorKeyBg.SetTint(HyColor::Black);//HyColor::Magenta);
	m_ColorKeyBg.SetAsBox(1920.0f, 1080.0f);
	m_ColorKeyBg.pos.Set(0.0f, 0.0f);
	//m_ColorKeyBg.SetAsBox(312.0f, 139.0f);	// Fight stick camera
	//m_ColorKeyBg.pos.Set(804.0f, 0.0f);		// Fight stick camera

	m_FightStick.UseWindowCoordinates();
	m_FightStick.pos.Set(1200.0f, 80.0f);
	m_FightStick.scale.Set(0.75f, 0.75f);
	m_FightStick.Load();
	m_FightStick.SetVisible(false);

	m_VgMusic.UseWindowCoordinates();
	m_VgMusic.Load();

	m_Crt.UseWindowCoordinates();
	m_Crt.Load();
	m_Crt.SetVisible(false);

	m_CtrlPanel.UseWindowCoordinates(1);
	m_CtrlPanel.AddComponent(m_FightStick);
	m_CtrlPanel.AddComponent(m_Crt);
	//m_CtrlPanel.AddComponent(m_VgMusic.GetLargePlayer());
	m_CtrlPanel.FinishComponents();
	m_CtrlPanel.Load();
}

Melody::~Melody()
{
}

/*virtual*/ bool Melody::OnUpdate() /*override*/
{
	return !HyEngine::Input().IsActionReleased(INPUT_ExitGame);
}
