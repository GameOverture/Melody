/********************************************************************************
** Auto generated class by Harmony Engine - Editor Tool
**
** WARNING! All changes made in this file will be lost when resaving the entity!
********************************************************************************/
#ifndef hy_Television_h__
#define hy_Television_h__

#include "pch.h"

namespace hy {

// State 0:  

class Television : public HyEntity2d
{
protected:
	const float					m_fTIMELINE_FRAME_DURATION;
	std::function<void()>		m_fpTimelineUpdate;
	float						m_fTimelineFrameTime;
	uint32						m_uiTimelineFrame;
	bool						m_bTimelinePaused;
	uint32						m_uiTimelineFinalFrame;

	HyTexturedQuad2d			m_LK_Released;
	HyTexturedQuad2d			m_HK_Pressed;

public:
	Television(HyEntity2d *pParent = nullptr);
	virtual ~Television();

	using IHyLoadable::GetState;
	virtual bool SetState(uint32 uiStateIndex) override;
	virtual uint32 GetNumStates() override;

	float GetTimelineFrameDuration() const;
	uint32 GetTimelineFrame() const;

	void SetTimelineFrame(uint32 uiFrameIndex);
	void ExtrapolateTimelineFrame(uint32 uiFrameIndex);

	bool IsTimelinePaused() const;
	void SetTimelinePause(bool bPause);

	bool IsTimelineFinished() const;
	
	HyTexturedQuad2d &GetLK_Released();
	HyTexturedQuad2d &GetHK_Pressed();

protected:
	virtual void Update() override;
	
};

} // 'hy' namespace

#endif // hy_Television_h__
