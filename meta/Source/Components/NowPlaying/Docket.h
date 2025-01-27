#ifndef Docket_h__
#define Docket_h__

#include "pch.h"
#include "IComponent.h"

class Docket : public IComponent
{
public:
	Docket(HyEntity2d *pParent = nullptr);
	virtual ~Docket();

protected:
	virtual void OnUpdate() override;
};

#endif // Docket_h__
