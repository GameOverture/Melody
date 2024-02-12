#include "pch.h"
#include "Channel.h"

Channel::Channel(ChannelType eChannelType, HyEntity2d *pParent /*= nullptr*/) :
	HyEntity2d(pParent),
	m_eCHANNEL_TYPE(eChannelType)
{
}

/*virtual*/ Channel::~Channel()
{
}

/*virtual*/ void Channel::OnUpdate() /*override*/
{
}
