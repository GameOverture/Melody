#include "pch.h"
#include "IComponent.h"
#include "CtrlPanel.h"

IComponent::IComponent(ComponentType eType, HyEntity2d *pParent /*= nullptr*/) :
	HyEntity2d(pParent),
	m_eCOMPONENT_TYPE(eType)
{
}

/*virtual*/ IComponent::~IComponent()
{
}

ComponentType IComponent::GetComponentType() const
{
	return m_eCOMPONENT_TYPE;
}

/*virtual*/ void IComponent::Show(float fDuration)
{
	alpha.Set(0.0f);
	alpha.Tween(1.0f, fDuration);
	SetVisible(true);
}

/*virtual*/ void IComponent::Hide(float fDuration)
{
	alpha.Tween(0.0f, fDuration, HyTween::Linear, 0.0f, [this](IHyNode *pThis) { pThis->SetVisible(false); });
}

/*virtual*/ void IComponent::OnUpdate() /*override*/
{
}
