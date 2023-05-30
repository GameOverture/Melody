#include "pch.h"
#include "LowerThird.h"
#include "Root.h"

LowerThird::LowerThird(HyEntity2d *pParent /*= nullptr*/) :
	IScene(SCENE_LowerThird, pParent),
	m_Border(this)
{
	m_Border.SetAsBox(HyEngine::Window().GetWidthF(), LOWERTHIRD_HEIGHT);
	m_Border.SetWireframe(true);

	UseWindowCoordinates(0);
}

/*virtual*/ LowerThird::~LowerThird()
{
}

/*virtual*/ void LowerThird::Show(float fDuration) /*override*/
{
	m_Border.SetVisible(true);

	GetRoot().GetCrt().alpha.Tween(0.0f, fDuration);

	GetRoot().GetMessage().SetMsgs("Big 20 #15 Practice", "Race Day Sept. 17", 30.0f, 3.0f);
	GetRoot().GetMessage().pos.Tween(HyEngine::Window().GetWidthF(0.5f), 38.0f, fDuration, HyTween::QuadInOut);
}

/*virtual*/ void LowerThird::Hide(float fDuration) /*override*/
{
	m_Border.SetVisible(false);
}

/*virtual*/ void LowerThird::OnUpdate() /*override*/
{
}
