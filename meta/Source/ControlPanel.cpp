#include "pch.h"
#include "ControlPanel.h"

ControlPanel::ControlPanel(HyEntity2d *pParent /*= nullptr*/) :
	HyEntity2d(pParent)
{
}

/*virtual*/ ControlPanel::~ControlPanel()
{
}

/*virtual*/ void ControlPanel::OnUpdate() /*override*/
{
}
