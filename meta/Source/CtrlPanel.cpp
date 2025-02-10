#include "pch.h"
#include "CtrlPanel.h"
#include "IComponent.h"
#include "Crt.h"
#include "MessageCycle.h"

CtrlPanel::CtrlPanel(HyEntity2d *pParent /*= nullptr*/) :
	HyUiContainer(HYORIENT_Vertical, HyPanelInit(), pParent)
{
	SetSize(HyEngine::Window(1).GetWidth(), HyEngine::Window(1).GetHeight());
}

/*virtual*/ CtrlPanel::~CtrlPanel()
{
	for(HyDividerLine *pDivider : m_DividerLineList)
		delete pDivider;
}

void CtrlPanel::InsertDividerLine()
{
	InsertSpacer(HYSIZEPOLICY_Fixed, 10);

	HyDividerLine *pNewDivider = HY_NEW HyDividerLine(HYORIENT_Horizontal, 2.0f, HyColor::White);
	InsertWidget(*pNewDivider);

	m_DividerLineList.push_back(pNewDivider);
}
