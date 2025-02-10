#ifndef VgMusic_h__
#define VgMusic_h__

#include "pch.h"
#include "Channel.h"

#define VGMUSIC_NEXT_FADEOUT_DUR 5.0f
#define VGMUSIC_STOP_FADEOUT_DUR 0.5f
#define VGMUSIC_MAX_SONG_LENGTH 90.0f//115.0f

class CtrlPanel;
class Crt;

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
	HyCheckBox									m_CtrlPanel_LoadCheckBox;
	HyButton									m_CtrlPanel_PrevBtn;
	HyButton									m_CtrlPanel_PlayBtn;
	HyButton									m_CtrlPanel_StopBtn;
	HyButton									m_btnVolume_Down;
	HyButton									m_btnVolume_Up;
	
	Crt *										m_pCrtRef;

	HyAudio2d									m_AudioTrack;
public:
	std::vector<MusicTrack>						m_MusicTrackList;
	int32										m_iCurrTrackIndex;

	PlayState									m_ePlayState;

	std::function<void(MusicTrack &)>			m_fpOnTrackChange;
	std::function<void(float)>					m_fpOnFadeOut;

public:
	VgMusic(HyEntity2d *pParent = nullptr);
	virtual ~VgMusic();

	void SetCrtRef(Crt *pCrtRef);
	void PopulateCtrlPanel(CtrlPanel &ctrlPanel);

	void SetOnTrackChangeCallback(std::function<void(MusicTrack &)> fpOnTrackChange);
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
