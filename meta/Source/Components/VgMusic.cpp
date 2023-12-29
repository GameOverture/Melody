#include "pch.h"
#include "VgMusic.h"

VgMusic::VgMusic(HyEntity2d *pParent /*= nullptr*/) :
	HyEntity2d(pParent),
	m_fAudioVolume(0.4f),
	m_iCurrTrackIndex(-1),
	m_ePlayState(PLAYSTATE_Stopped),
	m_fpOnTrackChange(nullptr),
	m_fpOnFadeOut(nullptr)
{
	std::vector<std::string> tempDirFileList;
	// Nintendo Entertainment System
	tempDirFileList = HyIO::GetFileList("\\\\IronMountain/Documents/Video Games Meta-Scrape/NES/media/Named_Ost", ".ogg", false);
	m_MusicFileList.insert(std::end(m_MusicFileList), std::begin(tempDirFileList), std::end(tempDirFileList));

	// Super Nintendo
	tempDirFileList = HyIO::GetFileList("\\\\IronMountain/Documents/Video Games Meta-Scrape/SNES/media/Named_Ost", ".ogg", false);
	m_MusicFileList.insert(std::end(m_MusicFileList), std::begin(tempDirFileList), std::end(tempDirFileList));

	// Genesis
	tempDirFileList = HyIO::GetFileList("\\\\IronMountain/Documents/Video Games Meta-Scrape/Genesis/media/Named_Ost", ".ogg", false);
	m_MusicFileList.insert(std::end(m_MusicFileList), std::begin(tempDirFileList), std::end(tempDirFileList));

	// N64
	tempDirFileList = HyIO::GetFileList("\\\\IronMountain/Documents/Video Games Meta-Scrape/N64/media/Named_Ost", ".ogg", false);
	m_MusicFileList.insert(std::end(m_MusicFileList), std::begin(tempDirFileList), std::end(tempDirFileList));

	// Dreamcast
	tempDirFileList = HyIO::GetFileList("\\\\IronMountain/Documents/Video Games Meta-Scrape/Dreamcast/media/Named_Ost", ".ogg", false);
	m_MusicFileList.insert(std::end(m_MusicFileList), std::begin(tempDirFileList), std::end(tempDirFileList));

	HyRand::Shuffle(m_MusicFileList);
}

/*virtual*/ VgMusic::~VgMusic()
{
}

void VgMusic::SetOnTrackChangeCallback(std::function<void(const std::string &)> fpOnTrackChange)
{
	m_fpOnTrackChange = fpOnTrackChange;
}

void VgMusic::SetOnFadeOutCallback(std::function<void(float)> fpOnFadeOut)
{
	m_fpOnFadeOut = fpOnFadeOut;
}

PlayState VgMusic::GetPlayState() const
{
	return m_ePlayState;
}

float VgMusic::GetElapsedPlayTime() const
{
	return m_AudioTrack.GetElapsedPlayTime();
}

/*virtual*/ void VgMusic::OnUpdate() /*override*/
{
	if(HyEngine::Input().IsActionReleased(INPUT_VgMusicPlay))
	{
		FadeOutTrack(0.5f);
		m_ePlayState = PLAYSTATE_FadeOutToNext;
	}

	if(HyEngine::Input().IsActionReleased(INPUT_VgMusicStop))
	{
		FadeOutTrack(VGMUSIC_STOP_FADEOUT_DUR);
		m_ePlayState = PLAYSTATE_FadeOutToStop;
	}

	if(HyEngine::Input().IsActionReleased(INPUT_VgMusicVolumeDown))
	{
		m_fAudioVolume = HyMath::Clamp(m_fAudioVolume - 0.1f, 0.0f, 1.0f);
		if(m_AudioTrack.volume.IsAnimating() == false)
			m_AudioTrack.volume.Set(m_fAudioVolume);
	}
	if(HyEngine::Input().IsActionReleased(INPUT_VgMusicVolumeUp))
	{
		m_fAudioVolume = HyMath::Clamp(m_fAudioVolume + 0.1f, 0.0f, 1.0f);
		if(m_AudioTrack.volume.IsAnimating() == false)
			m_AudioTrack.volume.Set(m_fAudioVolume);
	}

	switch(m_ePlayState)
	{
	case PLAYSTATE_Stopped:
		break;

	case PLAYSTATE_Starting:
		if(m_AudioTrack.IsPlaying())
			m_ePlayState = PLAYSTATE_Playing;
		break;

	case PLAYSTATE_Playing:
		if(m_AudioTrack.IsPlaying() == false || m_AudioTrack.GetElapsedPlayTime() >= VGMUSIC_MAX_SONG_LENGTH)
		{
			FadeOutTrack(VGMUSIC_NEXT_FADEOUT_DUR);
			m_ePlayState = PLAYSTATE_FadeOutToNext;
		}
		break;

	case PLAYSTATE_FadeOutToNext:
		if(m_AudioTrack.volume.IsAnimating() == false)
		{
			m_iCurrTrackIndex = (m_iCurrTrackIndex + 1) % m_MusicFileList.size();
			const std::string sMusicFile = m_MusicFileList[m_iCurrTrackIndex];

			m_AudioTrack.Init(sMusicFile, true, 0, 0, this);
			m_AudioTrack.volume.Set(m_fAudioVolume);
			m_AudioTrack.Play();
			m_AudioTrack.Load();

			if(m_fpOnTrackChange)
				m_fpOnTrackChange(sMusicFile);

			m_ePlayState = PLAYSTATE_Starting;
		}
		break;

	case PLAYSTATE_FadeOutToStop:
		if(m_AudioTrack.volume.IsAnimating() == false)
		{
			m_AudioTrack.Stop();
			m_ePlayState = PLAYSTATE_Stopped;
		}
		break;
	}
}

void VgMusic::FadeOutTrack(float fFadeOutDuration)
{
	m_AudioTrack.volume.Set(m_fAudioVolume);
	m_AudioTrack.volume.Tween(0.0f, fFadeOutDuration);

	if(m_fpOnFadeOut)
		m_fpOnFadeOut(fFadeOutDuration);
}
