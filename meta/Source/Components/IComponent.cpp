#include "pch.h"
#include "IComponent.h"
#include "CtrlPanel.h"

#define LGAUTOBETDLG_CheckBoxSize 32
#define LGAUTOBETDLG_CheckBoxStroke 4

const std::string g_ComponentTypeNames[NUM_COMPONENTS] =
{
	"Fight Stick",
	"VgMusic",
	"Crt",
	"Wheel",
};

IComponent::IComponent(ComponentType eType, HyEntity2d *pParent /*= nullptr*/) :
	HyEntity2d(pParent),
	m_eCOMPONENT_TYPE(eType),
	m_CtrlPanelCheckBox(HyPanelInit(LGAUTOBETDLG_CheckBoxSize, LGAUTOBETDLG_CheckBoxSize, LGAUTOBETDLG_CheckBoxStroke), "", "CtrlPanel")
{
	m_CtrlPanelCheckBox.SetText("Show " + g_ComponentTypeNames[eType]);
	m_CtrlPanelCheckBox.SetCheckedChangedCallback(
		[this](HyCheckBox *pCheckBox, void *pData)
		{
			if(pCheckBox->IsChecked())
				reinterpret_cast<IComponent *>(pData)->Show(0.5f);
			else
				reinterpret_cast<IComponent *>(pData)->Hide(0.5f);
		}, this);
}

/*virtual*/ IComponent::~IComponent()
{
}

ComponentType IComponent::GetComponentType() const
{
	return m_eCOMPONENT_TYPE;
}

HyCheckBox &IComponent::GetCtrlPanelCheckBox()
{
	return m_CtrlPanelCheckBox;
}

/*virtual*/ void IComponent::Show(float fDuration)
{
	alpha.Set(0.0f);
	alpha.Tween(1.0f, fDuration);
	SetVisible(true);
}

/*virtual*/ void IComponent::Hide(float fDuration)
{
	alpha.Tween(0.0f, fDuration, HyTween::Linear, 0.0f, [this](IHyNode *pThis) { pThis->SetVisible(false); });
}

/*virtual*/ void IComponent::OnUpdate() /*override*/
{
}
