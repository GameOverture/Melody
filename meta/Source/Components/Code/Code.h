#ifndef Code_h__
#define Code_h__

#include "pch.h"
#include "IComponent.h"

class Code : public IComponent
{
	HyPrimitive2d			m_Background;

public:
	Code(HyEntity2d *pParent = nullptr);
	virtual ~Code();

	virtual void PopulateCtrlPanel(CtrlPanel &ctrlPanel) override;
	virtual void Show(float fDuration) override;
	virtual void Hide(float fDuration) override;

protected:
	virtual void OnUpdate() override;
};

#endif // Code_h__
