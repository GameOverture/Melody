#include "pch.h"
#include "Brb.h"

Brb::Brb(HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_Brb, pParent),
	m_Brb("", "BRB", this),
	m_ElapsedTimeText("", "MainText", this)
{
	m_ElapsedTimeText.pos.SetX(BRB_WIDTH * 0.5f);
	m_ElapsedTimeText.SetTextAlignment(HYALIGN_Center);
}

/*virtual*/ Brb::~Brb()
{
}

/*virtual*/ void Brb::Show(float fDuration) /*override*/
{
	m_ElapsedTime.Reset();
	m_ElapsedTime.Start();

	IComponent::Show(fDuration);
	pos.Tween(0.0f, pos.GetY(), fDuration * 2.0f, HyTween::QuadInOut);
}

/*virtual*/ void Brb::Hide(float fDuration) /*override*/
{
	IComponent::Hide(fDuration);
	pos.Tween(-500.0f, pos.GetY(), fDuration, HyTween::QuadInOut, 0.0f, [this](IHyNode *pThis) { m_ElapsedTime.Pause(); });
}

/*virtual*/ void Brb::OnUpdate() /*override*/
{
	if(m_ElapsedTime.IsRunning())
		m_ElapsedTimeText.SetText(m_ElapsedTime.ToString());
}
