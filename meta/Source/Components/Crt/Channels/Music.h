#ifndef Music_h__
#define Music_h__

#include "pch.h"
#include "Channel.h"

#define NUM_DANCERS 6

class VgMusic;

enum DanceState
{
	DANCESTATE_Stop,
	DANCESTATE_Shimmy,
	DANCESTATE_Dance,
	DANCESTATE_DanceAlt,
};

class Dancer : public HyEntityWrapper2d<HySprite2d>
{
	DanceState			m_eDanceState;
	float				m_fElapsedTime;

	HyTimer				m_DeferTimer;
	DanceState			m_eDeferDanceState;

public:
	Dancer(HyEntity2d *pParent = nullptr);

	DanceState GetDanceState() const;

	void Dance();
	void DanceAlt();
	void Stop();
	void Shimmy();

	void DeferDance(DanceState eDanceState, float fDelay);

protected:
	virtual void OnUpdate() override;
};

class Music : public Channel
{
	VgMusic &					m_VgMusicRef;

	HySprite2d					m_AudioVisualizer;
	HyTexturedQuad2d			m_Snapshot;
	HyTexturedQuad2d			m_Title;
	HyTexturedQuad2d			m_BoxArt;

	Dancer						m_Dancers[NUM_DANCERS];

	HySprite2d					m_NowPlayingSound;
	HyText2d					m_TitleText;
	HyText2d					m_TrackText;
	HyText2d					m_ComposerText;

	enum LargeState
	{
		LARGESTATE_Stopped,

		LARGESTATE_Intro,
		LARGESTATE_IntroTitle,
		LARGESTATE_NowPlaying,
		LARGESTATE_CycleBoxArt,
		LARGESTATE_CycleSnapshot,
	};
	LargeState					m_eLargeState;
	HyTimer						m_LargeCycleTimer;

	//HyPrimitive2d				m_Debugbox;

public:
	Music(VgMusic &vgMusicRef, HyEntity2d *pParent = nullptr);
	virtual ~Music();

	void InitNextTrack(const std::string &sMusicFile);

	void ShowVisualizer(float fFadeInTime);
	void ShowIntroTitle(float fFadeInTime);
	void ShowNowPlaying(float fFadeInTime);
	void CycleBoxArt(float fFadeInTime);
	void CycleTitleAndSnapshot(float fFadeInTime);
	void FadeOut(float fFadeOutTime);

protected:
	virtual void OnUpdate() override;

	void TransformTexture(HyTexturedQuad2d &quadRef, glm::ivec2 vMaxSize, glm::vec2 ptCenter);
};

#endif // Music_h__
