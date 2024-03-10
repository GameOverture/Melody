#include "pch.h"
#include "StarCitizen.h"

StarCitizen::StarCitizen(HyEntity2d *pParent /*= nullptr*/) :
	IController(pParent)
{
}

/*virtual*/ StarCitizen::~StarCitizen()
{
}

/*virtual*/ void StarCitizen::ApplyInputs() /*override*/
{
}
