#ifndef Brb_h__
#define Brb_h__

#include "pch.h"
#include "IComponent.h"

#define BRB_WIDTH 500
#define BRB_HEIGHT 500

class Brb : public IComponent
{
	HyCheckBox				m_CtrlPanel_CheckBox;

	HySprite2d			m_Brb;
	HyStopwatch			m_ElapsedTime;
	HyText2d			m_ElapsedTimeText;

public:
	Brb(HyEntity2d *pParent = nullptr);
	virtual ~Brb();

	virtual void PopulateCtrlPanel(CtrlPanel &ctrlPanel) override;

	virtual void Show(float fDuration) override;
	virtual void Hide(float fDuration) override;

protected:
	virtual void OnUpdate() override;
};

#endif // Brb_h__
