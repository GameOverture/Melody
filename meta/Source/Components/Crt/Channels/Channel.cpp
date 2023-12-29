#include "pch.h"
#include "Channel.h"

Channel::Channel(ComponentType eComponentType, HyEntity2d *pParent /*= nullptr*/) :
	IComponent(eComponentType, pParent),
	m_Static("CRT", "Static", this)
{
	m_Static.scale.Set(2.25f, 2.25f);
	m_Static.SetVisible(false);
}

/*virtual*/ Channel::~Channel()
{
}

void Channel::ShowStatic()
{
	m_Static.alpha.Set(0.0f);
	m_Static.alpha.Tween(1.0f, 0.5f);
	m_Static.SetVisible(true);
}

void Channel::HideStatic()
{
}

/*virtual*/ void Channel::OnUpdate() /*override*/
{
}
