#ifndef HeartBeat_h__
#define HeartBeat_h__

#include "pch.h"
#include "IComponent.h"

#include <simpleble/SimpleBLE.h>

class HeartBeat : public IComponent
{
public:
	HeartBeat(HyEntity2d *pParent = nullptr);
	virtual ~HeartBeat();

protected:
	virtual void OnUpdate() override;
};

#endif // HeartBeat_h__
