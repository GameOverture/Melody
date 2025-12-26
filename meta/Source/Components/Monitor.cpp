#include "pch.h"
#include "Monitor.h"
#include "CtrlPanel.h"
#include "Melody.h"
#include "VgMusic.h"

#define MONITOR_WIDTH 488
#define MONITOR_HEIGHT 268
#define SCREEN_OFFSET_X 33
#define SCREEN_OFFSET_Y 27

Monitor::Monitor(HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_Monitor, pParent),
	m_iChannelIndex(MONITORCHANNEL_NoSignal),
	m_fChannelShowTime(0.0f),
	m_eMonitorState(MONITORSTATE_Idle),
	m_fElapsedTime(0.0f),
	m_Shadow("Monitor", "Frame", this),
	m_Background(this),
	m_Brb("Monitor", "BRB", this),
	m_Frame("Monitor", "Frame", this),
	m_NoSignal(this),
	m_ObsMask(this),
	m_ElapsedTimeText("", "MainText", this)
{
	m_CtrlPanel_CheckBox.SetText("Monitor");
	m_CtrlPanel_CheckBox.SetCheckedChangedCallback(
		[this](HyCheckBox *pCheckBox)
		{
			if(pCheckBox->IsChecked())
				Show(0.5f);
			else
				Hide(0.5f);
		});

	for(int iChannelIndex = 0; iChannelIndex < NUM_MONITORCHANNELS; ++iChannelIndex)
	{
		m_CtrlPanel_radChannel[iChannelIndex].Setup(HyUiPanelInit(15, 20, 2), HyNodePath("", "CtrlPanel"));
		m_CtrlPanel_radChannel[iChannelIndex].SetTag(iChannelIndex);
		m_CtrlPanel_radChannel[iChannelIndex].SetCheckedChangedCallback(
			[this](HyRadioButton *pRadio)
			{
				OnChannelChange(pRadio->GetTag());
			});

		m_BtnGrp.AddButton(m_CtrlPanel_radChannel[iChannelIndex]);
	}
	m_CtrlPanel_radChannel[MONITORCHANNEL_NoSignal].SetChecked(true);
	m_CtrlPanel_radChannel[MONITORCHANNEL_NoSignal].SetText("NUL");
	m_CtrlPanel_radChannel[MONITORCHANNEL_ObsFull].SetText("CAM");
	//m_CtrlPanel_radChannel[MONITORCHANNEL_ObsPartial].SetText("Partial");
	m_CtrlPanel_radChannel[MONITORCHANNEL_Brb].SetText("BRB");

	m_ChannelText.Init("CRT", "Volume", this);
	m_ChannelText.SetText("NULL");
	m_ChannelText.SetAlignment(HYALIGN_Right);

	m_ChannelText.pos.Set(SCREEN_OFFSET_X + MONITOR_WIDTH - 25, SCREEN_OFFSET_Y + 225);
	m_ChannelText.scale.Set(0.5f, 0.5f);
	m_ChannelText.SetVisible(false);

	m_Shadow.SetTint(HyColor::Black);
	m_Shadow.alpha.Set(SHADOW_ALPHA);
	m_Shadow.pos.Set(12.0f, -20.0f);

	m_ObsMask.SetTint(HyColor::Orange);
	m_ObsMask.SetVisible(false);

	m_Background.pos.Set(SCREEN_OFFSET_X, SCREEN_OFFSET_Y);
	m_Background.SetAsBox(MONITOR_WIDTH, MONITOR_HEIGHT);
	m_Background.SetTint(HyColor::DarkGray);

	m_Brb.SetVisible(false);
	m_Brb.scale.Set(0.5f, 0.5f);
	m_Brb.pos.Set(SCREEN_OFFSET_X + (MONITOR_WIDTH * 0.5f), SCREEN_OFFSET_Y + (MONITOR_HEIGHT * 0.5f));

	m_NoSignal.pos.Set(SCREEN_OFFSET_X, SCREEN_OFFSET_Y);
	m_NoSignal.SetAsBox(MONITOR_WIDTH, MONITOR_HEIGHT);
	m_NoSignal.SetTint(HyColor::Black);

	m_ElapsedTimeText.pos.Set(SCREEN_OFFSET_X + (MONITOR_WIDTH * 0.5f), SCREEN_OFFSET_Y + 20.0f);
	m_ElapsedTimeText.SetAlignment(HYALIGN_Center);

	//vgMusicRef.SetOnTrackChangeCallback(
	//	[this](MusicTrack &musicTrackRef)
	//	{
	//		InitNextTrack(musicTrackRef);
	//	});
	//vgMusicRef.SetOnFadeOutCallback(
	//	[this](float fFadeOutDuration)
	//	{
	//		FadeOut(fFadeOutDuration);
	//		m_eLargeState = LARGESTATE_Stopped;

	//		for(int32 i = 0; i < NUM_DANCERS; ++i)
	//			m_Dancers[i].DeferDance(DANCESTATE_Stop, fFadeOutDuration * 0.5f);
	//	});

	UseWindowCoordinates();
	SetDisplayOrder(DISPLAYORDER_Monitor);
	pos.Set(-MISC_WIDTH - 100, HyEngine::Window(0).GetHeight() - MISC_HEIGHT);
}

/*virtual*/ Monitor::~Monitor()
{
}

void Monitor::SetChannel(MonitorChannel eChannel)
{
	m_CtrlPanel_radChannel[eChannel].SetChecked(true);
}

HySprite2d &Monitor::GetShadow()
{
	return m_Shadow;
}

/*virtual*/ void Monitor::PopulateCtrlPanel(CtrlPanel &ctrlPanel) /*override*/
{
	HyLayoutHandle hRow = ctrlPanel.InsertLayout(HYORIENT_Horizontal);
	ctrlPanel.InsertWidget(m_CtrlPanel_CheckBox, hRow);
	
	//HyLayoutHandle hRow2 = ctrlPanel.InsertLayout(HYORIENT_Horizontal);
	for(int i = 0; i < NUM_MONITORCHANNELS; ++i)
		ctrlPanel.InsertWidget(m_CtrlPanel_radChannel[i], hRow);// , hRow2);

	ctrlPanel.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);
}

/*virtual*/ void Monitor::Show(float fDuration) /*override*/
{
	OnChannelChange(MONITORCHANNEL_NoSignal);
	SetVisible(true);
	scale.Set(0.5f, 0.5f);
	scale.Tween(1.0f, 1.0f, fDuration * 2.0f, HyTween::QuadIn);
	rot.Set(-20.0f);
	rot.Tween(0.0f, fDuration * 2.0f, HyTween::BounceOut);
	pos.Tween(0.0f, pos.GetY(), fDuration * 2.0f, HyTween::QuadInOut, 0.0f, 
		[this](IHyNode *pThis)
		{
			for(int iChn = 0; iChn < NUM_MONITORCHANNELS; ++iChn)
			{
				if(m_CtrlPanel_radChannel[iChn].IsChecked())
				{
					OnChannelChange(static_cast<MonitorChannel>(iChn));
					break;
				}
			}
			Melody::RefreshCamera();
		});
}

/*virtual*/ void Monitor::Hide(float fDuration) /*override*/
{
	OnChannelChange(MONITORCHANNEL_NoSignal);
	//IComponent::Hide(fDuration);
	rot.Tween(25.0f, fDuration, HyTween::QuadInOut);
	pos.Tween(-MISC_WIDTH - 100, static_cast<int>(pos.GetY()), fDuration, HyTween::QuadInOut, 0.0f,
		[this](IHyNode *pThis)
		{
			pThis->SetVisible(false);
			Melody::RefreshCamera();
		});
}

bool Monitor::IsBrb() const
{
	return m_Brb.IsVisible();
}

/*virtual*/ void Monitor::OnUpdate() /*override*/
{
	if(m_ElapsedTime.IsRunning())
		m_ElapsedTimeText.SetText(m_ElapsedTime.ToString());

	if(m_fChannelShowTime > 0.0f)
	{
		m_fChannelShowTime -= HyEngine::DeltaTime();
		if(m_fChannelShowTime <= 0.0f)
		{
			m_fChannelShowTime = 0.0f;
			m_ChannelText.SetVisible(false);
		}
	}

	switch(m_eMonitorState)
	{
	case MONITORSTATE_Idle:
		break;

	case MONITORSTATE_PreChangeChannel:
		switch(m_iChannelIndex)
		{
		case MONITORCHANNEL_NoSignal:
		case MONITORCHANNEL_ObsFull:
		//case MONITORCHANNEL_ObsPartial:
			m_Brb.alpha.Tween(0.0f, 1.0f, HyTween::Linear, 0.0f, [](IHyNode *pThis) { pThis->SetVisible(false); });
			m_ElapsedTimeText.alpha.Tween(0.0f, 1.0f, HyTween::Linear, 0.0f, [](IHyNode *pThis) { pThis->SetVisible(false); });
			m_ElapsedTime.Pause();
			break;

		case MONITORCHANNEL_Brb:
			break;

		default:
			HyLogError("Bad monitor channel index");
			break;
		}

		m_eMonitorState = MONITORSTATE_ChangingChannel;
		break;

	case MONITORSTATE_ChangingChannel:
		switch(m_iChannelIndex)
		{
		case MONITORCHANNEL_NoSignal:
			m_ChannelText.SetText("NO SIGNAL");
			m_ObsMask.SetVisible(false);
			break;
		case MONITORCHANNEL_ObsFull:
			m_ChannelText.SetText("CAMS");
			m_ObsMask.SetVisible(true);
			m_ObsMask.pos.Set(SCREEN_OFFSET_X, SCREEN_OFFSET_Y);
			m_ObsMask.SetAsBox(MONITOR_WIDTH, MONITOR_HEIGHT);
			break;

		//case MONITORCHANNEL_ObsPartial: {
		//	m_ChannelText.SetText("CAM");
		//	m_ObsMask.SetVisible(true);
		//	const int32 iCamWidth = 242;
		//	const int32 iCamHeight = 234;
		//	m_ObsMask.pos.Set(SCREEN_OFFSET_X + MONITOR_WIDTH - iCamWidth, SCREEN_OFFSET_Y + MONITOR_HEIGHT - iCamHeight);
		//	m_ObsMask.SetAsBox(iCamWidth, iCamHeight);
		//	break; }

		case MONITORCHANNEL_Brb:
			m_ObsMask.SetVisible(false);

			m_Brb.SetVisible(true);
			m_Brb.alpha.Set(0.0f);
			m_Brb.alpha.Tween(1.0f, 1.0f);

			m_ElapsedTimeText.SetVisible(true);
			m_ElapsedTimeText.alpha.Set(0.0f);
			m_ElapsedTimeText.alpha.Tween(1.0f, 1.0f);

			m_ElapsedTime.Reset();
			m_ElapsedTime.Start();

			m_ChannelText.SetText("BRB");
			break;

		default:
			HyLogError("Bad monitor channel index");
			break;
		}

		// Channel actually changing now
		m_ChannelText.SetVisible(true);
		m_fChannelShowTime = 2.0f;

		m_fElapsedTime = 0.0f;
		m_eMonitorState = MONITORSTATE_PostChangeChannel;
		break;

	case MONITORSTATE_PostChangeChannel:
		m_fElapsedTime += HyEngine::DeltaTime();
		if(m_fElapsedTime >= 0.5f)
		{
			m_eMonitorState = MONITORSTATE_Idle;
		}
		break;
	}
}

void Monitor::OnChannelChange(int iChannelIndex)
{
	iChannelIndex %= NUM_MONITORCHANNELS;
	iChannelIndex = fabs(iChannelIndex);
	if(m_iChannelIndex == iChannelIndex)
		return;

	m_iChannelIndex = iChannelIndex;
	m_NoSignal.SetVisible(m_iChannelIndex == MONITORCHANNEL_NoSignal);

	m_eMonitorState = MONITORSTATE_PreChangeChannel;
}
