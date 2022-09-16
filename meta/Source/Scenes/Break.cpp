#include "pch.h"
#include "Break.h"
#include "Root.h"

Break::Break(HyEntity2d *pParent /*= nullptr*/) :
	IScene(SCENE_Break, pParent)
{
}

/*virtual*/ Break::~Break()
{
}

/*virtual*/ void Break::Show(float fDuration) /*override*/
{
	GetRoot().GetCrt().alpha.Tween(1.0f, fDuration);
	//GetRoot().GetCrt().pos.Set(32.0f, 175.0f);

	GetRoot().GetMessage().SetMsgs("Taking a break...", "", 8.0f, 3.0f);
	GetRoot().GetMessage().pos.Tween(HyEngine::Window().GetWidthF(0.5f), 75.0f, fDuration, HyTween::QuadInOut);
}

/*virtual*/ void Break::Hide(float fDuration) /*override*/
{
	SetVisible(false);
}

/*virtual*/ void Break::OnUpdate() /*override*/
{
}
