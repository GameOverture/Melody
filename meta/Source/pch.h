#ifndef pch_h__
#define pch_h__

#include "HyEngine.h"

#define DISPLAYORDER_DEBUG			9999
#define DISPLAYORDER_MessageCycle	999
#define DISPLAYORDER_CrtVolume		99
#define DISPLAYORDER_Buttons		99
#define DISPLAYORDER_BallTop		85
#define DISPLAYORDER_GatePath		80
#define DISPLAYORDER_Gate			70

#define DISPLAYORDER_CrtStatic		60

#define CRT_SCREEN_WIDTH 851
#define CRT_SCREEN_HEIGHT 613

#define LOWERTHIRD_HEIGHT 110.0f

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
