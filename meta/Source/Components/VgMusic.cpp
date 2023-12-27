#include "pch.h"
#include "VgMusic.h"

#define LARGE_INTRO_TIME 5.0f//20.0f
#define LARGE_INTROSNAPSHOT_TIME 1.0f//5.0f
#define LARGE_INTRONOWPLAYING_TIME 1.0f//5.0f

#define LARGE_WIDTH 800.0f
#define LARGE_HEIGHT 600.0f
#define LARGE_CYCLE_DUR 5.0f
#define LARGE_FADEOUT_DUR 5.0f

#define VGMUSIC_MAX_SONG_LENGTH 20.0f//120.0f

VgMusic::LargePlayer::LargePlayer(HyEntity2d *pParent) :
	HyEntity2d(pParent),
	m_AudioVisualizer("VgMusic", "Visualizer", this),
	m_TitleText("", "MainText", this),
	m_TrackText("", "MainText", this),
	m_Snapshot(this),
	m_Title(this),
	m_BoxArt(this),
	m_Debugbox(this)
{
	m_AudioVisualizer.SetVisible(false);
	m_TitleText.SetVisible(false);
	m_TitleText.SetTextAlignment(HYALIGN_Right);
	m_TitleText.pos.Set(LARGE_WIDTH * 0.5f, 50.0f);
	m_TitleText.SetAsScaleBox(LARGE_WIDTH * 0.5f, 50.0f);
	m_TrackText.SetVisible(false);
	m_TrackText.SetTextAlignment(HYALIGN_Right);
	m_TrackText.pos.Set(LARGE_WIDTH * 0.5f, 0.0f);
	m_TrackText.SetAsScaleBox(LARGE_WIDTH * 0.5f, 50.0f);
	m_Snapshot.SetVisible(false);
	m_Title.SetVisible(false);
	m_BoxArt.SetVisible(false);

	m_Debugbox.SetWireframe(true);
	m_Debugbox.SetAsBox(LARGE_WIDTH, LARGE_HEIGHT);
}

/*virtual*/ VgMusic::LargePlayer::~LargePlayer()
{
}

void VgMusic::LargePlayer::InitNextTrack(const std::string &sMusicFile)
{
	std::string sMediaDirectory = HyIO::GetDirectoryFromPath(sMusicFile); // First strip the file name
	sMediaDirectory = HyIO::GetDirectoryFromPath(sMediaDirectory); // Then go back one directory

	std::string sMusicTrack = HyIO::GetFileNameWithoutExtension(HyIO::GetFileNameFromPath(sMusicFile));

	size_t uiDelimIndex = sMusicTrack.rfind('[', std::string::npos);
	if(uiDelimIndex == std::string::npos)
		return; // This shouldn't happen. There should always be a [...] at the end of the file name

	std::string sBaseFileName = sMusicTrack.substr(0, uiDelimIndex);
	sMusicTrack = sMusicTrack.substr(uiDelimIndex + 1, sMusicTrack.size() - uiDelimIndex - 2);

	std::string sBoxartFile = sMediaDirectory + "/Named_Boxarts/" + sBaseFileName + ".png";
	std::string sTitleFile = sMediaDirectory + "/Named_Titles/" + sBaseFileName + ".png";
	std::string sSnapshotFile = sMediaDirectory + "/Named_Snaps/" + sBaseFileName + ".png";

	// Strip any () from the end of the base file name
	size_t uiDelim = sBaseFileName.rfind('(', std::string::npos);
	if(uiDelim != std::string::npos)
		sBaseFileName = sBaseFileName.substr(0, uiDelim);
	HyIO::TrimWhitespace(sBaseFileName);
	HyIO::TrimWhitespace(sMusicTrack);
	m_TitleText.SetText(sBaseFileName);
	m_TrackText.SetText(sMusicTrack);

	m_BoxArt.Init(sBoxartFile, HyTextureInfo(), this);
	m_Title.Init(sTitleFile, HyTextureInfo(), this);
	m_Snapshot.Init(sSnapshotFile, HyTextureInfo(), this);

	m_AudioVisualizer.SetVisible(false);
	m_TitleText.SetVisible(false);
	m_TrackText.SetVisible(false);
	m_Snapshot.SetVisible(false);
	m_Title.SetVisible(false);
	m_BoxArt.SetVisible(false);
}

void VgMusic::LargePlayer::ShowVisualizer(float fFadeInTime)
{
	FadeOut(fFadeInTime);

	// Center the texture, using the bottom left corner as the anchor. Fade it in from 0.0f alpha to 1.0f alpha
	m_AudioVisualizer.scale.Set(1.0f);
	if(m_AudioVisualizer.GetWidth() > LARGE_WIDTH || m_AudioVisualizer.GetHeight() > LARGE_HEIGHT)
	{
		float fScale = std::min(LARGE_WIDTH / m_AudioVisualizer.GetWidth(), LARGE_HEIGHT / m_AudioVisualizer.GetHeight());
		m_AudioVisualizer.scale.Set(fScale);
	}
	m_AudioVisualizer.pos.Set(LARGE_WIDTH / 2.0f, LARGE_HEIGHT / 2.0f);

	m_AudioVisualizer.alpha.Set(0.0f);
	m_AudioVisualizer.alpha.Tween(1.0f, fFadeInTime);
	m_AudioVisualizer.SetVisible(true);
}

void VgMusic::LargePlayer::ShowSnapshot(float fFadeInTime)
{
	FadeOut(fFadeInTime);

	// Center the texture, using the bottom left corner as the anchor. Fade it in from 0.0f alpha to 1.0f alpha
	m_Snapshot.scale.Set(1.0f);
	if(m_Snapshot.GetWidth() > LARGE_WIDTH || m_Snapshot.GetHeight() > LARGE_HEIGHT)
	{
		float fScale = std::min(LARGE_WIDTH / m_Snapshot.GetWidth(), LARGE_HEIGHT / m_Snapshot.GetHeight());
		m_Snapshot.scale.Set(fScale);
	}
	m_Snapshot.pos.Set(LARGE_WIDTH / 2.0f - m_Snapshot.GetWidth(m_Snapshot.scale.GetX()) / 2.0f, LARGE_HEIGHT / 2.0f - m_Snapshot.GetHeight(m_Snapshot.scale.GetY()) / 2.0f);
	m_Snapshot.alpha.Set(0.0f);
	m_Snapshot.alpha.Tween(1.0f, fFadeInTime);
	m_Snapshot.SetVisible(true);
}

void VgMusic::LargePlayer::ShowNowPlaying(float fFadeInTime)
{
	m_TitleText.alpha.Set(0.0f);
	m_TitleText.alpha.Tween(1.0f, fFadeInTime);
	m_TitleText.SetVisible(true);

	m_TrackText.alpha.Set(0.0f);
	m_TrackText.alpha.Tween(1.0f, fFadeInTime);
	m_TrackText.SetVisible(true);
}

void VgMusic::LargePlayer::CycleBoxArt(float fFadeInTime)
{
	if(m_Snapshot.IsVisible())
		m_Snapshot.alpha.Tween(0.0f, fFadeInTime, HyTween::Linear, 0.0f, [](IHyNode *pThis) { pThis->SetVisible(false); });
	if(m_Title.IsVisible())
		m_Title.alpha.Tween(0.0f, fFadeInTime, HyTween::Linear, 0.0f, [](IHyNode *pThis) { pThis->SetVisible(false); });

	m_BoxArt.scale.Set(1.0f);
	if(m_BoxArt.GetWidth() > LARGE_WIDTH || m_BoxArt.GetHeight() > LARGE_HEIGHT)
	{
		float fScale = std::min(LARGE_WIDTH / m_BoxArt.GetWidth(), LARGE_HEIGHT / m_BoxArt.GetHeight());
		m_BoxArt.scale.Set(fScale);
	}
	m_BoxArt.pos.Set((LARGE_WIDTH / 2.0f) - (m_BoxArt.GetWidth(m_BoxArt.scale.GetX()) / 2.0f), (LARGE_HEIGHT / 2.0f) - (m_BoxArt.GetHeight(m_BoxArt.scale.GetY()) / 2.0f));
	m_BoxArt.alpha.Set(0.0f);
	m_BoxArt.alpha.Tween(1.0f, fFadeInTime);
	m_BoxArt.SetVisible(true);
}

void VgMusic::LargePlayer::CycleTitleAndSnapshot(float fFadeInTime)
{
	if(m_BoxArt.IsVisible())
		m_BoxArt.alpha.Tween(0.0f, fFadeInTime, HyTween::Linear, 0.0f, [](IHyNode *pThis) { pThis->SetVisible(false); });

	m_Title.scale.Set(1.0f);
	if(m_Title.GetWidth() > LARGE_WIDTH * 0.5f || m_Title.GetHeight() > LARGE_HEIGHT)
	{
		float fScale = std::min(LARGE_WIDTH * 0.5f / m_Title.GetWidth(), LARGE_HEIGHT / m_Title.GetHeight());
		m_Title.scale.Set(fScale);
	}
	m_Title.pos.Set((LARGE_WIDTH * 0.25f) - (m_Title.GetWidth(m_Title.scale.GetX()) * 0.5f), LARGE_HEIGHT / 2.0f - m_Title.GetHeight(m_Title.scale.GetY()) / 2.0f);
	m_Title.alpha.Set(0.0f);
	m_Title.alpha.Tween(1.0f, fFadeInTime);
	m_Title.SetVisible(true);

	m_Snapshot.scale.Set(1.0f);
	if(m_Snapshot.GetWidth() > LARGE_WIDTH * 0.5f || m_Snapshot.GetHeight() > LARGE_HEIGHT * 0.5f)
	{
		float fScale = std::min(LARGE_WIDTH * 0.5f / m_Snapshot.GetWidth(), LARGE_HEIGHT * 0.5f / m_Snapshot.GetHeight());
		m_Snapshot.scale.Set(fScale);
	}
	m_Snapshot.pos.Set((LARGE_WIDTH * 0.75f) - (m_Snapshot.GetWidth(m_Snapshot.scale.GetX()) * 0.5f), LARGE_HEIGHT / 2.0f - m_Snapshot.GetHeight(m_Snapshot.scale.GetY()) / 2.0f);
	m_Snapshot.alpha.Set(0.0f);
	m_Snapshot.alpha.Tween(1.0f, fFadeInTime);
	m_Snapshot.SetVisible(true);
}

void VgMusic::LargePlayer::FadeOut(float fFadeOutTime)
{
	if(m_AudioVisualizer.IsVisible())
		m_AudioVisualizer.alpha.Tween(0.0f, fFadeOutTime, HyTween::Linear, 0.0f, [](IHyNode *pThis) { pThis->SetVisible(false); });
	if(m_TitleText.IsVisible())
		m_TitleText.alpha.Tween(0.0f, fFadeOutTime, HyTween::Linear, 0.0f, [](IHyNode *pThis) { pThis->SetVisible(false); });
	if(m_TrackText.IsVisible())
		m_TrackText.alpha.Tween(0.0f, fFadeOutTime, HyTween::Linear, 0.0f, [](IHyNode *pThis) { pThis->SetVisible(false); });
	if(m_Snapshot.IsVisible())
		m_Snapshot.alpha.Tween(0.0f, fFadeOutTime, HyTween::Linear, 0.0f, [](IHyNode *pThis) { pThis->SetVisible(false); });
	if(m_Title.IsVisible())
		m_Title.alpha.Tween(0.0f, fFadeOutTime, HyTween::Linear, 0.0f, [](IHyNode *pThis) { pThis->SetVisible(false); });
	if(m_BoxArt.IsVisible())
		m_BoxArt.alpha.Tween(0.0f, fFadeOutTime, HyTween::Linear, 0.0f, [](IHyNode *pThis) { pThis->SetVisible(false); });
}

//////////////////////////////////////////////////////////////////////////

VgMusic::VgMusic(HyEntity2d *pParent /*= nullptr*/) :
	HyEntity2d(pParent),
	m_LargePlayer(this),
	m_fAudioVolume(0.8f),
	m_iCurrTrackIndex(-1),
	m_eLargeState(LARGESTATE_Hidden),
	m_eSmallState(SMALLSTATE_Hidden)
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

	ShowLarge(true);
	m_LargePlayer.pos.Set(HyEngine::Window().GetWidthF(0.5f) - (LARGE_WIDTH * 0.5f), HyEngine::Window().GetHeightF(0.5f) - (LARGE_HEIGHT * 0.5f));
}

/*virtual*/ VgMusic::~VgMusic()
{
}

bool VgMusic::IsLargeShowing() const
{
	return m_eLargeState != LARGESTATE_Hidden;
}

void VgMusic::ShowLarge(bool bEnable)
{
	if(bEnable)
	{
		if(m_eLargeState == LARGESTATE_Hidden)
			m_eLargeState = LARGESTATE_Stopped;
	}
	else
	{
		if(m_eLargeState != LARGESTATE_Hidden)
			m_eLargeState = LARGESTATE_Hide;
	}
}

bool VgMusic::IsSmallShowing() const
{
	return m_eSmallState != SMALLSTATE_Hidden;
}

void VgMusic::ShowSmall(bool bEnable)
{
	if(bEnable)
	{
		if(m_eSmallState == SMALLSTATE_Hidden)
			m_eSmallState = SMALLSTATE_Stopped;
	}
	else
	{
		if(m_eSmallState != SMALLSTATE_Hidden)
			m_eSmallState = SMALLSTATE_Hide;
	}
}

/*virtual*/ void VgMusic::OnUpdate() /*override*/
{
	if(HyEngine::Input().IsActionReleased(INPUT_VgMusicPlay))
	{
		NextTrack();
		if(IsLargeShowing())
		{
			m_LargePlayer.FadeOut(0.25f);
			m_eLargeState = LARGESTATE_Stopped;
		}
	}

	if(HyEngine::Input().IsActionReleased(INPUT_VgMusicStop))
	{
		m_AudioTrack.Stop();
		if(IsLargeShowing())
		{
			m_LargePlayer.FadeOut(0.25f);
			m_eLargeState = LARGESTATE_Stopped;
		}
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

	switch(m_eLargeState)
	{
	case LARGESTATE_Hidden:
		break;

	case LARGESTATE_Stopped:
		if(m_AudioTrack.IsPlaying())
		{
			m_LargePlayer.ShowVisualizer(0.5f);
			m_eLargeState = LARGESTATE_Intro;
		}
		break;

	case LARGESTATE_Intro:
		if(m_AudioTrack.GetElapsedPlayTime() >= LARGE_INTRO_TIME)
		{
			m_LargePlayer.ShowSnapshot(0.5f);

			m_eLargeState = LARGESTATE_Snapshot;
		}
		break;

	case LARGESTATE_Snapshot:
		if(m_AudioTrack.GetElapsedPlayTime() >= (LARGE_INTRO_TIME + LARGE_INTROSNAPSHOT_TIME))
		{
			m_LargePlayer.ShowNowPlaying(0.5f);
			m_eLargeState = LARGESTATE_Title;
		}
		break;

	case LARGESTATE_Title:
		if(m_AudioTrack.GetElapsedPlayTime() >= (LARGE_INTRONOWPLAYING_TIME + LARGE_INTRO_TIME + LARGE_INTROSNAPSHOT_TIME))
		{
			m_LargePlayer.CycleBoxArt(0.5f);
			m_LargeCycleTimer.InitStart(LARGE_CYCLE_DUR);
			m_eLargeState = LARGESTATE_CycleBoxArt;
		}
		break;

	case LARGESTATE_CycleBoxArt:
	case LARGESTATE_CycleSnapshot:
		if(m_LargeCycleTimer.IsExpired())
		{
			if(m_eLargeState == LARGESTATE_CycleBoxArt)
			{
				m_LargePlayer.CycleTitleAndSnapshot(0.5f);
				m_LargeCycleTimer.InitStart(LARGE_CYCLE_DUR);
				m_eLargeState = LARGESTATE_CycleSnapshot;
			}
			else
			{
				m_LargePlayer.CycleBoxArt(0.5f);
				m_LargeCycleTimer.InitStart(LARGE_CYCLE_DUR);
				m_eLargeState = LARGESTATE_CycleBoxArt;
			}
		}

		if(m_AudioTrack.GetElapsedPlayTime() >= VGMUSIC_MAX_SONG_LENGTH)
		{
			m_LargePlayer.FadeOut(LARGE_FADEOUT_DUR);
			m_LargeCycleTimer.InitStart(LARGE_FADEOUT_DUR);
			m_AudioTrack.volume.Tween(0.0f, LARGE_FADEOUT_DUR);
			m_eLargeState = LARGESTATE_FadeOut;
		}
		break;

	case LARGESTATE_FadeOut:
		if(m_LargeCycleTimer.IsExpired())
		{
			NextTrack();
			m_eLargeState = LARGESTATE_Stopped;
		}
		break;

	case LARGESTATE_Hide:
		m_LargePlayer.FadeOut(0.5f);
		m_eLargeState = LARGESTATE_Hidden;
		break;
	}
}

void VgMusic::NextTrack()
{
	m_iCurrTrackIndex = (m_iCurrTrackIndex + 1) % m_MusicFileList.size();

	const std::string sMusicFile = m_MusicFileList[m_iCurrTrackIndex];

	m_AudioTrack.Init(sMusicFile, true, 0, 0, this);
	m_AudioTrack.volume.Set(m_fAudioVolume);
	m_AudioTrack.Play();
	m_AudioTrack.Load();

	m_LargePlayer.InitNextTrack(sMusicFile);
	m_LargePlayer.Load();
}


void VgMusic::FitLargeTextureCentered(HyTexturedQuad2d &textureRef)
{
	HyAssert(textureRef.IsLoaded(), "VgMusic::FitLargeTextureCentered textureRef is not loaded");
}
