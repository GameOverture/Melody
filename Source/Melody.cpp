#include "pch.h"
#include "Melody.h"

Melody::Melody(HyInit &initStruct) :
	HyEngine(initStruct),
	m_pCamera(HyEngine::Window().CreateCamera2d())
{
	HyEngine::Input().MapBtn(INPUT_ExitGame, HYKEY_Escape);
	HyEngine::Input().MapBtn(INPUT_CameraUp, HYKEY_W);
	HyEngine::Input().MapBtn(INPUT_CameraLeft, HYKEY_A);
	HyEngine::Input().MapBtn(INPUT_CameraDown, HYKEY_S);
	HyEngine::Input().MapBtn(INPUT_CameraRight, HYKEY_D);
}

Melody::~Melody()
{
}

/*static*/ Melody *Melody::Get()
{
	return static_cast<Melody *>(sm_pInstance);
}

/*virtual*/ bool Melody::OnUpdate() /*override*/
{
	if(HyEngine::Input().IsActionDown(INPUT_CameraUp))
		m_pCamera->PanUp();
	if(HyEngine::Input().IsActionDown(INPUT_CameraLeft))
		m_pCamera->PanLeft();
	if(HyEngine::Input().IsActionDown(INPUT_CameraDown))
		m_pCamera->PanDown();
	if(HyEngine::Input().IsActionDown(INPUT_CameraRight))
		m_pCamera->PanRight();
	
	return !HyEngine::Input().IsActionReleased(INPUT_ExitGame);
}
