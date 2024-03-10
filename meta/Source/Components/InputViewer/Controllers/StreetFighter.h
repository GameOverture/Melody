#ifndef StreetFighter_h__
#define StreetFighter_h__

#include "pch.h"
#include "IController.h"

enum ButtonState
{
	BTNSTATE_LP = 0,
	BTNSTATE_MP,
	BTNSTATE_HP,
	BTNSTATE_LK,
	BTNSTATE_MK,
	BTNSTATE_HK
};
enum ButtonFlag
{
	BTNFLAG_LK = 0x01,
	BTNFLAG_MK = 0x02,
	BTNFLAG_HK = 0x04,
	BTNFLAG_LP = 0x08,
	BTNFLAG_MP = 0x10,
	BTNFLAG_HP = 0x20
};

class StreetFighter : public IController
{
	// Buttons
	HyPrimitive2d				m_DriveImpact;
	HyPrimitive2d				m_DriveParry;
	HySprite2d					m_ButtonLP;
	HySprite2d					m_ButtonMP;
	HySprite2d					m_ButtonHP;
	HySprite2d					m_ButtonLK;
	HySprite2d					m_ButtonMK;
	HySprite2d					m_ButtonHK;
	uint32						m_uiButtonFlags;
	
	// Assign new controller overlay
	HyPrimitive2d				m_AssignOverlayBG;
	HyText2d					m_AssignOverlayText;
	HyControllerInputFunc		m_fpAssignControllerFunc;

public:
	StreetFighter(HyEntity2d *pParent = nullptr);
	virtual ~StreetFighter();

	virtual void ApplyInputs() override;

protected:
	virtual void OnMouseClicked() override;
};

#endif // StreetFighter_h__
