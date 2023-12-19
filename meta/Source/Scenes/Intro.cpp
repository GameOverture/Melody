#include "pch.h"
#include "Intro.h"
#include "CtrlPanel.h"

Intro::Intro(HyEntity2d *pParent /*= nullptr*/) :
	IScene(SCENE_Intro, pParent)
{
	
}

/*virtual*/ Intro::~Intro()
{
}

/*virtual*/ void Intro::Show(float fDuration) /*override*/
{
	GetRoot().GetCrt().alpha.Tween(1.0f, fDuration);
	//GetRoot().GetCrt().pos.Set(32.0f, 175.0f);

	GetRoot().GetMessage().SetMsgs("Stream Starting Soon...", "Big 20 #15 Practice", 8.0f, 3.0f);
	GetRoot().GetMessage().pos.Tween(HyEngine::Window().GetWidthF(0.5f), 75.0f, fDuration, HyTween::QuadInOut);
}

/*virtual*/ void Intro::Hide(float fDuration) /*override*/
{
}

/*virtual*/ void Intro::OnUpdate() /*override*/
{
}
