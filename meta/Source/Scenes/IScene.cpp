#include "pch.h"
#include "IScene.h"
#include "Root.h"

IScene::IScene(SceneType eType, HyEntity2d *pParent /*= nullptr*/) :
	HyEntity2d(pParent),
	m_eSCENE_TYPE(eType)
{
}

/*virtual*/ IScene::~IScene()
{
}

SceneType IScene::GetSceneType() const
{
	return m_eSCENE_TYPE;
}

Root &IScene::GetRoot()
{
	return *static_cast<Root *>(ParentGet());
}

/*virtual*/ void IScene::OnUpdate() /*override*/
{
}
