#include "pch.h"
#include "Outro.h"
#include "Root.h"

Outro::Outro(HyEntity2d *pParent /*= nullptr*/) :
	IScene(SCENE_Outro, pParent)
{
}

/*virtual*/ Outro::~Outro()
{
}

/*virtual*/ void Outro::Show(float fDuration) /*override*/
{
	GetRoot().GetMessage().SetMsgs("Stream Ending!", "Thanks for watching", 5.0f, 3.0f);
	GetRoot().GetMessage().pos.Tween(HyEngine::Window().GetWidthF(0.5f), 75.0f, fDuration, HyTween::QuadInOut);
}

/*virtual*/ void Outro::Hide(float fDuration) /*override*/
{
	SetVisible(false);
}

/*virtual*/ void Outro::OnUpdate() /*override*/
{
}
