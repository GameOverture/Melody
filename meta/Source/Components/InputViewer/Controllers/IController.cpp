#include "pch.h"
#include "IController.h"

IController::IController(HyEntity2d *pParent /*= nullptr*/) :
	HyEntity2d(pParent)
{
}

/*virtual*/ IController::~IController()
{
}
