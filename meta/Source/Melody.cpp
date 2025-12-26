#include "pch.h"
#include "Melody.h"

#include "InputViewer.h"
#include "VgMusic.h"
#include "GameBrowser.h"
#include "Monitor.h"
#include "LiveSplit.h"
#include "Wheel.h"
#include "Crt.h"
#include "NowPlaying.h"
#include "Docket.h"
#include "HeartBeat.h"
#include "MessageCycle.h"
#include "Code.h"

/*static*/ Melody *Melody::sm_pThis = nullptr;

Melody::Melody(HyInit &initStruct) :
	HyEngine(initStruct),
	m_pCamera(HyEngine::Window().CreateCamera2d()),
	m_pCameraCtrlPanel(HyEngine::Window(1).CreateCamera2d()),
	m_Compositorium("C:\\Soft\\Game_Overture\\RetroCompositorium"),// "\\\\IronMountain/Documents/RetroCompositorium/"),
	m_ColorKeyBg(),
	m_CtrlPanel(),
	m_PresetStartingBtn(HyUiPanelInit(64, 32, 2), HyUiTextInit(HyNodePath("", "CtrlPanel"))),
	m_PresetLiveBtn(HyUiPanelInit(64, 32, 2), HyUiTextInit(HyNodePath("", "CtrlPanel"))),
	m_PresetBrb1Btn(HyUiPanelInit(64, 32, 2), HyUiTextInit(HyNodePath("", "CtrlPanel"))),
	m_PresetBrb5Btn(HyUiPanelInit(64, 32, 2), HyUiTextInit(HyNodePath("", "CtrlPanel"))),
	m_PresetBrb10Btn(HyUiPanelInit(64, 32, 2), HyUiTextInit(HyNodePath("", "CtrlPanel"))),
	m_PresetBrbOffBtn(HyUiPanelInit(64, 32, 2), HyUiTextInit(HyNodePath("", "CtrlPanel"))),
	m_PresetEndingBtn(HyUiPanelInit(64, 32, 2), HyUiTextInit(HyNodePath("", "CtrlPanel")))
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

	for(int i = 0; i < NUM_COMPONENTS; ++i)
		m_pComponents[i] = nullptr;

	for(int i = 0; i < NUM_COMPONENTS; ++i)
	{
		switch(i)
		{
		case COMPONENT_Crt:				m_pComponents[i] = HY_NEW Crt(); break;
		case COMPONENT_Code:			m_pComponents[i] = HY_NEW Code(); break;
		case COMPONENT_VgMusic:			m_pComponents[i] = HY_NEW VgMusic(); break;
		case COMPONENT_Monitor:			m_pComponents[i] = HY_NEW Monitor(); break;
		case COMPONENT_NowPlaying:		m_pComponents[i] = HY_NEW NowPlaying(); break;
		case COMPONENT_Docket:			m_pComponents[i] = HY_NEW Docket(); break;
		case COMPONENT_LiveSplit:		m_pComponents[i] = HY_NEW LiveSplit(); break;
		case COMPONENT_InputViewer:		m_pComponents[i] = HY_NEW InputViewer(); break;
		case COMPONENT_HeartBeat:		m_pComponents[i] = HY_NEW HeartBeat(); break;
		case COMPONENT_GameBrowser:		m_pComponents[i] = HY_NEW GameBrowser(); break;
		case COMPONENT_Wheel:			m_pComponents[i] = HY_NEW Wheel(WheelInit()); break;
		case COMPONENT_MessageCycle:	m_pComponents[i] = HY_NEW MessageCycle(); break;
		default:
			HyError("Melody::Melody() - Unknown component index:" << i);
			break;
		}
	}

	for(int i = 0; i < NUM_COMPONENTS; ++i)
	{
		m_pComponents[i]->Load();
		m_pComponents[i]->SetVisible(false);
		m_pComponents[i]->PopulateCtrlPanel(m_CtrlPanel);
	}

	// Presets
	m_PresetStartingBtn.SetText("Start");
	m_PresetStartingBtn.SetButtonClickedCallback(
		[this](HyButton *pButton)
		{
			m_pComponents[COMPONENT_Crt]->GetCtrlPanelCheckBox().SetChecked(true);
			m_pComponents[COMPONENT_Monitor]->GetCtrlPanelCheckBox().SetChecked(false);
			m_pComponents[COMPONENT_NowPlaying]->GetCtrlPanelCheckBox().SetChecked(false);
			m_pComponents[COMPONENT_LiveSplit]->GetCtrlPanelCheckBox().SetChecked(false);
			m_pComponents[COMPONENT_MessageCycle]->GetCtrlPanelCheckBox().SetChecked(true);
			static_cast<MessageCycle *>(m_pComponents[COMPONENT_MessageCycle])->AddMessage("Welcome! Stream Starting Soon", false);
		}
	);
	m_PresetLiveBtn.SetText("Live");
	m_PresetLiveBtn.SetButtonClickedCallback(
		[this](HyButton *pButton)
		{
			m_pComponents[COMPONENT_Crt]->GetCtrlPanelCheckBox().SetChecked(true);
			m_pComponents[COMPONENT_Monitor]->GetCtrlPanelCheckBox().SetChecked(true);
			static_cast<Monitor *>(m_pComponents[COMPONENT_Monitor])->SetChannel(MONITORCHANNEL_ObsFull);
			static_cast<MessageCycle *>(m_pComponents[COMPONENT_MessageCycle])->RemoveMessage("Welcome! Stream Starting Soon");
		}
	);

	m_PresetBrb1Btn.SetText("BRB 1");
	m_PresetBrb1Btn.SetButtonClickedCallback(
		[this](HyButton *pButton)
		{
			StartBrb(1);
		});
	m_PresetBrb5Btn.SetText("BRB 5");
	m_PresetBrb5Btn.SetButtonClickedCallback(
		[this](HyButton *pButton)
		{
			StartBrb(5);
		});
	m_PresetBrb10Btn.SetText("BRB 10");
	m_PresetBrb10Btn.SetButtonClickedCallback(
		[this](HyButton *pButton)
		{
			StartBrb(10);
		});

	m_PresetBrbOffBtn.SetText("BRB Off");
	m_PresetBrbOffBtn.SetButtonClickedCallback(
		[this](HyButton *pButton)
		{
			ClearBrb();
		}
	);
	m_PresetEndingBtn.SetText("End");
	m_PresetEndingBtn.SetButtonClickedCallback(
		[this](HyButton *pButton)
		{
			m_pComponents[COMPONENT_Crt]->GetCtrlPanelCheckBox().SetChecked(true);
			m_pComponents[COMPONENT_Monitor]->GetCtrlPanelCheckBox().SetChecked(false);
			m_pComponents[COMPONENT_NowPlaying]->GetCtrlPanelCheckBox().SetChecked(false);
			m_pComponents[COMPONENT_LiveSplit]->GetCtrlPanelCheckBox().SetChecked(false);
			static_cast<MessageCycle *>(m_pComponents[COMPONENT_MessageCycle])->ClearMessages();
			static_cast<MessageCycle *>(m_pComponents[COMPONENT_MessageCycle])->GetCtrlPanelCheckBox().SetChecked(true);
			static_cast<MessageCycle *>(m_pComponents[COMPONENT_MessageCycle])->AddMessage("Stream Ending! Thanks for watching", true);
		}
	);

	HyLayoutHandle hRow = m_CtrlPanel.InsertLayout(HYORIENT_Horizontal);
	m_CtrlPanel.InsertWidget(m_PresetBrb1Btn, hRow);
	m_CtrlPanel.InsertWidget(m_PresetBrb5Btn, hRow);
	m_CtrlPanel.InsertWidget(m_PresetBrb10Btn, hRow);
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
	for(int i = 0; i < NUM_COMPONENTS; ++i)
		delete m_pComponents[i];
}

/*virtual*/ bool Melody::OnUpdate() /*override*/
{
	GameBrowser *pGameBrowser = static_cast<GameBrowser *>(m_pComponents[COMPONENT_GameBrowser]);

	if(pGameBrowser->IsShowing())
	{
		const float fScaleDuration = 15.0f;
		if(pGameBrowser->scale.IsAnimating() == false)
		{
			if(pGameBrowser->scale.X() >= GAMEBROWSE_MAX_SIZE)
				pGameBrowser->scale.Tween(GAMEBROWSE_MAX_SIZE - 0.02f, GAMEBROWSE_MAX_SIZE - 0.02f, fScaleDuration, HyTween::QuadInOut);
			else
				pGameBrowser->scale.Tween(GAMEBROWSE_MAX_SIZE, GAMEBROWSE_MAX_SIZE, fScaleDuration, HyTween::QuadInOut);
		}

		const float fPosDuration = 12.0f;
		if(pGameBrowser->pos.IsAnimating() == false)
		{
			if(pGameBrowser->pos.Y() >= 0.0f)
				pGameBrowser->pos.Tween(0.0f, -5.0f, fPosDuration, HyTween::QuadInOut);
			else
				pGameBrowser->pos.Tween(0.0f, 5.0f, fPosDuration, HyTween::QuadInOut);
		}

		const float fRotDuration = 30.0f;
		if(pGameBrowser->rot.IsAnimating() == false)
		{
			if(pGameBrowser->rot.Get() >= 0.0f)
				pGameBrowser->rot.Tween(-1.0f, fRotDuration);
			else
				pGameBrowser->rot.Tween(1.0f, fRotDuration);
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

/*static*/ IComponent *Melody::GetComponent(ComponentType eType)
{
	return sm_pThis->m_pComponents[eType];
}

/*static*/ void Melody::RefreshCamera()
{
	if(static_cast<Crt *>(sm_pThis->m_pComponents[COMPONENT_Crt])->GetChannel() != CHANNELTYPE_Game)
	{
		if(sm_pThis->m_pComponents[COMPONENT_LiveSplit]->IsVisible() || static_cast<NowPlaying *>(sm_pThis->m_pComponents[COMPONENT_NowPlaying])->IsVisible())
		{
			HyEngine::Window().GetCamera2d(0)->pos.Tween(CAMERA_DIVIDER_POS, 1.5f, HyTween::QuadInOut);
			HyEngine::Window().GetCamera2d(0)->scale.Tween(CAMERA_DIVIDER_SCALE, 1.5f, HyTween::QuadInOut);
			HyEngine::Window().GetCamera2d(0)->SetTag(CAMTAG_Divider);

			static_cast<MessageCycle *>(sm_pThis->m_pComponents[COMPONENT_MessageCycle])->SetXPosOffset(MESSAGECYCLE_POS_X);
		}
		else
		{
			HyEngine::Window().GetCamera2d(0)->pos.Tween(CAMERA_CENTER_POS, 1.5f, HyTween::QuadInOut);
			HyEngine::Window().GetCamera2d(0)->scale.Tween(CAMERA_CENTER_SCALE, 1.5f, HyTween::QuadInOut);
			HyEngine::Window().GetCamera2d(0)->SetTag(CAMTAG_Center);

			static_cast<MessageCycle *>(sm_pThis->m_pComponents[COMPONENT_MessageCycle])->SetXPosOffset(0.0f);
		}
	}
}

void Melody::StartBrb(int iBrbTime)
{
	m_pComponents[COMPONENT_Crt]->GetCtrlPanelCheckBox().SetChecked(true);
	m_pComponents[COMPONENT_Monitor]->GetCtrlPanelCheckBox().SetChecked(true);
	static_cast<Monitor *>(m_pComponents[COMPONENT_Monitor])->SetChannel(MONITORCHANNEL_Brb);
	static_cast<NowPlaying *>(m_pComponents[COMPONENT_NowPlaying])->ShowGameTime(false);
	m_pComponents[COMPONENT_MessageCycle]->GetCtrlPanelCheckBox().SetChecked(true);

	std::string sMsg;
	if(iBrbTime > 0)
	{
		if(iBrbTime == 1)
			sMsg = "BRB 1 Minute!";
		else
			sMsg = "BRB " + std::to_string(iBrbTime) + " Minutes!";
	}
	else
		sMsg = "BRB!";
	static_cast<MessageCycle *>(m_pComponents[COMPONENT_MessageCycle])->AddMessage(sMsg, false);
}

void Melody::ClearBrb()
{
	m_pComponents[COMPONENT_Crt]->GetCtrlPanelCheckBox().SetChecked(true);
	m_pComponents[COMPONENT_Monitor]->GetCtrlPanelCheckBox().SetChecked(true);
	static_cast<Monitor *>(m_pComponents[COMPONENT_Monitor])->SetChannel(MONITORCHANNEL_ObsFull);
	static_cast<MessageCycle *>(m_pComponents[COMPONENT_MessageCycle])->RemoveMessage("BRB!");
	static_cast<MessageCycle *>(m_pComponents[COMPONENT_MessageCycle])->RemoveMessage("BRB 1 Minute!");
	static_cast<MessageCycle *>(m_pComponents[COMPONENT_MessageCycle])->RemoveMessage("BRB 5 Minutes!");
	static_cast<MessageCycle *>(m_pComponents[COMPONENT_MessageCycle])->RemoveMessage("BRB 10 Minutes!");
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
