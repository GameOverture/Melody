#include "pch.h"
#include "Code.h"
#include "CtrlPanel.h"
#include "Melody.h"
#include "Crt.h"

Code::Code(HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_Code, pParent),
	//m_CameraBg(this),
	m_LowerThirdBg(this)
{
	m_CtrlPanel_CheckBox.SetText("Code");
	m_CtrlPanel_CheckBox.SetCheckedChangedCallback(
		[this](HyCheckBox *pCheckBox)
		{
			if(pCheckBox->IsChecked())
			{
				if(static_cast<Crt *>(Melody::GetComponent(COMPONENT_Crt))->IsPowerOn())
					static_cast<Crt *>(Melody::GetComponent(COMPONENT_Crt))->Hide(0.5f);
				if(Melody::GetComponent(COMPONENT_NowPlaying)->IsVisible())
					Melody::GetComponent(COMPONENT_NowPlaying)->Hide(0.5f);

				Show(0.5f);
			}
			else
			{
				if(static_cast<Crt *>(Melody::GetComponent(COMPONENT_Crt))->IsPowerOn() == false)
					static_cast<Crt *>(Melody::GetComponent(COMPONENT_Crt))->Show(0.5f);
				Hide(0.5f);
			}
		});

	m_LowerThirdBg.SetAsBox(SCREEN_WIDTH, LOWERTHIRD_HEIGHT);
	m_LowerThirdBg.SetTint(HyColor(31, 31, 31));
	m_LowerThirdBg.SetDisplayOrder(DISPLAYORDER_LowerThird);

	//m_CameraBg.SetAsBox(314, 515);
	//m_CameraBg.SetTint(HyColor(31, 31, 31));
	//m_CameraBg.SetDisplayOrder(DISPLAYORDER_LowerThird);

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
	m_CtrlPanel_CheckBox.SetChecked(true);
	alpha.Set(0.0f);
	alpha.Tween(1.0f, fDuration, HyTween::Linear, 1.5f);
	SetVisible(true);
	Melody::RefreshCamera();
}

/*virtual*/ void Code::Hide(float fDuration) /*override*/
{
	IComponent::Hide(fDuration);
}

/*virtual*/ void Code::OnUpdate() /*override*/
{
}
