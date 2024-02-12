#ifndef IComponent_h__
#define IComponent_h__

#include "pch.h"

class CtrlPanel;

class IComponent : public HyEntity2d
{
	const ComponentType		m_eCOMPONENT_TYPE;

public:
	IComponent(ComponentType eType, HyEntity2d *pParent = nullptr);
	virtual ~IComponent();

	ComponentType GetComponentType() const;
	
	virtual void PopulateCtrlPanel(CtrlPanel &ctrlPanel) = 0;
	virtual void Show(float fDuration);
	virtual void Hide(float fDuration);

protected:
	virtual void OnUpdate() override;
};

#endif // IComponent_h__
