/********************************************************************************
** Auto generated class by Harmony Engine - Editor Tool
**
** WARNING! All changes made in this file will be lost when resaving the entity!
********************************************************************************/
#include "pch.h"
#include "hy_MusicPlayer.h"

namespace hy {

MusicPlayer::MusicPlayer(HyEntity2d *pParent /*= nullptr*/) :
	HyEntity2d(pParent),
	m_fTIMELINE_FRAME_DURATION(0.016667f),
	m_fpTimelineUpdate(nullptr),
	m_fTimelineFrameTime(0.0f),
	m_uiTimelineFrame(0),
	m_bTimelinePaused(false),
	m_uiTimelineFinalFrame(0),
	m_NowPlayingSound(HyNodePath("VgMusic/NowPlayingSound"), this)
{
	SetState(0);
}

/*virtual*/ MusicPlayer::~MusicPlayer()
{
}

/*virtual*/ bool MusicPlayer::SetState(uint32 uiStateIndex)
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
				m_NowPlayingSound.pos.Set(250.000000f, -120.000000f);
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

/*virtual*/ uint32 MusicPlayer::GetNumStates()
{
	return 1;
}

float MusicPlayer::GetTimelineFrameDuration() const
{
	return m_fTIMELINE_FRAME_DURATION;
}

uint32 MusicPlayer::GetTimelineFrame() const
{
	return m_uiTimelineFrame;
}

void MusicPlayer::SetTimelineFrame(uint32 uiFrameIndex)
{
	m_uiTimelineFrame = HyMath::Min(uiFrameIndex, m_uiTimelineFinalFrame);
	m_fpTimelineUpdate();
	m_fTimelineFrameTime = -m_fTIMELINE_FRAME_DURATION;
}

void MusicPlayer::ExtrapolateTimelineFrame(uint32 uiFrameIndex)
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
		m_NowPlayingSound.AdvanceAnim(m_fTIMELINE_FRAME_DURATION);

		
		m_uiTimelineFrame++;
	}
	
	m_fTimelineFrameTime = -m_fTIMELINE_FRAME_DURATION;
}

bool MusicPlayer::IsTimelinePaused() const
{
	return m_bTimelinePaused;
}

bool MusicPlayer::IsTimelineFinished() const
{
	return m_uiTimelineFrame == m_uiTimelineFinalFrame;
}

void MusicPlayer::SetTimelinePause(bool bPause)
{
	m_bTimelinePaused = bPause;
}


HySprite2d &MusicPlayer::GetNowPlayingSound() { return m_NowPlayingSound; }

/*virtual*/ void MusicPlayer::Update() /*override*/
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
