#include "pch.h"
#include "CtrlPanel.h"
#include "IComponent.h"

CtrlPanel::CtrlPanel(HyEntity2d *pParent /*= nullptr*/) :
	HyUiContainer(HYORIEN_Vertical, HyPanelInit(), pParent),
	m_MessagesLineEdit(HyPanelInit(400, 50, 5, HyColor::Blue), "", "CtrlPanel", this)
{
	SetSize(420, 1000);
}

/*virtual*/ CtrlPanel::~CtrlPanel()
{
}

void CtrlPanel::AddComponent(IComponent &componentRef)
{
	HyLayoutHandle hRow = InsertLayout(HYORIEN_Horizontal);
	InsertWidget(componentRef.GetCtrlPanelCheckBox(), hRow);
	InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);
}

void CtrlPanel::AddMessageWidgets()
{
	InsertWidget(m_MessagesLineEdit);
}

void CtrlPanel::FinishComponents()
{
	InsertSpacer();
}

/*virtual*/ void CtrlPanel::OnContainerUpdate() /*override*/
{
}
