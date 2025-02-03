#include "pch.h"
#include "BrowsePage.h"
#include "Compositorium.h"
#include "Melody.h"

#define BROWSEPAGE_LOAD_COOLDOWN 0.5f

BrowsePage::BrowsePage(HyEntity2d *pParent /*= nullptr*/) :
	HyUiContainer(HYORIENT_Vertical, HyPanelInit(GAMEBROWSER_WIDTH, GAMEBROWSER_HEIGHT, 0, HyColor(0.0f, 0.0f, 0.0f, 0.5f)), pParent),
	m_TitleLabel(HyPanelInit(GAMEBROWSER_WIDTH, 64), "MainText", this),
	m_PrevBtn(HyPanelInit(50, 420, 2), "MainText"),
	m_iHoverGameIndex(-1),
	m_NextBtn(HyPanelInit(50, 420, 2), "MainText"),
	m_eReloadState(RELOADSTATE_Idle)
{
	m_TitleLabel.SetTextState(1);

	for(int iGameIndex = 0; iGameIndex < NUM_GAMES_PER_PAGE; ++iGameIndex)
	{
		int iGameWidth = GAMEBROWSER_WIDTH / (NUM_GAMES_PER_PAGE / 2) - 35;
		m_GameTitleLabels[iGameIndex].Setup(HyPanelInit(iGameWidth, 42/*, 2, HyColor(0.0f, 0.0f, 0.0f, 1.0f)*/), "Description");
		m_GameTitleLabels[iGameIndex].SetAsBox();
		m_GameTitleLabels[iGameIndex].SetTextState(2);
		m_GameBtns[iGameIndex].Setup(HyPanelInit(iGameWidth, (GAMEBROWSER_HEIGHT / 2) - 110, 0, HyColor(0.0f, 0.0f, 0.0f, 0.2f)));
		m_GameBtns[iGameIndex].ChildAppend(m_GameBoxarts[iGameIndex]);

		m_GameBtns[iGameIndex].SetButtonClickedCallback(
			[this, iGameIndex](HyButton *pThis)
			{
				if(m_QueuedGameList.empty())
					return;
				
				static_cast<GameBrowser *>(ParentGet())->SetGame(Compositorium::Get()->GetGameStats(m_QueuedGameList[iGameIndex]));
			});
	}

	m_PrevBtn.SetText("<");
	m_PrevBtn.SetButtonClickedCallback(
		[this](HyButton *pThis)
		{
			m_eReloadState = RELOADSTATE_PrevPage;
		});

	m_NextBtn.SetText(">");
	m_NextBtn.SetButtonClickedCallback(
		[this](HyButton *pThis)
		{
			m_eReloadState = RELOADSTATE_NextPage;
		});

	for(int i = 0; i < 26; ++i)
	{
		m_AlphaJumpBtn[i].Setup(HyPanelInit(32, 32, 2), "MainText");
		m_AlphaJumpBtn[i].SetTextState(1);
		m_AlphaJumpBtn[i].SetText(std::string(1, 'A' + i));
		m_AlphaJumpBtn[i].SetButtonClickedCallback(
			[this](HyButton *pThis)
			{
				if(m_QueuedGameList.empty())
					return;
				GameInfo gameInfo = Compositorium::Get()->GetAlphaJumpGame(m_QueuedGameList[0].GetConsole(), pThis->GetUtf8String());
				BrowseAtGame(gameInfo);
			});
	}

	InsertWidget(m_TitleLabel);
	HyLayoutHandle hBody = InsertLayout(HYORIENT_Horizontal);
	InsertWidget(m_PrevBtn, hBody);
	HyLayoutHandle hBodyMain = InsertLayout(HYORIENT_Vertical, hBody);
	HyLayoutHandle hTopRow = InsertLayout(HYORIENT_Horizontal, hBodyMain);
	InsertSpacer(HYSIZEPOLICY_Fixed, 5, hBodyMain);
	HyLayoutHandle hBotRow = InsertLayout(HYORIENT_Horizontal, hBodyMain);
	InsertWidget(m_NextBtn, hBody);

	for(int iGameIndex = 0; iGameIndex < NUM_GAMES_PER_PAGE; ++iGameIndex)
	{
		if(iGameIndex < NUM_GAMES_PER_PAGE / 2) // Two rows per page
		{
			HyLayoutHandle hGame = InsertLayout(HYORIENT_Vertical, hTopRow);
			InsertWidget(m_GameTitleLabels[iGameIndex], hGame);
			InsertWidget(m_GameBtns[iGameIndex], hGame);
		}
		else
		{
			HyLayoutHandle hGame = InsertLayout(HYORIENT_Vertical, hBotRow);
			InsertWidget(m_GameTitleLabels[iGameIndex], hGame);
			InsertWidget(m_GameBtns[iGameIndex], hGame);
		}
	}

	HyLayoutHandle hAlphaJump = InsertLayout(HYORIENT_Horizontal);
	for(int i = 0; i < 26; ++i)
		InsertWidget(m_AlphaJumpBtn[i], hAlphaJump);
	InsertSpacer(HYSIZEPOLICY_Expanding);
}

/*virtual*/ BrowsePage::~BrowsePage()
{
}

void BrowsePage::BrowseAtGame(GameInfo gameInfo)
{
	std::vector<GameInfo> gameList;
	//if(bNext)
		gameList = Compositorium::Get()->GetNextGames(gameInfo, NUM_GAMES_PER_PAGE);
	//else
	//	gameList = Compositorium::Get()->GetPrevGames(gameInfo, NUM_GAMES_PER_PAGE);

	SetList(gameList);
}

void BrowsePage::SetList(std::vector<GameInfo> &gameList)
{
	if(gameList.empty())
		return;

	m_QueuedGameList = gameList;
	m_iHoverGameIndex = -1;

	m_hPrevGame = m_QueuedGameList[0];
	for(int i = 0; i < m_QueuedGameList.size(); ++i)
		m_GameTitleLabels[i].SetText(m_QueuedGameList[i].GetName());
	m_hNextGame = m_QueuedGameList[m_QueuedGameList.size() - 1];

	if(m_ReloadCooldownTimer.IsRunning() && m_ReloadCooldownTimer.IsExpired() == false)
		m_ReloadCooldownTimer.InitStart(BROWSEPAGE_LOAD_COOLDOWN);

	m_eReloadState = RELOADSTATE_TryLoad;
}

void BrowsePage::OnContainerUpdate() /*override*/
{
	switch(m_eReloadState)
	{
	case RELOADSTATE_Idle:
		break;

	case RELOADSTATE_TryLoad:
		if(m_ReloadCooldownTimer.IsExpired())
		{
			m_TitleLabel.SetText("Browsing " + Compositorium::Get()->GetConsoleName(m_QueuedGameList[0].GetConsole()) + " Games");

			
			for(int i = 0; i < m_QueuedGameList.size(); ++i)
			{
				std::string sBestMatchingLogoFile = Compositorium::Get()->GetBestMedia(m_QueuedGameList[i], MEDIATYPE_Boxarts);
				m_GameBoxarts[i].Uninit();
				m_GameBoxarts[i].SetVisible(false);
				if(sBestMatchingLogoFile.empty() == false)
					m_GameBoxarts[i].Init(Compositorium::Get()->GetMediaPath(m_QueuedGameList[i].GetConsole(), MEDIATYPE_Boxarts) + sBestMatchingLogoFile, HyTextureInfo(), &m_GameBtns[i]);
			}

			m_ReloadCooldownTimer.InitStart(BROWSEPAGE_LOAD_COOLDOWN);
			m_eReloadState = RELOADSTATE_Reload;
			Load();
		}
		break;

	case RELOADSTATE_Reload: {
		bool bAllLoaded = true;
		for(int i = 0; i < NUM_GAMES_PER_PAGE; ++i)
		{
			if(m_GameBoxarts[i].IsLoaded())
			{
				if(m_GameBoxarts[i].IsVisible() == false)
				{
					glm::vec2 vSize(m_GameBtns[i].GetWidth(), m_GameBtns[i].GetHeight());
					TransformTexture(m_GameBoxarts[i], vSize, vSize * 0.5f);

					float fGapSize = m_GameTitleLabels[i].pos.GetY();
					fGapSize -= m_GameBoxarts[i].pos.GetY();
					fGapSize -= m_GameBoxarts[i].GetHeight(m_GameBoxarts[i].scale.GetY());
					m_GameBoxarts[i].pos.Offset(0.0f, fGapSize);

					m_GameBoxarts[i].SetVisible(true);
					m_GameBoxarts[i].alpha.Set(0.0f);
					m_GameBoxarts[i].alpha.Tween(1.0f, 0.32f);

					HyRand::Boolean() ? m_GameBtns[i].scale.Set(1.01f, 1.01f) : m_GameBtns[i].scale.Set(0.99f, 0.99f);
					//m_GameBoxarts[i].scale.Set(0.001f, 0.001f);
					//m_GameBoxarts[i].scale.Tween(1.0f, 1.0f, 0.32f, HyTween::QuadOut);
				}
			}
			else
				bAllLoaded = false;
		}

		if(bAllLoaded)
		{
			m_eReloadState = RELOADSTATE_Idle;
		}
		break; }

	case RELOADSTATE_NextPage:
		if(m_hNextGame.IsValid())
			SetList(Compositorium::Get()->GetNextGames(m_hNextGame, NUM_GAMES_PER_PAGE));
		break;

	case RELOADSTATE_PrevPage:
		if(m_hPrevGame.IsValid())
			SetList(Compositorium::Get()->GetNextGames(m_hPrevGame, NUM_GAMES_PER_PAGE));
		break;
	}

	if(IsVisible())
	{
		for(int i = 0; i < NUM_GAMES_PER_PAGE; ++i)
		{
			if(m_GameBtns[i].IsMouseHover())
			{
				if(m_iHoverGameIndex != i)
				{
					if(m_iHoverGameIndex != -1)
						m_GameBtns[m_iHoverGameIndex].SetTint(HyColor::White);

					m_iHoverGameIndex = i;
					m_GameBtns[m_iHoverGameIndex].SetTint(HyColor::LightGray);

					float fSize = 1.02f;
					float fDur = HyRand::Range(0.5f, 1.0f);
					m_GameBtns[i].scale.Tween(fSize, fSize, fDur, HyTween::QuadInOut);
					m_GameBtns[i].pos.Tween(0.0f, 0.0f, fDur, HyTween::QuadInOut);
				}
			}
			
			if(m_iHoverGameIndex != i && m_GameBoxarts[i].IsLoaded() && m_GameBoxarts[i].IsVisible() && m_GameBtns[i].scale.IsAnimating() == false)
			{
				if(m_GameBtns[i].scale.X() >= 1.0f)
				{
					float fSize = 0.98f;// HyRand::Range(0.95f, 0.98f);
					float fDur = HyRand::Range(9.0f, 10.0f);
					m_GameBtns[i].scale.Tween(fSize, fSize, fDur, HyTween::QuadInOut);
					m_GameBtns[i].pos.Tween(0.0f, HyRand::Range(-8.0f, 0.0f), fDur, HyTween::QuadInOut);
				}
				else
				{
					float fSize = 0.95f;
					float fDur = HyRand::Range(9.0f, 10.0f);
					m_GameBtns[i].scale.Tween(fSize, fSize, fDur, HyTween::QuadInOut);
					m_GameBtns[i].pos.Tween(0.0f, HyRand::Range(0.0f, 8.0f), fDur, HyTween::QuadInOut);
				}
			}
		}
	}
}
