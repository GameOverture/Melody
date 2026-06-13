#ifndef GuiltyGear_h__
#define GuiltyGear_h__

#include "pch.h"
#include "IController.h"
#include "Dpad.h"

class GuiltyGear : public IController
{
	Dpad						m_Dpad;

	HySprite2d					m_ButtonP;
	HySprite2d					m_ButtonK;
	HySprite2d					m_ButtonS;
	HySprite2d					m_ButtonHS;
	HySprite2d					m_ButtonDU;
	HySprite2d					m_ButtonDA;
	//HySprite2d					m_ButtonFD;
	uint32						m_uiButtonFlags;

	// Assign new controller overlay
	HyPrimitive2d				m_AssignOverlayBG;
	HyText2d					m_AssignOverlayText;
	HyControllerInputFunc		m_fpAssignControllerFunc;

	enum ButtonState
	{
		BTNSTATE_P = 0,
		BTNSTATE_K,
		BTNSTATE_S,
		BTNSTATE_HS,
		BTNSTATE_DU,
		BTNSTATE_DA,
		BTNSTATE_FD
	};
	enum ButtonFlag
	{
		BTNFLAG_P = 0x01,
		BTNFLAG_K = 0x02,
		BTNFLAG_S = 0x04,
		BTNFLAG_HS = 0x08,
		BTNFLAG_DU = 0x10,
		BTNFLAG_DA = 0x20,
		//BTNFLAG_FD = 0x40
	};

public:
	GuiltyGear(HyEntity2d *pParent = nullptr);
	virtual ~GuiltyGear();

	virtual void ApplyInputs() override;

protected:
	virtual void OnMouseClicked() override;
};

#endif // GuiltyGear_h__
