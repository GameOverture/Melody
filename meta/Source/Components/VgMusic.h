#ifndef VgMusic_h__
#define VgMusic_h__

#include "pch.h"
#include "Channel.h"
#include "Crt.h"

class LargePlayer : public Channel
{
	HySprite2d					m_AudioVisualizer;
	HyTexturedQuad2d			m_Snapshot;
	HyTexturedQuad2d			m_Title;
	HyTexturedQuad2d			m_BoxArt;
	HySprite2d					m_NowPlayingSound;
	HyText2d					m_TitleText;
	HyText2d					m_TrackText;

	HyPrimitive2d				m_Debugbox;

public:
	LargePlayer(HyEntity2d *pParent);
	virtual ~LargePlayer();

	void InitNextTrack(const std::string &sMusicFile);

	void ShowVisualizer(float fFadeInTime);
	void ShowIntroTitle(float fFadeInTime);
	void ShowNowPlaying(float fFadeInTime);
	void CycleBoxArt(float fFadeInTime);
	void CycleTitleAndSnapshot(float fFadeInTime);
	void FadeOut(float fFadeOutTime);

protected:
	void TransformTexture(HyTexturedQuad2d &quadRef, glm::ivec2 vMaxSize, glm::vec2 ptCenter);
};

class VgMusic : public HyEntity2d
{
	LargePlayer					m_LargePlayer;
	HyTimer						m_LargeCycleTimer;

	float 						m_fAudioVolume;
	HyAudio2d					m_AudioTrack;

	std::vector<std::string>	m_MusicFileList;
	int32						m_iCurrTrackIndex;

	enum LargeState
	{
		LARGESTATE_Hidden,
		LARGESTATE_Stopped,

		LARGESTATE_Intro,
		LARGESTATE_IntroTitle,
		LARGESTATE_NowPlaying,
		LARGESTATE_CycleBoxArt,
		LARGESTATE_CycleSnapshot,

		LARGESTATE_Hide
	};
	LargeState					m_eLargeState;

	enum SmallState
	{
		SMALLSTATE_Hidden,
		SMALLSTATE_Stopped,

		SMALLSTATE_Hide
	};
	SmallState					m_eSmallState;

	enum PlayState
	{
		PLAYSTATE_Stopped,
		PLAYSTATE_Starting,
		PLAYSTATE_Playing,
		PLAYSTATE_FadeOutToNext,
		PLAYSTATE_FadeOutToStop
	};
	PlayState					m_ePlayState;

public:
	VgMusic(Crt &crtRef, HyEntity2d *pParent = nullptr);
	virtual ~VgMusic();

	bool IsLargeShowing() const;
	void ShowLarge(bool bEnable);
	bool IsSmallShowing() const;
	void ShowSmall(bool bEnable);

	LargePlayer &GetLargePlayer();

protected:
	virtual void OnUpdate() override;

	void FadeOutTrack(float fFadeOutDuration);

	void FitLargeTextureCentered(HyTexturedQuad2d &textureRef);
};

#endif // VgMusic_h__
