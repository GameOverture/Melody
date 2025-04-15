#include "pch.h"
#include "LiveSplit.h"
#include "Monitor.h"
#include "CtrlPanel.h"
#include "Melody.h"

LiveSplit::LiveSplit(Monitor &monitorRef, HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_LiveSplit, pParent),
	m_MonitorRef(monitorRef),
	m_LiveSplitMask(this)
{
	m_CtrlPanel_CheckBox.SetText("LiveSplit");
	m_CtrlPanel_CheckBox.SetCheckedChangedCallback(
		[this](HyCheckBox *pCheckBox)
		{
			if(pCheckBox->IsChecked())
				Show(0.5f);
			else
				Hide(0.5f);
		});

	m_LiveSplitMask.UseWindowCoordinates();
	m_LiveSplitMask.SetAsBox(MISC_WIDTH + DIVIDER_WIDTH, LIVESPLIT_HEIGHT + 20);// HyEngine::Window(0).GetHeight());
	m_LiveSplitMask.SetTint(HyColor::Orange);
	m_LiveSplitMask.alpha.Set(0.0f);
}

/*virtual*/ LiveSplit::~LiveSplit()
{
}

/*virtual*/ void LiveSplit::PopulateCtrlPanel(CtrlPanel &ctrlPanel) /*override*/
{
	HyLayoutHandle hRow = ctrlPanel.InsertLayout(HYORIENT_Horizontal);
	ctrlPanel.InsertWidget(m_CtrlPanel_CheckBox, hRow);
	ctrlPanel.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);
}

/*virtual*/ void LiveSplit::Show(float fDuration) /*override*/
{
	SetVisible(true);

	m_LiveSplitMask.alpha.Set(0.0f);
	m_LiveSplitMask.alpha.Tween(1.0f, 1.0f);

	m_MonitorRef.GetShadow().alpha.Tween(0.0f, 0.5f);

	Melody::RefreshCamera();
}

/*virtual*/ void LiveSplit::Hide(float fDuration) /*override*/
{
	m_LiveSplitMask.alpha.Tween(0.0f, 1.0f, HyTween::Linear, 0.0f,
		[this](IHyNode *pThis)
		{
			SetVisible(false);
			Melody::RefreshCamera();
		});

	m_MonitorRef.GetShadow().alpha.Tween(SHADOW_ALPHA, 1.0f);
}

/*virtual*/ void LiveSplit::OnUpdate() /*override*/
{
}
