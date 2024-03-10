#ifndef IController_h__
#define IController_h__

#include "pch.h"

class Dpad;

class IController : public HyEntity2d
{
protected: // Construct data in derived classes
	Dpad *							m_pDpad;
	std::vector<HySprite2d *>		m_ButtonList;

public:
	IController(HyEntity2d *pParent = nullptr);
	virtual ~IController();

	virtual void ApplyInputs() = 0;
};

#endif // IController_h__
