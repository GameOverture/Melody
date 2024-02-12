#include "pch.h"
#include "HeartBeat.h"
#include "CtrlPanel.h"

HeartBeat::HeartBeat(HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_HeartBeat, pParent),
	m_CtrlPanel_CheckBox(HyPanelInit(32, 32, 2), HyNodePath("", "CtrlPanel"))
{
	m_CtrlPanel_CheckBox.SetText("Heart Beat");
	m_CtrlPanel_CheckBox.SetCheckedChangedCallback(
		[this](HyCheckBox *pCheckBox, void *pData)
		{
			if(pCheckBox->IsChecked())
				reinterpret_cast<IComponent *>(pData)->Show(0.5f);
			else
				reinterpret_cast<IComponent *>(pData)->Hide(0.5f);
		}, this);

	if(SimpleBLE::Adapter::bluetooth_enabled() == false)
	{
		HyLogWarning("Bluetooth is not enabled");
		return;
	}

	std::vector<SimpleBLE::Adapter> adapterList = SimpleBLE::Adapter::get_adapters();
	if(adapterList.empty())
	{
		HyLogWarning("No Bluetooth adapters found");
		return;
	}

	for(uint32 i = 0; i < static_cast<uint32>(adapterList.size()); ++i)
	{
		SimpleBLE::Adapter &adapter = adapterList[i];
		HyLogSection("BlueTooth Adapter #" << i);
		HyLog("Identifier: " << adapter.identifier());
		HyLog("Address:    " << adapter.address());
	}
}

/*virtual*/ HeartBeat::~HeartBeat()
{
}

/*virtual*/ void HeartBeat::PopulateCtrlPanel(CtrlPanel &ctrlPanel) /*override*/
{
	HyLayoutHandle hRow = ctrlPanel.InsertLayout(HYORIEN_Horizontal);
	ctrlPanel.InsertWidget(m_CtrlPanel_CheckBox, hRow);
	ctrlPanel.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);
}

/*virtual*/ void HeartBeat::OnUpdate() /*override*/
{
}
