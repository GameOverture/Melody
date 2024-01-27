#ifndef CtrlPanel_h__
#define CtrlPanel_h__

#include "pch.h"

class IComponent;

class CtrlPanel : public HyUiContainer
{
	HyLineEdit		m_MessagesLineEdit;

public:
	CtrlPanel(HyEntity2d *pParent = nullptr);
	virtual ~CtrlPanel();

	void AddComponent(IComponent &componentRef);
	void AddMessageWidgets();
	void FinishComponents();

protected:
	virtual void OnContainerUpdate() override;
};

#endif // CtrlPanel_h__
