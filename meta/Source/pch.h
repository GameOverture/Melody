#ifndef pch_h__
#define pch_h__

#include "HyEngine.h"

#define DISPLAYORDER_DEBUG			99999
#define DISPLAYORDER_MessageCycle	9999
#define DISPLAYORDER_LiveSplitMask	999
#define DISPLAYORDER_CrtVolume		99
#define DISPLAYORDER_Buttons		99
#define DISPLAYORDER_BallTop		85
#define DISPLAYORDER_GatePath		80
#define DISPLAYORDER_Gate			70

#define DISPLAYORDER_CrtStatic		60


#define CAMERA_CENTER_POS 0.0f, 0.0f
#define CAMERA_CENTER_SCALE 1.0f, 1.0f
#define CAMERA_DIVIDER_POS -RETRO_SIDES, 0
#define CAMERA_DIVIDER_SCALE 1.0f, 1.0f
#define CAMERA_GAME_POS -293.0f, -13.0f
#define CAMERA_GAME_SCALE 0.67f, 0.67f
enum CameraTag
{
	CAMTAG_Center = 0,
	CAMTAG_Divider,
	CAMTAG_Game
};

#define LIVESPLIT_WIDTH 544
#define LIVESPLIT_HEIGHT 768

#define MISC_WIDTH 544
#define MISC_HEIGHT 312

#define DIVIDER_WIDTH 4

#define RETRO_SIDES 274
#define CRT_SCREEN_WIDTH 855
#define CRT_SCREEN_HEIGHT 601

#define LOWERTHIRD_HEIGHT 172

enum InputAction
{
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

enum InputController
{
	INPUTCONTROLLER_StreetFighter = 0,
	INPUTCONTROLLER_NES,
	INPUTCONTROLLER_StarCitizen,

	NUM_INPUTCONTROLLERS
};

enum ComponentType
{
	COMPONENT_Brb = 0,
	COMPONENT_InputViewer,
	COMPONENT_Crt,
	COMPONENT_HeartBeat,
	COMPONENT_Wheel,
	COMPONENT_MessageCycle,

	NUM_COMPONENTS
};

#endif // pch_h__
