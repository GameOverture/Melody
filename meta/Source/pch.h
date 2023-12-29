#ifndef pch_h__
#define pch_h__

#include "HyEngine.h"

#define DISPLAYORDER_FIGHTSTICK		999999
#define DISPLAYORDER_GatePath		999980
#define DISPLAYORDER_Gate			999970

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
	INPUT_DanceShimmy,

	INPUT_VgMusicPlay, // Also skips current track if already playing
	INPUT_VgMusicStop,
	INPUT_VgMusicVolumeDown,
	INPUT_VgMusicVolumeUp,

	FIGHTSTICK_LK,
	FIGHTSTICK_MK,
	//FIGHTSTICK_HK, // Trigger Positive
	FIGHTSTICK_LP,
	FIGHTSTICK_MP,
	FIGHTSTICK_HP,
	FIGHTSTICK_Start,
	FIGHTSTICK_Select,
	FIGHTSTICK_Up,
	FIGHTSTICK_Down,
	FIGHTSTICK_Left,
	FIGHTSTICK_Right,
	FIGHTSTICK_DriveImpact,
	FIGHTSTICK_Parry
};

#endif // pch_h__
