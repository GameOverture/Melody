#include "pch.h"
#include "CtrlPanel.h"
#include "IComponent.h"

CtrlPanel::CtrlPanel(HyEntity2d *pParent /*= nullptr*/) :
	HyUiContainer(HYORIEN_Vertical, HyPanelInit(), pParent)
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

void CtrlPanel::FinishComponents()
{
	InsertSpacer();
}

/*virtual*/ void CtrlPanel::OnContainerUpdate() /*override*/
{
}
