#ifndef StarCitizen_h__
#define StarCitizen_h__

#include "pch.h"
#include "IController.h"

class StarCitizen : public IController
{
public:
	StarCitizen(HyEntity2d *pParent = nullptr);
	virtual ~StarCitizen();

	virtual void ApplyInputs() override;
};

#endif // StarCitizen_h__
