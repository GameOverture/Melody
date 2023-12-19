#ifndef FightStick_h__
#define FightStick_h__

#include "pch.h"

#define DISPLAYORDER_GatePath -100
#define DISPLAYORDER_Gate -110

enum JSGatePoint
{
	JSGATE_Neutral = 0,
	JSGATE_Up,
	JSGATE_Down,
	JSGATE_Left,
	JSGATE_Right,
	JSGATE_UpLeft,
	JSGATE_UpRight,
	JSGATE_DownLeft,
	JSGATE_DownRight,

	NUM_JSGATES
};
const glm::vec2 JSGatePositions[NUM_JSGATES] = { {0.0f, 0.0f},
												 {0.0f, 85.0f},
												 {0.0f, -85.0f},
												 {-85.0f, 0.0f},
												 {85.0f, 0.0f},
												 {-75.0f, 75.0f},
												 {75.0f, 75.0f},
												 {-75.0f, -75.0f},
												 {75.0f, -75.0f} };
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

class FightStick : public HyEntity2d
{
	class GatePath : public HyEntity2d
	{
		HyPrimitive2d		m_Path;
		HyPrimitive2d		m_StartPoint;
		HyPrimitive2d		m_EndPoint;

	public:
		GatePath(JSGatePoint eStart, JSGatePoint eEnd, HyEntity2d *pParent) :
			HyEntity2d(pParent),
			m_Path(this),
			m_StartPoint(this),
			m_EndPoint(this)
		{
			HyAssert(eStart != eEnd, "Joystick::GatePath::GatePath() - eStart == eEnd");
			m_Path.SetAsLineSegment(JSGatePositions[eStart], JSGatePositions[eEnd]);
			m_Path.SetTint(HyColor::Red);
			m_Path.SetLineThickness(12.0f);

			m_StartPoint.SetAsCircle(JSGatePositions[eStart], 16.0f);
			m_StartPoint.SetTint(HyColor::Red);

			m_EndPoint.SetAsCircle(JSGatePositions[eEnd], 16.0f);
			m_EndPoint.SetTint(HyColor::Red);

			alpha.Tween(0.0f, 0.1f, HyTween::Linear, 0.25f);
			SetDisplayOrder(DISPLAYORDER_GatePath);
		}
	};
	// Joystick
	std::deque<GatePath *>		m_PathList;
	HySprite2d					m_Gate;
	HySprite2d					m_BallFollow;
	HySprite2d					m_Ball;
	HySprite2d					m_ButtonOverlays[NUM_JSGATES];
	JSGatePoint					m_eOldBallPos;

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
	FightStick(HyEntity2d *pParent = nullptr);
	virtual ~FightStick();

	void SetButtonPress(ButtonState eButtonState, bool bPressed);

protected:
	virtual void OnUpdate() override;

	virtual void OnMouseClicked() override;
};

#endif // FightStick_h__
