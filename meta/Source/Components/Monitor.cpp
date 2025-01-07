#include "pch.h"
#include "Monitor.h"
#include "CtrlPanel.h"
#include "Melody.h"

#define MONITOR_WIDTH 488
#define MONITOR_HEIGHT 268
#define SCREEN_OFFSET_X 33
#define SCREEN_OFFSET_Y 27

#define SHADOW_ALPHA 0.42f

Monitor::Monitor(HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_Brb, pParent),
	m_CtrlPanel_CheckBox(HyPanelInit(32, 32, 2), HyNodePath("", "CtrlPanel")),
	m_CtrlPanel_LiveSplit(HyPanelInit(32, 32, 2), HyNodePath("", "CtrlPanel")),
	m_iChannelIndex(MONITORCHANNEL_NoSignal),
	m_fChannelShowTime(0.0f),
	m_eMonitorState(MONITORSTATE_Idle),
	m_fElapsedTime(0.0f),
	m_Shadow("Monitor", "Frame", this),
	m_ColorKeyBg(this),
	m_Background(this),
	m_Brb("Monitor", "BRB", this),
	m_Frame("Monitor", "Frame", this),
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

	m_CtrlPanel_LiveSplit.SetText("LiveSplit");
	m_CtrlPanel_LiveSplit.SetCheckedChangedCallback(
		[this](HyCheckBox *pCheckBox)
		{
			if(pCheckBox->IsChecked())
			{
				m_Shadow.alpha.Tween(0.0f, 0.5f);

				m_LiveSplitMask.alpha.Set(0.0f);
				m_LiveSplitMask.alpha.Tween(1.0f, 1.0f);
				m_LiveSplitMask.SetVisible(true);

				Melody::RefreshCamera();
			}
			else
			{
				m_Shadow.alpha.Tween(SHADOW_ALPHA, 1.0f);
				m_LiveSplitMask.alpha.Tween(0.0f, 1.0f, HyTween::Linear, 0.0f,
					[](IHyNode *pThis)
					{
						pThis->SetVisible(false);
						Melody::RefreshCamera();
					});
			}
		});

	for(int iChannelIndex = 0; iChannelIndex < NUM_MONITORCHANNELS; ++iChannelIndex)
	{
		m_CtrlPanel_radChannel[iChannelIndex].Setup(HyPanelInit(24, 24, 2), HyNodePath("", "CtrlPanel"));
		m_CtrlPanel_radChannel[iChannelIndex].SetTag(iChannelIndex);
		m_CtrlPanel_radChannel[iChannelIndex].SetCheckedChangedCallback(
			[this](HyRadioButton *pRadio)
			{
				SetChannel(pRadio->GetTag());
			});

		m_BtnGrp.AddButton(m_CtrlPanel_radChannel[iChannelIndex]);
	}
	m_CtrlPanel_radChannel[MONITORCHANNEL_NoSignal].SetChecked(true);
	m_CtrlPanel_radChannel[MONITORCHANNEL_NoSignal].SetText("Null");
	m_CtrlPanel_radChannel[MONITORCHANNEL_ObsCams].SetText("Cams");
	m_CtrlPanel_radChannel[MONITORCHANNEL_Brb].SetText("BRB");

	m_ChannelText.Init("CRT", "Volume", this);
	m_ChannelText.SetText("NULL");
	m_ChannelText.pos.Set(SCREEN_OFFSET_X + 350, SCREEN_OFFSET_Y + 225);
	m_ChannelText.scale.Set(0.5f, 0.5f);
	m_ChannelText.SetVisible(false);

	m_LiveSplitMask.UseWindowCoordinates();
	m_LiveSplitMask.SetAsBox(MISC_WIDTH + DIVIDER_WIDTH, HyEngine::Window(0).GetHeight());
	m_LiveSplitMask.SetTint(HyColor::Orange);
	m_LiveSplitMask.SetDisplayOrder(DISPLAYORDER_LiveSplitMask);
	m_LiveSplitMask.alpha.Set(0.0f);
	m_LiveSplitMask.SetVisible(false);

	m_Shadow.SetTint(HyColor::Black);
	m_Shadow.alpha.Set(SHADOW_ALPHA);
	m_Shadow.pos.Set(12.0f, -20.0f);

	m_ColorKeyBg.pos.Set(SCREEN_OFFSET_X, SCREEN_OFFSET_Y);
	m_ColorKeyBg.SetAsBox(MONITOR_WIDTH, MONITOR_HEIGHT);
	m_ColorKeyBg.SetTint(HyColor::Orange);

	m_Background.pos.Set(SCREEN_OFFSET_X, SCREEN_OFFSET_Y);
	m_Background.SetAsBox(MONITOR_WIDTH, MONITOR_HEIGHT);
	m_Background.SetTint(HyColor::Black);

	m_Brb.SetVisible(false);
	m_Brb.scale.Set(0.5f, 0.5f);
	m_Brb.pos.Set(SCREEN_OFFSET_X + (MONITOR_WIDTH * 0.5f), SCREEN_OFFSET_Y + (MONITOR_HEIGHT * 0.5f));

	m_ElapsedTimeText.pos.Set(SCREEN_OFFSET_X + (MONITOR_WIDTH * 0.5f), SCREEN_OFFSET_Y + 20.0f);
	m_ElapsedTimeText.SetAlignment(HYALIGN_Center);
}

/*virtual*/ Monitor::~Monitor()
{
}

/*virtual*/ void Monitor::PopulateCtrlPanel(CtrlPanel &ctrlPanel) /*override*/
{
	HyLayoutHandle hRow = ctrlPanel.InsertLayout(HYORIENT_Horizontal);
	ctrlPanel.InsertWidget(m_CtrlPanel_CheckBox, hRow);
	ctrlPanel.InsertWidget(m_CtrlPanel_LiveSplit, hRow);
	ctrlPanel.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);
	
	HyLayoutHandle hRow2 = ctrlPanel.InsertLayout(HYORIENT_Horizontal);
	for(int i = 0; i < NUM_MONITORCHANNELS; ++i)
		ctrlPanel.InsertWidget(m_CtrlPanel_radChannel[i], hRow2);
}

/*virtual*/ void Monitor::Show(float fDuration) /*override*/
{
	SetVisible(true);
	scale.Set(0.5f, 0.5f);
	scale.Tween(1.0f, 1.0f, fDuration * 2.0f, HyTween::QuadIn);
	rot.Set(-20.0f);
	rot.Tween(0.0f, fDuration * 2.0f, HyTween::BounceOut);
	pos.Tween(0.0f, pos.GetY(), fDuration * 2.0f, HyTween::QuadInOut, 0.0f, 
		[](IHyNode *pThis)
		{
			Melody::RefreshCamera();
		});
}

/*virtual*/ void Monitor::Hide(float fDuration) /*override*/
{
	//IComponent::Hide(fDuration);
	rot.Tween(25.0f, fDuration, HyTween::QuadInOut);
	pos.Tween(-MISC_WIDTH - 100, static_cast<int>(pos.GetY()), fDuration, HyTween::QuadInOut, 0.0f,
		[this](IHyNode *pThis)
		{
			pThis->SetVisible(false);
			Melody::RefreshCamera();
		});
}

bool Monitor::IsDivider() const
{
	return m_LiveSplitMask.IsVisible() || IsVisible();
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
			m_Brb.alpha.Tween(0.0f, 1.0f, HyTween::Linear, 0.0f, [](IHyNode *pThis) { pThis->SetVisible(false); });
			m_ElapsedTimeText.alpha.Tween(0.0f, 1.0f, HyTween::Linear, 0.0f, [](IHyNode *pThis) { pThis->SetVisible(false); });
			m_ElapsedTime.Pause();
			break;
		case MONITORCHANNEL_ObsCams:
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
			m_ChannelText.SetText("NULL");
			m_Background.SetVisible(true);
			break;
		case MONITORCHANNEL_ObsCams:
			m_ChannelText.SetText("CAMS");
			m_Background.SetVisible(false);
			break;
		case MONITORCHANNEL_Brb:
			m_Background.SetVisible(true);
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

void Monitor::SetChannel(int iChannelIndex)
{
	iChannelIndex %= NUM_MONITORCHANNELS;
	iChannelIndex = fabs(iChannelIndex);
	if(m_iChannelIndex == iChannelIndex)
		return;

	m_iChannelIndex = iChannelIndex;
	m_eMonitorState = MONITORSTATE_PreChangeChannel;
}
