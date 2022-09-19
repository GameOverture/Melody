#include "pch.h"
#include "Channel.h"

Channel::Channel(HyEntity2d *pParent /*= nullptr*/) :
	HyEntity2d(pParent),
	m_Static("CRT", "Static", this)
{
	m_Static.scale.Set(2.25f, 2.25f);
}

/*virtual*/ Channel::~Channel()
{
}

/*virtual*/ void Channel::OnUpdate() /*override*/
{
}
