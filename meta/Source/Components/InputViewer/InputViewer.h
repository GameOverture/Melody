#ifndef InputViewer_h__
#define InputViewer_h__

#include "pch.h"
#include "IComponent.h"
#include "IController.h"

class InputViewer : public IComponent
{
	HyCheckBox				m_CtrlPanel_CheckBox;

	IController *			m_Controllers[NUM_INPUTCONTROLLERS];
	InputController			m_eActiveController;

public:
	InputViewer(HyEntity2d *pParent = nullptr);
	virtual ~InputViewer();

	virtual void PopulateCtrlPanel(CtrlPanel &ctrlPanel) override;
	virtual void Show(float fDuration) override;
	virtual void Hide(float fDuration) override;

protected:
	virtual void OnUpdate() override;
};

#endif // InputViewer_h__
