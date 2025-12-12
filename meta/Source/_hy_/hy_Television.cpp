/********************************************************************************
** Auto generated class by Harmony Engine - Editor Tool
**
** WARNING! All changes made in this file will be lost when resaving the entity!
********************************************************************************/
#include "pch.h"
#include "hy_Television.h"

namespace hy {

Television::Television(HyEntity2d *pParent /*= nullptr*/) :
	HyEntity2d(pParent),
	m_fTIMELINE_FRAME_DURATION(0.016667f),
	m_fpTimelineUpdate(nullptr),
	m_fTimelineFrameTime(0.0f),
	m_uiTimelineFrame(0),
	m_bTimelinePaused(false),
	m_uiTimelineFinalFrame(0),
	m_LK_Released(3547625482, 0, this),
	m_HK_Pressed(330467946, 0, this)
{
	SetState(0);
}

/*virtual*/ Television::~Television()
{
}

/*virtual*/ bool Television::SetState(uint32 uiStateIndex)
{
	if(HyEntity2d::SetState(uiStateIndex) == false || uiStateIndex >= GetNumStates())
		return false;
	
	switch(m_uiState)
	{
	case 0: // 
		m_uiTimelineFinalFrame = 0;
		m_fpTimelineUpdate = [this]()
		{
			std::vector<glm::vec2> vertList;
			switch(m_uiTimelineFrame)
			{
			default:
				break;

			case 0:
				m_HK_Pressed.pos.Set(-33.500000f, -34.000000f);
				m_LK_Released.pos.Set(60.520020f, 34.000000f);
				break;

			}
		};
		break;
	}
	
	m_uiTimelineFrame = 0;
	m_fpTimelineUpdate();
	
	m_fTimelineFrameTime = -m_fTIMELINE_FRAME_DURATION;
	
	return true;
}

/*virtual*/ uint32 Television::GetNumStates()
{
	return 1;
}

float Television::GetTimelineFrameDuration() const
{
	return m_fTIMELINE_FRAME_DURATION;
}

uint32 Television::GetTimelineFrame() const
{
	return m_uiTimelineFrame;
}

void Television::SetTimelineFrame(uint32 uiFrameIndex)
{
	m_uiTimelineFrame = HyMath::Min(uiFrameIndex, m_uiTimelineFinalFrame);
	m_fpTimelineUpdate();
	m_fTimelineFrameTime = -m_fTIMELINE_FRAME_DURATION;
}

void Television::ExtrapolateTimelineFrame(uint32 uiFrameIndex)
{
	uiFrameIndex = HyMath::Min(uiFrameIndex, m_uiTimelineFinalFrame);

	// Extrapolate to frame 'uiFrameIndex'
	m_uiTimelineFrame = 0;
	while(true)
	{
		m_fpTimelineUpdate();
		if(m_uiTimelineFrame == uiFrameIndex)
			break;
		
		// Advance timeline by 1 frame

		
		m_uiTimelineFrame++;
	}
	
	m_fTimelineFrameTime = -m_fTIMELINE_FRAME_DURATION;
}

bool Television::IsTimelinePaused() const
{
	return m_bTimelinePaused;
}

bool Television::IsTimelineFinished() const
{
	return m_uiTimelineFrame == m_uiTimelineFinalFrame;
}

void Television::SetTimelinePause(bool bPause)
{
	m_bTimelinePaused = bPause;
}


HyTexturedQuad2d &Television::GetLK_Released() { return m_LK_Released; }
HyTexturedQuad2d &Television::GetHK_Pressed() { return m_HK_Pressed; }

/*virtual*/ void Television::Update() /*override*/
{
	if(m_bTimelinePaused == false && m_uiTimelineFrame < m_uiTimelineFinalFrame)
	{
		m_fTimelineFrameTime += HyEngine::DeltaTime();
		while(m_fTimelineFrameTime >= 0.0f && m_uiTimelineFrame < m_uiTimelineFinalFrame)
		{
			m_uiTimelineFrame++;
			m_fpTimelineUpdate();
			
			m_fTimelineFrameTime -= m_fTIMELINE_FRAME_DURATION;
		}
	}
	
	HyEntity2d::Update();
}

} // 'hy' namespace
