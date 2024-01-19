#include "pch.h"
#include "HeartBeat.h"

HeartBeat::HeartBeat(HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_HeartBeat, pParent)
{
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

/*virtual*/ void HeartBeat::OnUpdate() /*override*/
{
}
