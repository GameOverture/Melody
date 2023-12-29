#ifndef Wheel_h__
#define Wheel_h__

#include "pch.h"
#include "IComponent.h"
#include "WheelInit.h"
#include "WeightTable.h"

typedef HyEntityLeaf2d<HySprite2d> Wedge;

class Wheel : public IComponent
{
	class SpinEnt : public HyEntity2d
	{
	public:
		HySprite2d									m_Frame;
		std::vector<HyEntityLeaf2d<HySprite2d> *>	m_WedgeList;
		std::vector<float>							m_WedgeAngleList;
		std::vector<HySprite2d *>					m_WedgeDecalList;
		std::map<uint32, std::vector<HyText2d *>>	m_WedgeTextMap;
		HySprite2d									m_Notches;

		SpinEnt(const WheelInit &initStructRef);
		virtual ~SpinEnt();
	};
	HySprite2d										m_Base;
	SpinEnt											m_SpinEnt;
	HySprite2d										m_Hub;
	HyEntityLeaf2d<HySprite2d>						m_Ticker;

	float											m_fTickerNotchHalfWidthDegs;
	float											m_fTickerTiltAmtDeg;
	float											m_fTickerLengthDeg;

	enum TickerState
	{
		TICKERSTATE_Idle = 0,
		TICKERSTATE_Left,
		TICKERSTATE_Right
	};
	TickerState										m_eTickerState;
	float											m_fTickerPos;
	float											m_fTickerNotchPercentage;	// Starts at 0.0 and goes to 1.0 as wheel stops. Amount the ticker holds onto the notch, which only looks good when stopping.

	const WeightTable *								m_pWeightTable;

	bool											m_bSpinClockwise;		// The desired spin direction
	bool											m_bIsClockwise;			// The current spin direction, which may be different than 'm_bSpinClockwise' if moved manually or in some states like windup

	float											m_fLastDegLocation;

	float											m_fSpinWindUpAmt;		// How many degrees to pull back prior to spin
	float											m_fSpinWindUpDuration;	// How long the pull back during the wind-up is
	float											m_fSpinStartDuration;	// How long from the pull back, to reaching max velocity
	float											m_fSpinSpeed;			// Starting max-speed in degrees per second. Has minor affect on total duration of spin
	float											m_fSpinDuration;		// How long the wheel spins at full velocity
	float											m_fSpinDegsToStop;		// How many degrees it takes for wheel to come to a stop from its max speed

	float											m_fElapsedTime;
	float											m_fDegsLeftForStop;

#if LG_USE_XACT
	int												m_iSndID_Ticker;
#endif

	enum State
	{
		STATE_Idle = 0,
		STATE_StartWindUp,
		STATE_WindingUp,
		STATE_StartSpin,
		STATE_Spinning,
		STATE_PrepStop,	// Spins to the ideal location to begin stopping
		STATE_Stopping,
		STATE_FlingBack
	};
	State											m_eState;

	bool											m_bForceContinueSpin;
	bool											m_bSlammed;
	uint32											m_uiCurIndex;
	uint32											m_uiDestinationIndex;

	std::function<void(Wheel *, void *)>			m_fpTickerFlingCallback;
	void *m_pTickerFlingCallbackParam;

public:
	Wheel(const WheelInit &initStructRef, HyEntity2d *pParent = nullptr);
	virtual ~Wheel();

	virtual void Show(float fDuration) override;
	virtual void Hide(float fDuration) override;

	void NudgeCounterClockwise(float fDegrees);
	void NudgeClockwise(float fDegrees);

	Wedge &GetWedge(uint32 uiWedgeIndex);
	HySprite2d &GetWedgeDecal(uint32 uiWedgeIndex);
	HyEntityLeaf2d<HySprite2d> &GetTicker();
	HySprite2d &GetNotches();
	HySprite2d &GetHub();
	HySprite2d &GetFrame();

	std::vector<HyText2d *> SetTextOnWedge(uint32 uiWedgeIndex, std::string sText, std::string sPrefix, std::string sName, bool bVerticalText, float fEdgeMargin, float fVerticalScaleOffset = 0.12f, float fVerticalPosOffset = 0.0f);

	bool IsIdle();
	bool IsStopping();
	uint32 GetNumWedges();

	void ForceContinueSpin(bool bForce);

	uint32 GetCurrentIndex();

	HyAnimFloat &GetRotation();

	const WeightTable *GetWeightTable();
	int GetWedgeValue(uint32 uiWedgeIndex);
	float GetWedgeAngle(uint32 uiWedgeIndex);

	void Reset(uint32 uiWedgeIndex);

	uint32 GetDestinationIndex();
	void PrepSpin(uint32 uiDestinationWedgeIndex);
	bool BeginSpin(bool bDoWindUp = true);

#if LG_USE_XACT
	void SetSoundTickerID(int iSndTickID) { m_iSndID_Ticker = iSndTickID; }
#endif

	void SetTickerFlingCallback(std::function<void(Wheel *, void *)> func, void *pParam);

	void Slam();

protected:
	virtual void OnUpdate() override;

	void NormalizeRotation(float &fRotOut);
};

#endif // Wheel_h__
