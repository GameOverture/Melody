#include "pch.h"
#include "Code.h"
#include "CtrlPanel.h"

Code::Code(HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_Code, pParent),
	m_Background(this)
{
	m_CtrlPanel_CheckBox.SetText("Code");
	m_CtrlPanel_CheckBox.SetCheckedChangedCallback(
		[this](HyCheckBox *pCheckBox)
		{
			if(pCheckBox->IsChecked())
				Show(0.5f);
			else
				Hide(0.5f);
		});

	m_Background.SetAsBox(SCREEN_WIDTH, LOWERTHIRD_HEIGHT);
	m_Background.SetTint(HyColor(31, 31, 31));
	m_Background.SetDisplayOrder(DISPLAYORDER_LowerThird);

	UseWindowCoordinates();
}

/*virtual*/ Code::~Code()
{
}

/*virtual*/ void Code::PopulateCtrlPanel(CtrlPanel &ctrlPanel) /*override*/
{
	HyLayoutHandle hRow = ctrlPanel.InsertLayout(HYORIENT_Horizontal);
	ctrlPanel.InsertWidget(m_CtrlPanel_CheckBox, hRow);
	ctrlPanel.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);

	ctrlPanel.InsertDividerLine();
}

/*virtual*/ void Code::Show(float fDuration) /*override*/
{
	SetVisible(true);
	alpha.Set(0.0f);
	alpha.Tween(1.0f, fDuration);
}

/*virtual*/ void Code::Hide(float fDuration) /*override*/
{
	alpha.Tween(0.0f, fDuration, HyTween::Linear, 0.0f, [this](IHyNode *pThis) { pThis->SetVisible(false); });
}

/*virtual*/ void Code::OnUpdate() /*override*/
{
}
