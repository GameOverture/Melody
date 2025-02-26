#include "pch.h"
#include "Compositorium.h"
#include "VgMusic.h"
#include "CtrlPanel.h"
#include "Crt.h"

VgMusic::VgMusic(HyEntity2d *pParent /*= nullptr*/) :
	HyEntity2d(pParent),
	m_CtrlPanel_LoadCheckBox(HyPanelInit(32, 32, 2), HyNodePath("", "CtrlPanel"), this),
	m_CtrlPanel_PrevBtn(HyPanelInit(42, 32, 2), HyNodePath("", "MainText"), this),
	m_CtrlPanel_PlayBtn(HyPanelInit(42, 32, 2), HyNodePath("", "MainText"), this),
	m_CtrlPanel_StopBtn(HyPanelInit(64, 32, 2), HyNodePath("", "CtrlPanel"), this),
	m_pCrtRef(nullptr),
	m_btnVolume_Down(HyPanelInit(64, 32, 2), HyNodePath("", "CtrlPanel"), this),
	m_btnVolume_Up(HyPanelInit(64, 32, 2), HyNodePath("", "CtrlPanel"), this),
	m_iCurrTrackIndex(-1),
	m_ePlayState(PLAYSTATE_Stopped),
	m_fpOnTrackChange(nullptr),
	m_fpOnFadeOut(nullptr)
{
	m_CtrlPanel_LoadCheckBox.SetText("VgMusic");
	m_CtrlPanel_LoadCheckBox.SetCheckedChangedCallback(
		[this](HyCheckBox *pThis)
		{
			if(pThis->IsChecked() && m_MusicTrackList.empty())
				m_MusicTrackList = Compositorium::Get()->GetMusicPlayList(CONSOLE_NES | CONSOLE_SNES | CONSOLE_Genesis | CONSOLE_N64 | CONSOLE_Dreamcast);
		});

	m_CtrlPanel_PrevBtn.SetAsBox(HYALIGN_Center);
	m_CtrlPanel_PrevBtn.SetTextState(1);
	m_CtrlPanel_PrevBtn.SetText("<");
	m_CtrlPanel_PrevBtn.SetButtonClickedCallback([this](HyButton *pThis)
		{
			Prev();
		});

	m_CtrlPanel_PlayBtn.SetAsBox(HYALIGN_Center);
	m_CtrlPanel_PlayBtn.SetTextState(1);
	m_CtrlPanel_PlayBtn.SetText(">");
	m_CtrlPanel_PlayBtn.SetButtonClickedCallback([this](HyButton *pThis)
		{
			Play();
		});

	m_CtrlPanel_StopBtn.SetAsBox(HYALIGN_Center);
	m_CtrlPanel_StopBtn.SetTextState(1);
	m_CtrlPanel_StopBtn.SetText("STOP");
	m_CtrlPanel_StopBtn.SetButtonClickedCallback([this](HyButton *pThis)
		{
			Stop();
		});

	m_btnVolume_Down.SetAsBox(HYALIGN_Center);
	m_btnVolume_Down.SetText("VOL -");
	m_btnVolume_Down.SetButtonClickedCallback([this](HyButton *pThis)
		{
			HyEngine::Audio().SetGlobalVolume(HyMath::Clamp(HyEngine::Audio().GetGlobalVolume() - 0.05f, 0.0f, 1.0f));
			m_pCrtRef->SetVolume(HyEngine::Audio().GetGlobalVolume());
		});

	m_btnVolume_Up.SetAsBox(HYALIGN_Center);
	m_btnVolume_Up.SetText("VOL +");
	m_btnVolume_Up.SetButtonClickedCallback([this](HyButton *pThis)
		{
			HyEngine::Audio().SetGlobalVolume(HyMath::Clamp(HyEngine::Audio().GetGlobalVolume() + 0.05f, 0.0f, 1.0f));
			m_pCrtRef->SetVolume(HyEngine::Audio().GetGlobalVolume());
		});
}

/*virtual*/ VgMusic::~VgMusic()
{
}

void VgMusic::SetCrtRef(Crt *pCrtRef)
{
	m_pCrtRef = pCrtRef;
}

void VgMusic::PopulateCtrlPanel(CtrlPanel &ctrlPanel)
{
	HyLayoutHandle hCurRow = ctrlPanel.InsertLayout(HYORIENT_Horizontal);
	ctrlPanel.InsertWidget(m_CtrlPanel_LoadCheckBox, hCurRow);
	ctrlPanel.InsertWidget(m_CtrlPanel_PrevBtn, hCurRow);
	ctrlPanel.InsertWidget(m_CtrlPanel_PlayBtn, hCurRow);
	ctrlPanel.InsertWidget(m_CtrlPanel_StopBtn, hCurRow);
	ctrlPanel.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hCurRow);
	
	HyLayoutHandle hVolumeRow = ctrlPanel.InsertLayout(HYORIENT_Horizontal);
	ctrlPanel.InsertWidget(m_btnVolume_Down, hVolumeRow);
	ctrlPanel.InsertWidget(m_btnVolume_Up, hVolumeRow);
}

void VgMusic::SetOnTrackChangeCallback(std::function<void(MusicTrack &)> fpOnTrackChange)
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

void VgMusic::Prev()
{
	if(m_MusicTrackList.empty())
		return;

	if(m_iCurrTrackIndex > 0)
		m_iCurrTrackIndex--;
	else
		m_iCurrTrackIndex = static_cast<int>(m_MusicTrackList.size()) - 1;
	
	Play();
}

void VgMusic::Play()
{
	if(m_MusicTrackList.empty())
		return;

	FadeOutTrack(0.5f);
	m_ePlayState = PLAYSTATE_FadeOutToNext;
}

void VgMusic::Stop()
{
	if(m_MusicTrackList.empty())
		return;

	FadeOutTrack(VGMUSIC_STOP_FADEOUT_DUR);
	m_ePlayState = PLAYSTATE_FadeOutToStop;
}

/*virtual*/ void VgMusic::OnUpdate() /*override*/
{
	//if(HyEngine::Input().IsActionReleased(INPUT_VgMusicPlay))
	//{
	//	Play();
	//}

	//if(HyEngine::Input().IsActionReleased(INPUT_VgMusicStop))
	//{
	//	Stop();
	//}

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
			m_iCurrTrackIndex = (m_iCurrTrackIndex + 1) % m_MusicTrackList.size();
			MusicTrack musicTrack = m_MusicTrackList[m_iCurrTrackIndex];

			m_AudioTrack.Init(musicTrack.m_sFilePath, true, 0, 0, this);
			m_AudioTrack.volume.Set(1.0f);
			m_AudioTrack.Play();
			m_AudioTrack.Load();

			if(m_fpOnTrackChange)
				m_fpOnTrackChange(musicTrack);

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
	m_AudioTrack.volume.Set(1.0f);
	m_AudioTrack.volume.Tween(0.0f, fFadeOutDuration);

	if(m_fpOnFadeOut)
		m_fpOnFadeOut(fFadeOutDuration);
}
