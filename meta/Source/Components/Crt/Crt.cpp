#include "pch.h"
#include "Crt.h"
#include "CtrlPanel.h"
#include "Monitor.h"
#include "InputViewer.h"
#include "Melody.h"

#define CRT_SHRINK_AMT 0.01f
#define CRT_SHUTOFF_DUR 0.2f

Crt::Crt(HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_Crt, pParent),
	m_CtrlPanel_btnGame(HyUiPanelInit(64, 32, 2), HyUiTextInit(HyNodePath("", "CtrlPanel"))),
	m_CtrlPanel_btnMusic(HyUiPanelInit(64, 32, 2), HyUiTextInit(HyNodePath("", "CtrlPanel"))),
	m_CtrlPanel_btnStatic(HyUiPanelInit(64, 32, 2), HyUiTextInit(HyNodePath("", "CtrlPanel"))),
	m_iChannelIndex(-1),
	m_Screen("CRT", "Screen", this),
	m_ChannelStack(this),
	m_ScreenOverlay("CRT", "ScreenOverlay", this),
	m_Static("CRT", "Static"),
	m_Frame("CRT", "Frame", this),
	m_Nob("CRT", "Nob", this),
	m_VcrTimeHrs("CRT", "VCR", this),
	m_VcrTimeMins("CRT", "VCR", this),
	m_fVolumeShowTime(0.0f),
	m_fChannelShowTime(0.0f),
	m_ChannelStatic(CHANNELTYPE_Static, &m_ChannelStack),
	m_ChannelMusic(&m_ChannelStack),
	m_ChannelGame(CHANNELTYPE_Game, &m_ChannelStack),
	m_eChannelState(CRTSTATE_Off),
	m_fElapsedTime(0.0f)
{
	m_ChannelList.push_back(&m_ChannelStatic);
	m_ChannelList.push_back(&m_ChannelMusic);
	m_ChannelList.push_back(&m_ChannelGame);
	HyAssert(m_ChannelList.size() == NUM_CHANNELTYPE, "Crt::Crt() - m_ChannelList.size() != CHANNELTYPE_COUNT");

	m_CtrlPanel_CheckBox.SetText("CRT");
	m_CtrlPanel_CheckBox.SetCheckedChangedCallback(
		[this](HyCheckBox *pCheckBox)
		{
			if(pCheckBox->IsChecked())
				Show(0.5f);
			else
				Hide(0.5f);
		});

	m_CtrlPanel_btnGame.SetText("Gam");
	m_CtrlPanel_btnGame.SetButtonClickedCallback(
		[this](HyButton *pButton)
		{
			SetChannel(CHANNELTYPE_Game);
		});

	m_CtrlPanel_btnMusic.SetText("Mus");
	m_CtrlPanel_btnMusic.SetButtonClickedCallback(
		[this](HyButton *pButton)
		{
			SetChannel(CHANNELTYPE_Music);
		});

	m_CtrlPanel_btnStatic.SetText("Nul");
	m_CtrlPanel_btnStatic.SetButtonClickedCallback(
		[this](HyButton *pButton)
		{
			SetChannel(CHANNELTYPE_Static);
		});

	const int32 iScreenX = RETRO_SIDES + 150;
	const int32 iScreenY = 280;
	
	m_Screen.pos.Set(iScreenX, iScreenY);
	m_ScreenOverlay.pos.Set(iScreenX, iScreenY);

	m_Static.scale.Set(2.4f, 2.4f);
	m_Static.pos.Set(-75, -20);
	m_Static.SetDisplayOrder(DISPLAYORDER_CrtStatic);
	m_Static.SetVisible(false);

	m_ChannelStack.pos.Set(iScreenX, iScreenY);
	m_ChannelStack.scale_pivot.Set(CRT_SCREEN_WIDTH * 0.5f, CRT_SCREEN_HEIGHT * 0.5f);
	m_ChannelStack.scale.Set(CRT_SHRINK_AMT, CRT_SHRINK_AMT);
	m_ChannelStack.SetVisible(false);
	m_ChannelStack.SetStencil(&m_Stencil);
	m_ChannelStack.ChildAppend(m_Static);
	//m_ChannelStack.SetPostProcessing(
	
	m_Nob.pos.Set(RETRO_SIDES + 1120.0f, 730.0f);

	m_VcrTimeHrs.pos.Set(RETRO_SIDES + 620, 1050);
	m_VcrTimeHrs.SetAlignment(HYALIGN_Right);
	m_VcrTimeMins.pos.Set(RETRO_SIDES + 640, 1050);

	m_VolumeText.Init("CRT", "Volume", this);
	m_VolumeText.SetText("VOLUME");
	m_VolumeText.pos.Set(iScreenX + 95, iScreenY + 160);
	m_VolumeText.SetDisplayOrder(DISPLAYORDER_CrtVolume);
	m_VolumeText.SetVisible(false);
	for(int i = 0; i < NUM_VOLUME_BARS; ++i)
	{
		m_VolumeBar[i].SetVisible(false);
		m_VolumeBar[i].SetTint(HyColor::Green);
		m_VolumeBar[i].SetAsBox(24.0f, 48.0f);
		m_VolumeBar[i].pos.Set(iScreenX + 100 + (i * 32), iScreenY + 100);
		m_VolumeBar[i].SetDisplayOrder(DISPLAYORDER_CrtVolume);
		
		ChildAppend(m_VolumeBar[i]);
	}

	m_ChannelText.Init("CRT", "Volume", this);
	m_ChannelText.SetText("CH 01");
	m_ChannelText.pos.Set(iScreenX + 532, iScreenY + 500);
	m_ChannelText.SetDisplayOrder(DISPLAYORDER_CrtVolume);
	m_ChannelText.SetVisible(false);

	m_Stencil.AddMask(m_Screen);

	pos.Set(HyEngine::Window(0).GetWidthF(-0.5f), HyEngine::Window(0).GetHeightF(-0.5f));
}

/*virtual*/ Crt::~Crt()
{
}

/*virtual*/ void Crt::PopulateCtrlPanel(CtrlPanel &ctrlPanel) /*override*/
{
	m_ChannelMusic.RegisterWithVgMusic();

	HyLayoutHandle hRow = ctrlPanel.InsertLayout(HYORIENT_Horizontal);
	ctrlPanel.InsertWidget(m_CtrlPanel_CheckBox, hRow);
	ctrlPanel.InsertWidget(m_CtrlPanel_btnGame, hRow);
	ctrlPanel.InsertWidget(m_CtrlPanel_btnMusic, hRow);
	ctrlPanel.InsertWidget(m_CtrlPanel_btnStatic, hRow);
	ctrlPanel.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);
}

/*virtual*/ void Crt::Show(float fDuration) /*override*/
{
	alpha.Set(0.0f);
	alpha.Tween(1.0f, fDuration, HyTween::Linear, 0.0f, [this](IHyNode *pThis) { TogglePower(true); });
	SetVisible(true);
}

/*virtual*/ void Crt::Hide(float fDuration) /*override*/
{
	TogglePower(false);
	alpha.Tween(0.0f, fDuration, HyTween::Linear, CRT_SHUTOFF_DUR + 1.0f, [this](IHyNode *pThis) { pThis->SetVisible(false); });
}

bool Crt::IsPowerOn() const
{
	return m_ChannelStack.IsVisible();
}

void Crt::TogglePower(bool bPowerOn)
{
	if(bPowerOn)
	{
		if(IsPowerOn() == false)
		{
			m_ChannelStack.SetVisible(true);
			m_ChannelStack.scale.Tween(1.0f, CRT_SHRINK_AMT, 0.2f, HyTween::Linear, 0.0f, [](IHyNode *pThis) { static_cast<HyEntity2d *>(pThis)->scale.Tween(1.0f, 1.0f, 0.4f, HyTween::BounceOut); });
			SetChannel(CHANNELTYPE_Static);
		}
	}
	else
	{
		if(IsPowerOn())
		{
			m_ChannelStack.scale.Tween(CRT_SHRINK_AMT, CRT_SHRINK_AMT, CRT_SHUTOFF_DUR, HyTween::BounceIn, 0.0f, [](IHyNode *pThis) {  pThis->SetVisible(false); });
			m_eChannelState = CRTSTATE_Off;
			m_iChannelIndex = -1;
		}
	}
}

int32 Crt::GetChannel() const
{
	return m_iChannelIndex;
}

void Crt::SetChannel(int32 iChannelIndex)
{
	if(m_eChannelState != CRTSTATE_Idle && m_eChannelState != CRTSTATE_Off)
		return;

	iChannelIndex %= NUM_CHANNELTYPE;
	iChannelIndex = fabs(iChannelIndex);
	if(m_iChannelIndex == iChannelIndex)
		return;

	m_iChannelIndex = iChannelIndex;
	m_eChannelState = CRTSTATE_PreChangeChannel;
}

void Crt::NudgeChannel(int32 iIndexOffset)
{
	SetChannel(m_iChannelIndex + iIndexOffset);
}

void Crt::SetVolume(float fVolume)
{
	m_VolumeText.SetVisible(true);
	for(int i = 0; i < NUM_VOLUME_BARS; ++i)
	{
		if(fVolume >= 0.05f)
		{
			m_VolumeBar[i].SetVisible(true);
			fVolume -= 0.05f;
		}
		else
			m_VolumeBar[i].SetVisible(false);
	}

	m_fVolumeShowTime = 2.0f;
}

/*virtual*/ void Crt::OnUpdate() /*override*/
{
	// VCR Time
	__time64_t long_time;
	_time64(&long_time);
	// Convert to local time.
	tm newtime;
	errno_t err = _localtime64_s(&newtime, &long_time);
	if(newtime.tm_hour > 12)	// Convert from 24-hour
		newtime.tm_hour -= 12;	// to 12-hour clock.
	if(newtime.tm_hour == 0)	// Set hour to 12 if midnight.
		newtime.tm_hour = 12;

	m_VcrTimeHrs.SetText(std::to_string(newtime.tm_hour));
	if(newtime.tm_min < 10)
		m_VcrTimeMins.SetText("0" + std::to_string(newtime.tm_min));
	else
		m_VcrTimeMins.SetText(std::to_string(newtime.tm_min));

	//if(HyEngine::Input().IsActionReleased(INPUT_CrtPowerToggle))
	//	TogglePower(!IsPowerOn());

	//if(HyEngine::Input().IsActionReleased(INPUT_CrtChannelUp))
	//	NudgeChannel(1);
	//if(HyEngine::Input().IsActionReleased(INPUT_CrtChannelDown))
	//	NudgeChannel(-1);

	if(m_fVolumeShowTime > 0.0f)
	{
		m_fVolumeShowTime -= HyEngine::DeltaTime();
		if(m_fVolumeShowTime <= 0.0f)
		{
			m_fVolumeShowTime = 0.0f;
			m_VolumeText.SetVisible(false);
			for(int i = 0; i < NUM_VOLUME_BARS; ++i)
				m_VolumeBar[i].SetVisible(false);
		}
	}

	if(m_fChannelShowTime > 0.0f)
	{
		m_fChannelShowTime -= HyEngine::DeltaTime();
		if(m_fChannelShowTime <= 0.0f)
		{
			m_fChannelShowTime = 0.0f;
			m_ChannelText.SetVisible(false);
		}
	}

	switch(m_eChannelState)
	{
	case CRTSTATE_Off:
		break;

	case CRTSTATE_PreChangeChannel:
		if(m_iChannelIndex == CHANNELTYPE_Game)
		{
			HyEngine::Window().GetCamera2d(0)->pos.Tween(CAMERA_GAME_POS, 1.5f, HyTween::QuadInOut);
			HyEngine::Window().GetCamera2d(0)->scale.Tween(CAMERA_GAME_SCALE, 1.5f, HyTween::QuadInOut);
			HyEngine::Window().GetCamera2d(0)->SetTag(CAMTAG_Game);
		}

		m_eChannelState = CRTSTATE_ChangingChannel;
		break;

	case CRTSTATE_ChangingChannel:
		if(HyEngine::Window().GetCamera2d(0)->pos.IsAnimating() || HyEngine::Window().GetCamera2d(0)->scale.IsAnimating())
			break;

		// Channel actually changing now
		m_ChannelText.SetVisible(true);
		m_ChannelText.SetText("CH 0" + std::to_string(m_iChannelIndex + 2)); // Start at channel 2 (static)
		m_fChannelShowTime = 2.0f;

		if(m_iChannelIndex == CHANNELTYPE_Game)
		{
			m_Screen.SetVisible(false);
			m_ScreenOverlay.SetVisible(false);

			InputViewer *pInputViewer = static_cast<InputViewer *>(Melody::GetComponent(COMPONENT_InputViewer));
			pInputViewer->RetroIntro();
			static_cast<MessageCycle *>(Melody::GetComponent(COMPONENT_MessageCycle))->SetXPosOffset(MESSAGECYCLE_GAMEPOS_X);
		}
		else
		{
			m_Screen.SetVisible(true);
			m_ScreenOverlay.SetVisible(true);

			Melody::RefreshCamera();

			InputViewer *pInputViewer = static_cast<InputViewer *>(Melody::GetComponent(COMPONENT_InputViewer));
			pInputViewer->RetroOutro();
		}

		for(int i = 0; i < NUM_CHANNELTYPE; ++i)
			m_ChannelList[i]->SetVisible(false);
		m_Static.SetVisible(true);
		m_Nob.rot.Tween(/*(360.0f / NUM_CHANNELTYPE)*/180.0f + (32.0f * m_iChannelIndex), 0.2f, HyTween::QuadIn);

		m_fElapsedTime = 0.0f;
		m_eChannelState = CRTSTATE_PostChangeChannel;
		break;

	case CRTSTATE_PostChangeChannel:
		m_fElapsedTime += HyEngine::DeltaTime();
		if(m_fElapsedTime >= 0.5f)
		{
			if(m_iChannelIndex != CHANNELTYPE_Static)
				m_Static.SetVisible(false);

			m_ChannelList[m_iChannelIndex]->SetVisible(true);
			m_eChannelState = CRTSTATE_Idle;
		}
		break;

	case CRTSTATE_Idle:
		break;
	}
}
