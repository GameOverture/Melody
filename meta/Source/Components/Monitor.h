#ifndef Monitor_h__
#define Monitor_h__

#include "pch.h"
#include "IComponent.h"

class Monitor : public IComponent
{
	HyCheckBox			m_CtrlPanel_CheckBox;
	HyCheckBox			m_CtrlPanel_LiveSplit;
	HyCheckBox			m_CtrlPanel_Brb;

	HyPrimitive2d		m_LiveSplitMask;
	HySprite2d			m_Shadow;
	HyPrimitive2d		m_ColorKeyBg;
	HyPrimitive2d		m_Background;
	HySprite2d			m_Brb;
	HySprite2d			m_Frame;

	HyStopwatch			m_ElapsedTime;
	HyText2d			m_ElapsedTimeText;

public:
	Monitor(HyEntity2d *pParent = nullptr);
	virtual ~Monitor();

	virtual void PopulateCtrlPanel(CtrlPanel &ctrlPanel) override;

	virtual void Show(float fDuration) override;
	virtual void Hide(float fDuration) override;

	bool IsDivider() const;
	bool IsBrb() const;

protected:
	virtual void OnUpdate() override;
};

#endif // Monitor_h__
