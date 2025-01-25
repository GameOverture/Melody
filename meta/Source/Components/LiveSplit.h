#ifndef LiveSplit_h__
#define LiveSplit_h__

#include "pch.h"
#include "IComponent.h"

class Monitor;

class LiveSplit : public IComponent
{
	Monitor &			m_MonitorRef;

	HyPrimitive2d		m_LiveSplitMask;

public:
	LiveSplit(Monitor &monitorRef, HyEntity2d *pParent = nullptr);
	virtual ~LiveSplit();

	virtual void PopulateCtrlPanel(CtrlPanel &ctrlPanel) override;

	virtual void Show(float fDuration) override;
	virtual void Hide(float fDuration) override;

protected:
	virtual void OnUpdate() override;
};

#endif // LiveSplit_h__
