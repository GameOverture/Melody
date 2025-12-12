#include "pch.h"
#include "Music.h"
#include "VgMusic.h"
#include "NowPlaying.h"
#include "Melody.h"

//#include <fileref.h>
//#include <tag.h>
//#include <ogg/oggfile.h>

#define LARGE_INTRO_TIME 20.0f
#define LARGE_INTROSNAPSHOT_TIME 5.0f
#define LARGE_INTRONOWPLAYING_TIME 5.0f

#define LARGE_CYCLE_DUR 15.0f

Dancer::Dancer(HyEntity2d *pParent /*= nullptr*/) :
	HyEntityWrapper2d<HySprite2d>("GameSprites", "Dancer", pParent),
	m_eDanceState(DANCESTATE_Stop),
	m_eDeferDanceState(DANCESTATE_Stop)
{
	m_FusedNode.scale.Set(2.0f, 2.0f);
}

DanceState Dancer::GetDanceState() const
{
	return m_eDanceState;
}

void Dancer::Dance()
{
	m_eDanceState = DANCESTATE_Dance;
	m_FusedNode.SetAnimCallback(m_FusedNode.GetState(), nullptr);
	m_FusedNode.SetAnimCtrl(HYANIMCTRL_ResetAndPlay);
	m_FusedNode.SetAnimCtrl(HYANIMCTRL_DontBounce);
}

void Dancer::DanceAlt()
{
	m_eDanceState = DANCESTATE_DanceAlt;
	m_FusedNode.SetAnimCallback(m_FusedNode.GetState(), nullptr);
	m_FusedNode.SetAnimCtrl(HYANIMCTRL_ResetAndPlay);
	m_FusedNode.SetAnimCtrl(HYANIMCTRL_Bounce);
}

void Dancer::Stop()
{
	m_eDanceState = DANCESTATE_Stop;
	m_FusedNode.SetAnimCallback(m_FusedNode.GetState(), 
		[](HySprite2d *pSelf)
		{
			pSelf->SetAnimCtrl(HYANIMCTRL_Reset);
			pSelf->SetAnimPause(true);
		});
}

void Dancer::Shimmy()
{
	m_FusedNode.SetAnimCallback(m_FusedNode.GetState(),
		[](HySprite2d *pSelf)
		{
			pSelf->SetAnimCtrl(HYANIMCTRL_Reset);
			pSelf->SetAnimPause(true);
		});

	m_fElapsedTime = 0.0f;
	m_eDanceState = DANCESTATE_Shimmy;
}

void Dancer::DeferDance(DanceState eDanceState, float fDelay)
{
	m_DeferTimer.InitStart(fDelay);
	m_eDeferDanceState = eDanceState;
}

/*virtual*/ void Dancer::OnUpdate() /*override*/
{
	if(m_DeferTimer.IsExpired())
	{
		switch(m_eDeferDanceState)
		{
		case DANCESTATE_Stop:
			Stop();
			break;
		case DANCESTATE_Shimmy:
			Shimmy();
			break;
		case DANCESTATE_Dance:
			Dance();
			break;
		case DANCESTATE_DanceAlt:
			DanceAlt();
			break;
		}
		m_DeferTimer.Reset();
	}

	if(m_eDanceState == DANCESTATE_Shimmy)
	{
		if(m_FusedNode.GetFrame() == 0 || m_FusedNode.GetFrame() == 1)
		{
			m_fElapsedTime += HyEngine::DeltaTime();

			if(m_fElapsedTime > 0.5f)
			{
				m_FusedNode.SetFrame(m_FusedNode.GetFrame() == 0 ? 1 : 0);
				m_fElapsedTime = 0.0f;
			}
		}
	}
}

Music::Music(VgMusic &vgMusicRef, NowPlaying &nowPlayingRef, HyEntity2d *pParent /*= nullptr*/) :
	Channel(CHANNELTYPE_Music, pParent),
	m_VgMusicRef(vgMusicRef),
	m_NowPlayingRef(nowPlayingRef),
	m_AudioVisualizer("VgMusic", "Visualizer", this),
	m_Snapshot(this),
	m_Title(this),
	m_BoxArt(this),
	m_eLargeState(LARGESTATE_Stopped)
{
	m_VgMusicRef.SetOnTrackChangeCallback([this](MusicTrack &musicTrackRef) { InitNextTrack(musicTrackRef); });
	m_VgMusicRef.SetOnFadeOutCallback([this](float fFadeOutDuration)
		{
			FadeOut(fFadeOutDuration);
			m_eLargeState = LARGESTATE_Stopped;

			for(int32 i = 0; i < NUM_DANCERS; ++i)
				m_Dancers[i].DeferDance(DANCESTATE_Stop, fFadeOutDuration * 0.5f);
		});

	m_AudioVisualizer.SetVisible(false);

	m_NowPlayingSound.SetVisible(false);
	m_NowPlayingSound.scale.Set(1.0f, 0.5f);
	m_NowPlayingSound.pos.Set(CRT_SCREEN_WIDTH, 20);

	m_TitleText.SetVisible(false);
	m_TitleText.SetAlignment(HYALIGN_Right);
	m_TitleText.pos.Set(0.0f, 80.0f);
	m_TitleText.SetAsScaleBox(CRT_SCREEN_WIDTH, 50.0f);

	m_TrackText.SetVisible(false);
	m_TrackText.SetAlignment(HYALIGN_Right);
	m_TrackText.pos.Set(0.0f, 30.0f);
	m_TrackText.SetAsScaleBox(CRT_SCREEN_WIDTH, 50.0f);

	m_ComposerText.SetVisible(false);
	m_ComposerText.SetAlignment(HYALIGN_Right);
	m_ComposerText.pos.Set(0.0f, 5.0f);
	m_ComposerText.SetAsScaleBox(CRT_SCREEN_WIDTH, 50.0f);

	m_Snapshot.SetVisible(false);
	m_Title.SetVisible(false);
	m_BoxArt.SetVisible(false);

	for(int32 i = 0; i < NUM_DANCERS; ++i)
	{
		ChildAppend(m_Dancers[i]);
		m_Dancers[i].GetLeaf().SetState(i);
		m_Dancers[i].Stop();
	}
	m_Dancers[0].pos.Set(130.0f, 25.0f);
	m_Dancers[1].pos.Set(250.0f, 90.0f);
	m_Dancers[2].pos.Set(370.0f, 25.0f);
	m_Dancers[3].pos.Set(490.0f, 90.0f);
	m_Dancers[4].pos.Set(610.0f, 25.0f);
	m_Dancers[5].pos.Set(730.0f, 90.0f);

	m_NowPlayingSound.Init("VgMusic", "NowPlayingSound", this);
	m_TitleText.Init("", "MainText", this);
	m_TrackText.Init("", "MainText", this);
	m_ComposerText.Init("", "CtrlPanel", this);

	//m_Debugbox.SetParent(this);
	//m_Debugbox.SetWireframe(true);
	//m_Debugbox.SetAsBox(LARGE_WIDTH, LARGE_HEIGHT);
}

/*virtual*/ Music::~Music()
{
}

void Music::InitNextTrack(const MusicTrack &musicTrack)
{
	GameConsole eConsole;
	std::string sSongName, sComposerName;
	Compositorium::Get()->GetMusicInfo(musicTrack, eConsole, sSongName, sComposerName);
	
	m_TrackText.SetText(sSongName);
	m_ComposerText.SetText(sComposerName);

	GameInfo gameInfo = Compositorium::Get()->GetGame(musicTrack.m_sGameId);

	m_TitleText.SetText(gameInfo.GetName());

	std::string sMediaPath = Compositorium::Get()->GetMediaPath(gameInfo.GetConsole(), MEDIATYPE_Boxarts);
	std::string sBoxartFile = Compositorium::Get()->GetBestMedia(gameInfo, MEDIATYPE_Boxarts);
	if(sBoxartFile.empty() == false)
		m_BoxArt.Init(sMediaPath + sBoxartFile, HyTextureInfo(), this);

	sMediaPath = Compositorium::Get()->GetMediaPath(gameInfo.GetConsole(), MEDIATYPE_Titles);
	std::string sTitleFile = Compositorium::Get()->GetBestMedia(gameInfo, MEDIATYPE_Titles);
	if(sTitleFile.empty() == false)
		m_Title.Init(sMediaPath + sTitleFile, HyTextureInfo(), this);

	sMediaPath = Compositorium::Get()->GetMediaPath(gameInfo.GetConsole(), MEDIATYPE_Snaps);
	std::string sSnapshotFile = Compositorium::Get()->GetBestMedia(gameInfo, MEDIATYPE_Snaps);
	if(sSnapshotFile.empty() == false)
		m_Snapshot.Init(sMediaPath + sSnapshotFile, HyTextureInfo(), this);

	m_AudioVisualizer.SetVisible(false);
	m_NowPlayingSound.SetVisible(false);
	m_TitleText.SetVisible(false);
	m_TrackText.SetVisible(false);
	m_ComposerText.SetVisible(false);
	m_Snapshot.SetVisible(false);
	m_Title.SetVisible(false);
	m_BoxArt.SetVisible(false);

	Load();
}

void Music::ShowVisualizer(float fFadeInTime)
{
	FadeOut(fFadeInTime);

	// Center the texture, using the bottom left corner as the anchor. Fade it in from 0.0f alpha to 1.0f alpha
	m_AudioVisualizer.scale.Set(1.0f, 1.0f);
	if(m_AudioVisualizer.GetWidth() > CRT_SCREEN_WIDTH || m_AudioVisualizer.GetHeight() > CRT_SCREEN_HEIGHT - 100.0f)
	{
		float fScale = std::min(CRT_SCREEN_WIDTH / m_AudioVisualizer.GetWidth(), (CRT_SCREEN_HEIGHT - 100.0f) / m_AudioVisualizer.GetHeight());
		m_AudioVisualizer.scale.Set(fScale, fScale);
	}
	m_AudioVisualizer.pos.Set(CRT_SCREEN_WIDTH / 2.0f, 100.0f + (CRT_SCREEN_HEIGHT / 2.0f));
	m_AudioVisualizer.SetState(HyRand::Range(0u, m_AudioVisualizer.GetNumStates() - 1));

	m_AudioVisualizer.alpha.Set(0.0f);
	m_AudioVisualizer.alpha.Tween(1.0f, fFadeInTime);
	m_AudioVisualizer.SetVisible(true);
}

void Music::ShowIntroTitle(float fFadeInTime)
{
	FadeOut(fFadeInTime);

	glm::ivec2 vMaxSize(CRT_SCREEN_WIDTH, CRT_SCREEN_HEIGHT - 100.0f);
	glm::vec2 ptCenter(vMaxSize.x * 0.5f, 100.0f + (vMaxSize.y * 0.5f));
	TransformTexture(m_Title, vMaxSize, ptCenter);

	m_Title.alpha.Set(0.0f);
	m_Title.alpha.Tween(1.0f, fFadeInTime);
	m_Title.SetVisible(true);
}

void Music::ShowNowPlaying(float fFadeInTime)
{
	m_NowPlayingSound.alpha.Set(0.0f);
	m_NowPlayingSound.alpha.Tween(1.0f, fFadeInTime);
	m_NowPlayingSound.SetVisible(true);

	m_TitleText.alpha.Set(0.0f);
	m_TitleText.alpha.Tween(1.0f, fFadeInTime);
	m_TitleText.SetVisible(true);

	m_TrackText.alpha.Set(0.0f);
	m_TrackText.alpha.Tween(1.0f, fFadeInTime);
	m_TrackText.SetVisible(true);

	m_ComposerText.alpha.Set(0.0f);
	m_ComposerText.alpha.Tween(1.0f, fFadeInTime);
	m_ComposerText.SetVisible(true);
}

void Music::CycleBoxArt(float fFadeInTime)
{
	if(m_Snapshot.IsVisible())
		m_Snapshot.alpha.Tween(0.0f, fFadeInTime, HyTween::Linear, 0.0f, [](IHyNode *pThis) { pThis->SetVisible(false); });
	if(m_Title.IsVisible())
		m_Title.alpha.Tween(0.0f, fFadeInTime, HyTween::Linear, 0.0f, [](IHyNode *pThis) { pThis->SetVisible(false); });

	glm::ivec2 vMaxSize(CRT_SCREEN_WIDTH, CRT_SCREEN_HEIGHT - 100.0f);
	glm::vec2 ptCenter(vMaxSize.x * 0.5f, 100.0f + (vMaxSize.y * 0.5f));
	TransformTexture(m_BoxArt, vMaxSize, ptCenter);

	m_BoxArt.alpha.Set(0.0f);
	m_BoxArt.alpha.Tween(1.0f, fFadeInTime);
	m_BoxArt.SetVisible(true);
}

void Music::CycleTitleAndSnapshot(float fFadeInTime)
{
	if(m_BoxArt.IsVisible())
		m_BoxArt.alpha.Tween(0.0f, fFadeInTime, HyTween::Linear, 0.0f, [](IHyNode *pThis) { pThis->SetVisible(false); });

	glm::ivec2 vMaxSize(CRT_SCREEN_WIDTH * 0.5f, CRT_SCREEN_HEIGHT - 100.0f);
	glm::vec2 ptCenter(vMaxSize.x * 0.5f, 100.0f + (vMaxSize.y * 0.5f));

	TransformTexture(m_Title, vMaxSize, ptCenter);
	m_Title.alpha.Set(0.0f);
	m_Title.alpha.Tween(1.0f, fFadeInTime);
	m_Title.SetVisible(true);

	ptCenter.x += CRT_SCREEN_WIDTH * 0.5f;
	TransformTexture(m_Snapshot, vMaxSize, ptCenter);
	m_Snapshot.alpha.Set(0.0f);
	m_Snapshot.alpha.Tween(1.0f, fFadeInTime);
	m_Snapshot.SetVisible(true);
}

void Music::FadeOut(float fFadeOutTime)
{
	if(m_AudioVisualizer.IsVisible())
		m_AudioVisualizer.alpha.Tween(0.0f, fFadeOutTime, HyTween::Linear, 0.0f, [](IHyNode *pThis) { pThis->SetVisible(false); });
	if(m_NowPlayingSound.IsVisible())
		m_NowPlayingSound.alpha.Tween(0.0f, fFadeOutTime, HyTween::Linear, 0.0f, [](IHyNode *pThis) { pThis->SetVisible(false); });
	if(m_TitleText.IsVisible())
		m_TitleText.alpha.Tween(0.0f, fFadeOutTime, HyTween::Linear, 0.0f, [](IHyNode *pThis) { pThis->SetVisible(false); });
	if(m_TrackText.IsVisible())
		m_TrackText.alpha.Tween(0.0f, fFadeOutTime, HyTween::Linear, 0.0f, [](IHyNode *pThis) { pThis->SetVisible(false); });
	if(m_ComposerText.IsVisible())
		m_ComposerText.alpha.Tween(0.0f, fFadeOutTime, HyTween::Linear, 0.0f, [](IHyNode *pThis) { pThis->SetVisible(false); });
	if(m_Snapshot.IsVisible())
		m_Snapshot.alpha.Tween(0.0f, fFadeOutTime, HyTween::Linear, 0.0f, [](IHyNode *pThis) { pThis->SetVisible(false); });
	if(m_Title.IsVisible())
		m_Title.alpha.Tween(0.0f, fFadeOutTime, HyTween::Linear, 0.0f, [](IHyNode *pThis) { pThis->SetVisible(false); });
	if(m_BoxArt.IsVisible())
		m_BoxArt.alpha.Tween(0.0f, fFadeOutTime, HyTween::Linear, 0.0f, [](IHyNode *pThis) { pThis->SetVisible(false); });

	m_NowPlayingRef.Hide(fFadeOutTime * 0.5f);
}

/*virtual*/ void Music::OnUpdate() /*override*/
{
	//if(HyEngine::Input().IsActionReleased(INPUT_Dance))
	//{
	//	for(int32 i = 0; i < NUM_DANCERS; ++i)
	//		m_Dancers[i].Dance();
	//}
	//if(HyEngine::Input().IsActionReleased(INPUT_DanceAlt))
	//{
	//	for(int32 i = 0; i < NUM_DANCERS; ++i)
	//		m_Dancers[i].DanceAlt();
	//}
	//if(HyEngine::Input().IsActionReleased(INPUT_DanceStop))
	//{
	//	for(int32 i = 0; i < NUM_DANCERS; ++i)
	//		m_Dancers[i].Stop();
	//}
	//if(HyEngine::Input().IsActionReleased(INPUT_DanceShimmy))
	//{
	//	for(int32 i = 0; i < NUM_DANCERS; ++i)
	//		m_Dancers[i].Shimmy();
	//}

	switch(m_eLargeState)
	{
	case LARGESTATE_Stopped:
		if(m_VgMusicRef.GetPlayState() == PLAYSTATE_Playing)
		{
			ShowVisualizer(0.5f);

			for(int32 i = 0; i < NUM_DANCERS; ++i)
				m_Dancers[i].DeferDance(DANCESTATE_Shimmy, 4.0f);

			m_NowPlayingRef.SetGame(m_VgMusicRef.m_MusicTrackList[m_VgMusicRef.m_iCurrTrackIndex].m_sGameId);

			m_eLargeState = LARGESTATE_Intro;
		}
		break;

	case LARGESTATE_Intro:
		if(m_VgMusicRef.GetElapsedPlayTime() >= LARGE_INTRO_TIME - 5.0f)
		{
			for(int32 i = 0; i < NUM_DANCERS; ++i)
			{
				if(m_Dancers[i].GetDanceState() != DANCESTATE_Dance)
					m_Dancers[i].Dance();
			}
		}

		if(m_VgMusicRef.GetElapsedPlayTime() >= LARGE_INTRO_TIME)
		{
			ShowIntroTitle(0.5f);

			m_eLargeState = LARGESTATE_IntroTitle;
		}
		break;

	case LARGESTATE_IntroTitle:
		if(m_VgMusicRef.GetElapsedPlayTime() >= (LARGE_INTRO_TIME + LARGE_INTROSNAPSHOT_TIME))
		{
			ShowNowPlaying(0.5f);
			m_eLargeState = LARGESTATE_NowPlaying;
		}
		break;

	case LARGESTATE_NowPlaying:
		if(m_VgMusicRef.GetElapsedPlayTime() >= (LARGE_INTRONOWPLAYING_TIME + LARGE_INTRO_TIME + LARGE_INTROSNAPSHOT_TIME))
		{
			CycleBoxArt(0.5f);
			m_LargeCycleTimer.InitStart(LARGE_CYCLE_DUR);


			for(int32 i = 0; i < NUM_DANCERS; ++i)
			{
				m_Dancers[i].DanceAlt();
				m_Dancers[i].DeferDance(DANCESTATE_Dance, VGMUSIC_MAX_SONG_LENGTH * 0.5f);
			}

			m_NowPlayingRef.Show(0.5f);

			m_eLargeState = LARGESTATE_CycleBoxArt;
		}
		break;

	case LARGESTATE_CycleBoxArt:
	case LARGESTATE_CycleSnapshot:
		if(m_LargeCycleTimer.IsExpired())
		{
			if(m_eLargeState == LARGESTATE_CycleBoxArt)
			{
				CycleTitleAndSnapshot(1.5f);
				m_LargeCycleTimer.InitStart(LARGE_CYCLE_DUR);
				m_eLargeState = LARGESTATE_CycleSnapshot;
			}
			else
			{
				CycleBoxArt(1.5f);
				m_LargeCycleTimer.InitStart(LARGE_CYCLE_DUR);
				m_eLargeState = LARGESTATE_CycleBoxArt;
			}
		}
		break;
	}
}

