#include "pch.h"
#include "Monitor.h"
#include "CtrlPanel.h"
#include "Melody.h"

#define MONITOR_WIDTH 488
#define MONITOR_HEIGHT 268
#define SCREEN_OFFSET_X 23
#define SCREEN_OFFSET_Y 27

#define SHADOW_ALPHA 0.42f

Monitor::Monitor(HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_Brb, pParent),
	m_CtrlPanel_CheckBox(HyPanelInit(32, 32, 2), HyNodePath("", "CtrlPanel")),
	m_CtrlPanel_Brb(HyPanelInit(32, 32, 2), HyNodePath("", "CtrlPanel")),
	m_CtrlPanel_LiveSplit(HyPanelInit(32, 32, 2), HyNodePath("", "CtrlPanel")),
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

	m_CtrlPanel_Brb.SetText("BRB");
	m_CtrlPanel_Brb.SetCheckedChangedCallback(
		[this](HyCheckBox *pCheckBox)
		{
			if(pCheckBox->IsChecked())
			{
				m_Brb.SetVisible(true);
				m_Brb.alpha.Set(0.0f);
				m_Brb.alpha.Tween(1.0f, 1.0f);

				m_ElapsedTime.Reset();
				m_ElapsedTime.Start();
			}
			else
			{
				m_Brb.alpha.Tween(0.0f, 1.0f, HyTween::Linear, 0.0f, [](IHyNode *pThis) { pThis->SetVisible(false); });
				m_ElapsedTime.Pause();
			}
		});

	m_LiveSplitMask.UseWindowCoordinates();
	m_LiveSplitMask.SetAsBox(MISC_WIDTH + DIVIDER_WIDTH, HyEngine::Window(0).GetHeight());
	m_LiveSplitMask.SetTint(HyColor::Orange);
	m_LiveSplitMask.SetDisplayOrder(DISPLAYORDER_LiveSplitMask);
	m_LiveSplitMask.alpha.Set(0.0f);
	m_LiveSplitMask.SetVisible(false);

	m_Shadow.SetTint(HyColor::Black);
	m_Shadow.alpha.Set(SHADOW_ALPHA);
	m_Shadow.pos.Set(20.0f, -20.0f);

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
	ctrlPanel.InsertWidget(m_CtrlPanel_Brb, hRow2);
}

/*virtual*/ void Monitor::Show(float fDuration) /*override*/
{
	

	//IComponent::Show(fDuration);

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
}
