#include "pch.h"
#include "Docket.h"

Docket::Docket(HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_Docket, pParent)
{
}

/*virtual*/ Docket::~Docket()
{
}

/*virtual*/ void Docket::OnUpdate() /*override*/
{
}
