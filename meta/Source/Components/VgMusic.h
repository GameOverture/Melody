#ifndef VgMusic_h__
#define VgMusic_h__

#include "pch.h"
#include "Channel.h"

#define VGMUSIC_NEXT_FADEOUT_DUR 5.0f
#define VGMUSIC_STOP_FADEOUT_DUR 0.5f
#define VGMUSIC_MAX_SONG_LENGTH 90.0f//115.0f

class CtrlPanel;

enum PlayState
{
	PLAYSTATE_Stopped,
	PLAYSTATE_Starting,
	PLAYSTATE_Playing,
	PLAYSTATE_FadeOutToNext,
	PLAYSTATE_FadeOutToStop
};

class VgMusic : public HyEntity2d
{
	HyButton									m_CtrlPanel_PrevBtn;
	HyButton									m_CtrlPanel_PlayBtn;
	HyButton									m_CtrlPanel_StopBtn;

	HyAudio2d									m_AudioTrack;

	std::vector<std::string>					m_MusicFileList;
	int32										m_iCurrTrackIndex;

	PlayState									m_ePlayState;

	std::function<void(const std::string &)>	m_fpOnTrackChange;
	std::function<void(float)>					m_fpOnFadeOut;

public:
	VgMusic(HyEntity2d *pParent = nullptr);
	virtual ~VgMusic();

	void PopulateCtrlPanel(CtrlPanel &ctrlPanel);

	void SetOnTrackChangeCallback(std::function<void(const std::string &)> fpOnTrackChange);
	void SetOnFadeOutCallback(std::function<void(float)> fpOnFadeOut);

	PlayState GetPlayState() const;
	float GetElapsedPlayTime() const;

	void Prev();
	void Play();
	void Stop();

protected:
	virtual void OnUpdate() override;

	void FadeOutTrack(float fFadeOutDuration);
};

#endif // VgMusic_h__
