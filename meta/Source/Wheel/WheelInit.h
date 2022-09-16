#ifndef WheelInit_h__
#define WheelInit_h__

#include "pch.h"
#include "WeightTable.h"

struct LoadPath
{
	std::string		m_sPrefix;
	std::string		m_sName;

	LoadPath()
	{ }

	bool IsUsed() const {
		return m_sName.empty() == false;
	}

	void Set(const char *szPre, const char *szNam) {
		m_sPrefix = szPre == nullptr ? "" : szPre;
		m_sName = szNam == nullptr ? "" : szNam;
	}

	void Clear() {
		m_sName.clear();
		m_sPrefix.clear();
	}
};

struct WheelInit
{
	LoadPath					base;
	LoadPath					wedges;
	LoadPath					wedgeDecal;
	LoadPath					frame;
	LoadPath					notches;
	LoadPath					hub;
	LoadPath					ticker;
	const WeightTable *			pWedgeWeightTable;
	std::vector<float>			wedgeDegreeThicknessList;	// How large each wedge is (leave empty if all wedges are uniform)
	glm::ivec2					tickerOffset;				// Position offset from center of wheel
	float						tickerNotchWidthDegs;
	float						tickerDegTiltAmt;
	float						tickerLengthDeg;
	float						spinWindUpAmt;
	float						spinWindUpDuration;
	float						spinStartDuration;
	float						spinSpeed;
	float						spinDuration;
	float						spinDegsToStop;

	WheelInit() :
		pWedgeWeightTable(nullptr),
		tickerNotchWidthDegs(5.0f),
		tickerDegTiltAmt(35.0f),
		tickerLengthDeg(5.0f),
		spinWindUpAmt(30.0f),
		spinWindUpDuration(0.75f),
		spinStartDuration(0.5f),
		spinSpeed(720.0f),
		spinDuration(2.0f),
		spinDegsToStop(540.0f)
	{ }

	void SetTicker(std::string sPrefix, std::string sName, int32 iTickerOffsetX, int32 iTickerOffsetY, float fNotchWidthInDegs = 5.0f, float fTickerLengthInDegs = 5.0f, float fTickerTiltAmtDeg = 80.0f)
	{
		ticker.Set(sPrefix.c_str(), sName.c_str());
		HySetVec(tickerOffset, iTickerOffsetX, iTickerOffsetY);
		tickerNotchWidthDegs = fNotchWidthInDegs;
		tickerLengthDeg = fTickerLengthInDegs;
		tickerDegTiltAmt = fTickerTiltAmtDeg;
	}

	void SetWeightTable(const WeightTable *pWeightTable)
	{
		pWedgeWeightTable = pWeightTable;
	}

	void SetSpinAttributes(float fWindUpAmtInDeg, float fWindUpDurationInSec, float fStartDurationInSec, float fSpinSpeedInDegPerSec, float fSpinDurationInSec, float fDegreesToStop)
	{
		spinWindUpAmt = fWindUpAmtInDeg;
		spinWindUpDuration = fWindUpDurationInSec;
		spinStartDuration = fStartDurationInSec;
		spinSpeed = fSpinSpeedInDegPerSec;
		spinDuration = fSpinDurationInSec;
		spinDegsToStop = fDegreesToStop;
	}
};

#endif // WheelInit_h__
