#ifndef Dpad_h__
#define Dpad_h__

#include "pch.h"

class Buttons;

enum DPadFlags
{
	DPad_Up = 0x1,
	DPad_Down = 0x2,
	DPad_Left = 0x4,
	DPad_Right = 0x8
};

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

class Dpad : public HyEntity2d
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
			HyAssert(eStart != eEnd, "GatePath::GatePath() - eStart == eEnd");
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

public:
	Dpad(const HyNodePath &gatePath, const HyNodePath &pressPath, const HyNodePath &buttonsPath, HyEntity2d *pParent = nullptr);
	virtual ~Dpad();

	void ApplyInput(uint32 uiDPadFlags);
	
	void SetButtonPress(int iButtonState, bool bPressed);

protected:
	virtual void OnUpdate() override;
};

#endif // Dpad_h__
