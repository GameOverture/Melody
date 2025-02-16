#include "pch.h"
#include "Melody.h"

/*static*/ Melody *Melody::sm_pThis = nullptr;

#define GAMEBROWSE_MAX_SIZE 0.95f

Melody::Melody(HarmonyInit &initStruct) :
	HyEngine(initStruct),
	m_pCamera(HyEngine::Window().CreateCamera2d()),
	m_pCameraCtrlPanel(HyEngine::Window(1).CreateCamera2d()),
	m_Compositorium("E:\\Soft\\Game_Overture\\RetroCompositorium\\data"),// "\\\\IronMountain/Documents/RetroCompositorium/"),
	m_ColorKeyBg(),
	m_CtrlPanel(),
	m_VgMusic(),
	m_GameBrowser(),
	m_Monitor(),
	m_LiveSplit(m_Monitor),
	m_MessageCycle(m_Monitor),
	m_InputViewer(),
	m_Crt(m_VgMusic, m_MessageCycle, m_InputViewer),
	m_NowPlaying(),
	m_HeartBeat(),
	m_PresetStartingBtn(HyPanelInit(64, 32, 2), HyNodePath("", "CtrlPanel")),
	m_PresetLiveBtn(HyPanelInit(64, 32, 2), HyNodePath("", "CtrlPanel")),
	m_PresetBrbOnBtn(HyPanelInit(64, 32, 2), HyNodePath("", "CtrlPanel")),
	m_PresetBrbOffBtn(HyPanelInit(64, 32, 2), HyNodePath("", "CtrlPanel")),
	m_PresetEndingBtn(HyPanelInit(64, 32, 2), HyNodePath("", "CtrlPanel"))
{
	sm_pThis = this;

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

	m_CtrlPanel.UseWindowCoordinates(1);

	m_GameBrowser.Load();
	m_GameBrowser.SetVisible(false);
	m_GameBrowser.SetDisplayOrder(DISPLAYORDER_GameBrowser);
	m_GameBrowser.scale.Set(GAMEBROWSE_MAX_SIZE, GAMEBROWSE_MAX_SIZE);
	m_GameBrowser.PopulateCtrlPanel(m_CtrlPanel);
	m_CtrlPanel.InsertDividerLine();

	m_VgMusic.Load();
	m_VgMusic.SetCrtRef(&m_Crt);
	m_VgMusic.PopulateCtrlPanel(m_CtrlPanel);
	m_CtrlPanel.InsertDividerLine();

	m_Crt.pos.Set(HyEngine::Window(0).GetWidthF(-0.5f), HyEngine::Window(0).GetHeightF(-0.5f));
	m_Crt.Load();
	m_Crt.SetVisible(false);
	m_Crt.PopulateCtrlPanel(m_CtrlPanel);

	m_Monitor.UseWindowCoordinates();
	m_Monitor.SetDisplayOrder(DISPLAYORDER_Monitor);
	m_Monitor.Load();
	m_Monitor.SetVisible(false);
	m_Monitor.pos.Set(-MISC_WIDTH - 100, HyEngine::Window(0).GetHeight() - MISC_HEIGHT);
	m_Monitor.PopulateCtrlPanel(m_CtrlPanel);
	m_CtrlPanel.InsertDividerLine();

	m_Docket.UseWindowCoordinates();
	m_Docket.SetDisplayOrder(DISPLAYORDER_Docket);
	m_Docket.Load();
	m_Docket.SetVisible(false);
	m_Docket.PopulateCtrlPanel(m_CtrlPanel);

	m_NowPlaying.UseWindowCoordinates();
	m_NowPlaying.SetDisplayOrder(DISPLAYORDER_NowPlaying);
	m_NowPlaying.Load();
	m_NowPlaying.SetVisible(false);
	m_NowPlaying.PopulateCtrlPanel(m_CtrlPanel);

	m_LiveSplit.UseWindowCoordinates();
	m_LiveSplit.SetDisplayOrder(DISPLAYORDER_LiveSplitMask);
	m_LiveSplit.Load();
	m_LiveSplit.SetVisible(false);
	m_LiveSplit.PopulateCtrlPanel(m_CtrlPanel);
	m_CtrlPanel.InsertDividerLine();

	m_InputViewer.UseWindowCoordinates();
	m_InputViewer.Load();
	m_InputViewer.SetVisible(false);
	m_InputViewer.PopulateCtrlPanel(m_CtrlPanel);
	m_CtrlPanel.InsertDividerLine();

	m_HeartBeat.UseWindowCoordinates();
	m_HeartBeat.Load();
	m_HeartBeat.SetVisible(false);
	m_HeartBeat.PopulateCtrlPanel(m_CtrlPanel);

	m_MessageCycle.UseWindowCoordinates();
	m_MessageCycle.Load();
	m_MessageCycle.SetDisplayOrder(DISPLAYORDER_MessageCycle);
	m_MessageCycle.PopulateCtrlPanel(m_CtrlPanel);

	// Presets
	m_PresetStartingBtn.SetText("Start");
	m_PresetStartingBtn.SetButtonClickedCallback(
		[this](HyButton *pButton)
		{
			m_Crt.GetCtrlPanelCheckBox().SetChecked(true);
			m_Monitor.GetCtrlPanelCheckBox().SetChecked(false);
			m_NowPlaying.GetCtrlPanelCheckBox().SetChecked(false);
			m_LiveSplit.GetCtrlPanelCheckBox().SetChecked(false);
			m_MessageCycle.GetCtrlPanelCheckBox().SetChecked(true);
			m_MessageCycle.AddMessage("Welcome! Stream Starting Soon", false);
		}
	);
	m_PresetLiveBtn.SetText("Live");
	m_PresetLiveBtn.SetButtonClickedCallback(
		[this](HyButton *pButton)
		{
			m_Crt.GetCtrlPanelCheckBox().SetChecked(true);
			m_Monitor.GetCtrlPanelCheckBox().SetChecked(true);
			m_Monitor.SetChannel(MONITORCHANNEL_ObsFull);
			m_MessageCycle.RemoveMessage("Welcome! Stream Starting Soon");
		}
	);
	m_PresetBrbOnBtn.SetText("BRB On");
	m_PresetBrbOnBtn.SetButtonClickedCallback(
		[this](HyButton *pButton)
		{
			m_Crt.GetCtrlPanelCheckBox().SetChecked(true);
			m_Monitor.GetCtrlPanelCheckBox().SetChecked(true);
			m_Monitor.SetChannel(MONITORCHANNEL_Brb);
			m_NowPlaying.ShowGameTime(false);
			m_MessageCycle.GetCtrlPanelCheckBox().SetChecked(true);
			m_MessageCycle.AddMessage("BRB 5 Minutes!", false);
		}
	);
	m_PresetBrbOffBtn.SetText("BRB Off");
	m_PresetBrbOffBtn.SetButtonClickedCallback(
		[this](HyButton *pButton)
		{
			m_Crt.GetCtrlPanelCheckBox().SetChecked(true);
			m_Monitor.GetCtrlPanelCheckBox().SetChecked(true);
			m_Monitor.SetChannel(MONITORCHANNEL_ObsFull);
			m_MessageCycle.RemoveMessage("BRB 5 Minutes!");
		}
	);
	m_PresetEndingBtn.SetText("End");
	m_PresetEndingBtn.SetButtonClickedCallback(
		[this](HyButton *pButton)
		{
			m_Crt.GetCtrlPanelCheckBox().SetChecked(true);
			m_Monitor.GetCtrlPanelCheckBox().SetChecked(false);
			m_NowPlaying.GetCtrlPanelCheckBox().SetChecked(false);
			m_LiveSplit.GetCtrlPanelCheckBox().SetChecked(false);
			m_MessageCycle.ClearMessages();
			m_MessageCycle.GetCtrlPanelCheckBox().SetChecked(true);
			m_MessageCycle.AddMessage("Stream Ending! Thanks for watching", true);
		}
	);

	HyLayoutHandle hRow = m_CtrlPanel.InsertLayout(HYORIENT_Horizontal);
	m_CtrlPanel.InsertWidget(m_PresetBrbOnBtn, hRow);
	m_CtrlPanel.InsertWidget(m_PresetBrbOffBtn, hRow);
	m_CtrlPanel.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);
	HyLayoutHandle hRow2 = m_CtrlPanel.InsertLayout(HYORIENT_Horizontal);
	m_CtrlPanel.InsertWidget(m_PresetStartingBtn, hRow2);
	m_CtrlPanel.InsertWidget(m_PresetLiveBtn, hRow2);
	m_CtrlPanel.InsertWidget(m_PresetEndingBtn, hRow2);
	m_CtrlPanel.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow2);

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
	if(m_GameBrowser.IsShowing())
	{
		const float fScaleDuration = 15.0f;
		if(m_GameBrowser.scale.IsAnimating() == false)
		{
			if(m_GameBrowser.scale.X() >= GAMEBROWSE_MAX_SIZE)
				m_GameBrowser.scale.Tween(GAMEBROWSE_MAX_SIZE - 0.02f, GAMEBROWSE_MAX_SIZE - 0.02f, fScaleDuration, HyTween::QuadInOut);
			else
				m_GameBrowser.scale.Tween(GAMEBROWSE_MAX_SIZE, GAMEBROWSE_MAX_SIZE, fScaleDuration, HyTween::QuadInOut);
		}

		const float fPosDuration = 12.0f;
		if(m_GameBrowser.pos.IsAnimating() == false)
		{
			if(m_GameBrowser.pos.Y() >= 0.0f)
				m_GameBrowser.pos.Tween(0.0f, -5.0f, fPosDuration, HyTween::QuadInOut);
			else
				m_GameBrowser.pos.Tween(0.0f, 5.0f, fPosDuration, HyTween::QuadInOut);
		}

		const float fRotDuration = 30.0f;
		if(m_GameBrowser.rot.IsAnimating() == false)
		{
			if(m_GameBrowser.rot.Get() >= 0.0f)
				m_GameBrowser.rot.Tween(-1.0f, fRotDuration);
			else
				m_GameBrowser.rot.Tween(1.0f, fRotDuration);
		}
	}

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

/*static*/ void Melody::RefreshCamera()
{
	if(sm_pThis->m_Crt.GetChannel() != CHANNELTYPE_Game)
	{
		if(sm_pThis->m_Monitor.IsVisible() || sm_pThis->m_LiveSplit.IsVisible() || sm_pThis->m_NowPlaying.IsVisible())
		{
			HyEngine::Window().GetCamera2d(0)->pos.Tween(CAMERA_DIVIDER_POS, 1.5f, HyTween::QuadInOut);
			HyEngine::Window().GetCamera2d(0)->scale.Tween(CAMERA_DIVIDER_SCALE, 1.5f, HyTween::QuadInOut);
			HyEngine::Window().GetCamera2d(0)->SetTag(CAMTAG_Divider);

			sm_pThis->m_MessageCycle.SetXPosOffset(MESSAGECYCLE_POS_X);
		}
		else
		{
			HyEngine::Window().GetCamera2d(0)->pos.Tween(CAMERA_CENTER_POS, 1.5f, HyTween::QuadInOut);
			HyEngine::Window().GetCamera2d(0)->scale.Tween(CAMERA_CENTER_SCALE, 1.5f, HyTween::QuadInOut);
			HyEngine::Window().GetCamera2d(0)->SetTag(CAMTAG_Center);

			sm_pThis->m_MessageCycle.SetXPosOffset(0.0f);
		}
	}
}

void TransformTexture(HyTexturedQuad2d &quadRef, glm::ivec2 vMaxSize, glm::vec2 ptCenter)
{
	// Scale the texture to fit within the max width and height
	quadRef.scale.SetAll(1.0f);
	//if(quadRef.GetWidth() > vMaxSize.x || quadRef.GetHeight() > vMaxSize.y)
	{
		float fScale = std::min(vMaxSize.x / quadRef.GetWidth(), vMaxSize.y / quadRef.GetHeight());
		quadRef.scale.SetAll(fScale);
	}

	// Find center of desired position, then offset by half the width and height of the texture
	quadRef.pos.Set(ptCenter);
	quadRef.pos.Offset(quadRef.GetWidth(quadRef.scale.GetX()) * -0.5f, quadRef.GetHeight(quadRef.scale.GetY()) * -0.5f);
}
