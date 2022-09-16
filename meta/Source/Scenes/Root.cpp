#include "pch.h"
#include "Root.h"

Root::Root(HyEntity2d *pParent /*= nullptr*/) :
	HyEntity2d(pParent),
	m_Crt(this),
	m_Message(this),
	m_pCurrentScene(nullptr),
	m_Intro(this),
	m_LowerThird(this),
	m_Break(this),
	m_Wheel(WheelInit(), this),
	m_Outro(this)
{
	m_Intro.Hide(0.0f);
	m_LowerThird.Hide(0.0f);
	m_Break.Hide(0.0f);
	m_Wheel.Hide(0.0f);
	m_Outro.Hide(0.0f);

	m_pCurrentScene = &m_Intro;
	m_Intro.Show(0.0f);
}

/*virtual*/ Root::~Root()
{
}

Crt &Root::GetCrt()
{
	return m_Crt;
}

Message &Root::GetMessage()
{
	return m_Message;
}

/*virtual*/ void Root::OnUpdate() /*override*/
{
	// INPUT
	IScene *pNextScene = nullptr;
	if(HyEngine::Input().IsActionReleased(INPUT_GotoIntro))
		pNextScene = &m_Intro;
	else if(HyEngine::Input().IsActionReleased(INPUT_GotoLowerThird))
		pNextScene = &m_LowerThird;
	else if(HyEngine::Input().IsActionReleased(INPUT_GotoBreak))
		pNextScene = &m_Break;
	else if(HyEngine::Input().IsActionReleased(INPUT_GotoWheel))
		pNextScene = &m_Wheel;
	else if(HyEngine::Input().IsActionReleased(INPUT_GotoOutro))
		pNextScene = &m_Outro;

	// TRANSITION
	if(pNextScene && pNextScene != m_pCurrentScene)
	{
		m_pCurrentScene->Hide(0.0f);
		pNextScene->Show(2.0f);
		m_pCurrentScene = pNextScene;
		pNextScene = nullptr;
	}
}
