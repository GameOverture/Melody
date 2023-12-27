#ifndef VgMusic_h__
#define VgMusic_h__

#include "pch.h"

class VgMusic : public HyEntity2d
{
	class LargePlayer : public HyEntity2d
	{
		HySprite2d					m_AudioVisualizer;
		HyText2d					m_TitleText;
		HyText2d					m_TrackText;
		HyTexturedQuad2d			m_Snapshot;
		HyTexturedQuad2d			m_Title;
		HyTexturedQuad2d			m_BoxArt;

		HyPrimitive2d				m_Debugbox;

	public:
		LargePlayer(HyEntity2d *pParent);
		virtual ~LargePlayer();

		void InitNextTrack(const std::string &sMusicFile);

		void ShowVisualizer(float fFadeInTime);
		void ShowSnapshot(float fFadeInTime);
		void ShowNowPlaying(float fFadeInTime);
		void CycleBoxArt(float fFadeInTime);
		void CycleTitleAndSnapshot(float fFadeInTime);
		void FadeOut(float fFadeOutTime);
	};
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
		LARGESTATE_Snapshot,
		LARGESTATE_Title,
		LARGESTATE_CycleBoxArt,
		LARGESTATE_CycleSnapshot,
		LARGESTATE_FadeOut,
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

public:
	VgMusic(HyEntity2d *pParent = nullptr);
	virtual ~VgMusic();

	bool IsLargeShowing() const;
	void ShowLarge(bool bEnable);
	bool IsSmallShowing() const;
	void ShowSmall(bool bEnable);

protected:
	virtual void OnUpdate() override;

	void NextTrack();

	void FitLargeTextureCentered(HyTexturedQuad2d &textureRef);
};

#endif // VgMusic_h__
