#ifndef pch_h__
#define pch_h__

#include "HyEngine.h"

#define CRT_SCREEN_WIDTH 851
#define CRT_SCREEN_HEIGHT 613

#define LOWERTHIRD_HEIGHT 110.0f

enum InputAction
{
	INPUT_ExitGame = 0,

	INPUT_GotoIntro,
	INPUT_GotoLowerThird,
	INPUT_GotoBreak,
	INPUT_GotoWheel,
	INPUT_GotoOutro,

	INPUT_CrtPowerToggle
};

#endif // pch_h__
