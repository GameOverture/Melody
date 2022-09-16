#include "pch.h"
#include "Melody.h"

Melody::Melody(HarmonyInit &initStruct) :
	HyEngine(initStruct),
	m_pCamera(HyEngine::Window().CreateCamera2d())
{
	HyEngine::Input().MapBtn(INPUT_ExitGame, HYKEY_Escape);
	
	HyEngine::Input().MapBtn(INPUT_GotoIntro, HYKEY_1);
	HyEngine::Input().MapBtn(INPUT_GotoLowerThird, HYKEY_2);
	HyEngine::Input().MapBtn(INPUT_GotoBreak, HYKEY_3);
	HyEngine::Input().MapBtn(INPUT_GotoWheel, HYKEY_4);
	HyEngine::Input().MapBtn(INPUT_GotoOutro, HYKEY_6);
	HyEngine::Input().MapBtn(INPUT_CrtPowerToggle, HYKEY_P);

	m_Root.Load();
	m_Root.UseWindowCoordinates();
}

Melody::~Melody()
{
}

/*virtual*/ bool Melody::OnUpdate() /*override*/
{
	return !HyEngine::Input().IsActionReleased(INPUT_ExitGame);
}
