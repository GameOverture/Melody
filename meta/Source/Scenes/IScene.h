#ifndef IScene_h__
#define IScene_h__

#include "pch.h"

class Root;

enum SceneType
{
	SCENE_Intro = 0,
	SCENE_LowerThird,
	SCENE_Break,
	SCENE_Wheel,
	SCENE_Outro
};

class IScene : public HyEntity2d
{
	const SceneType		m_eSCENE_TYPE;

public:
	IScene(SceneType eType, HyEntity2d *pParent = nullptr);
	virtual ~IScene();

	SceneType GetSceneType() const;

	Root &GetRoot();

	virtual void Show(float fDuration) = 0;
	virtual void Hide(float fDuration) = 0;

protected:
	virtual void OnUpdate() override;
};

#endif // IScene_h__
