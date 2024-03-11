#ifndef InputViewer_h__
#define InputViewer_h__

#include "pch.h"
#include "IComponent.h"
#include "IController.h"

class InputViewer : public IComponent
{
	HyCheckBox				m_CtrlPanel_CheckBox;
	HyRadioButton			m_CtrlPanel_radController[NUM_INPUTCONTROLLERS];
	HyButtonGroup			m_BtnGrp;

	IController *			m_Controllers[NUM_INPUTCONTROLLERS];
	InputController			m_eActiveController;
	bool					m_bShowRetro;

public:
	InputViewer(HyEntity2d *pParent = nullptr);
	virtual ~InputViewer();

	virtual void PopulateCtrlPanel(CtrlPanel &ctrlPanel) override;
	virtual void Show(float fDuration) override;
	virtual void Hide(float fDuration) override;

	void SetShowRetro(bool bShow);
	void RetroIntro();
	void RetroOutro();

protected:
	virtual void OnUpdate() override;
};

#endif // InputViewer_h__
