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

	INPUT_CrtPowerToggle,
	INPUT_CrtChannelUp,
	INPUT_CrtChannelDown,

	INPUT_Dance,
	INPUT_DanceAlt,
	INPUT_DanceStop,
	INPUT_DanceShimmy
};

#endif // pch_h__
