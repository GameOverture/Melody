#ifndef ControlPanel_h__
#define ControlPanel_h__

#include "pch.h"

class ControlPanel : public HyEntity2d
{
public:
	ControlPanel(HyEntity2d *pParent = nullptr);
	virtual ~ControlPanel();

protected:
	virtual void OnUpdate() override;
};

#endif // ControlPanel_h__
