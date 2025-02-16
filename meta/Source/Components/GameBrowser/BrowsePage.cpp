#include "pch.h"
#include "BrowsePage.h"
#include "Compositorium.h"
#include "Melody.h"

#define BROWSEPAGE_LOAD_COOLDOWN 0.5f

//#define BROWSEPAGE_TOP_SPACER_HEIGHT 25
#define BROWSEPAGE_PAGE_BTN_WIDTH 50
#define BROWSEPAGE_SPINE_SPACER 75.0f
#define BROWSEPAGE_BACKBTN_SIZE 100, 100
#define BROWSEPAGE_GAME_WIDTH ((GAMEBROWSER_LAYOUT_WIDTH - (BROWSEPAGE_PAGE_BTN_WIDTH*2) - BROWSEPAGE_SPINE_SPACER) / (NUM_GAMES_PER_PAGE / 2))
#define BROWSEPAGE_GAME_HEIGHT ((GAMEBROWSER_LAYOUT_HEIGHT / 2) - 128)

BrowsePage::BrowsePage(HyEntity2d *pParent /*= nullptr*/) :
	HyUiContainer(HYORIENT_Vertical, HyPanelInit(GAMEBROWSER_WIDTH, GAMEBROWSER_HEIGHT), pParent),
	m_PrevBtn(HyPanelInit(BROWSEPAGE_PAGE_BTN_WIDTH, BROWSEPAGE_GAME_HEIGHT * 2), "MainText"),
	m_iHoverGameIndex(-1),
	m_NextBtn(HyPanelInit(BROWSEPAGE_PAGE_BTN_WIDTH, BROWSEPAGE_GAME_HEIGHT * 2), "MainText"),
	m_BackBtn(HyPanelInit(HYTYPE_Sprite, HyNodePath("Consoles/Console"), BROWSEPAGE_BACKBTN_SIZE), "MainText", this),
	m_eReloadState(RELOADSTATE_Idle)
{
	m_RootLayout.SetMargins(GAMEBROWSER_MARGINS, 5);

	for(int iGameIndex = 0; iGameIndex < NUM_GAMES_PER_PAGE; ++iGameIndex)
	{
		m_GameTitleLabels[iGameIndex].Setup(HyPanelInit(BROWSEPAGE_GAME_WIDTH, 42/*, 2, HyColor(0.0f, 0.0f, 0.0f, 1.0f)*/), "Description");
		m_GameTitleLabels[iGameIndex].SetAsBox();
		m_GameTitleLabels[iGameIndex].SetTextState(2);
		m_GameBtns[iGameIndex].Setup(HyPanelInit(BROWSEPAGE_GAME_WIDTH, BROWSEPAGE_GAME_HEIGHT/*, 0, HyColor(0.0f, 0.0f, 0.0f, 0.2f)*/));
		/*m_GameBtns[iGameIndex].*/ChildAppend(m_GameBoxarts[iGameIndex]);

		m_GameBtns[iGameIndex].SetButtonClickedCallback(
			[this, iGameIndex](HyButton *pThis)
			{
				if(IsInputAllowed() == false)
					return;

				m_iHoverGameIndex = iGameIndex;
				static_cast<GameBrowser *>(ParentGet())->SetGame(
					m_GameBoxarts[iGameIndex],
					GetWidgetPos(m_GameBtns[iGameIndex]),
					Compositorium::Get()->GetGameStats(m_QueuedGamesArray[iGameIndex]));
			});
	}

	m_PrevBtn.SetText("<");
	m_PrevBtn.SetButtonClickedCallback(
		[this](HyButton *pThis)
		{
			static_cast<GameBrowser *>(ParentGet())->PrevPage();

			if(IsInputAllowed() == false)
				return;

			m_eReloadState = RELOADSTATE_PrevPage;
		});

	m_NextBtn.SetText(">");
	m_NextBtn.SetButtonClickedCallback(
		[this](HyButton *pThis)
		{
			static_cast<GameBrowser *>(ParentGet())->NextPage();

			if(IsInputAllowed() == false)
				return;

			m_eReloadState = RELOADSTATE_NextPage;
		});

	m_BackBtn.SetButtonClickedCallback(
		[this](HyButton *pThis)
		{
			static_cast<GameBrowser *>(ParentGet())->ShowConsoles();
		});

	for(int i = 0; i < 26; ++i)
	{
		m_AlphaJumpBtn[i].Setup(HyPanelInit(32, 32, 2), "MainText");
		m_AlphaJumpBtn[i].SetTextState(1);
		m_AlphaJumpBtn[i].SetText(std::string(1, 'A' + i));
		m_AlphaJumpBtn[i].SetButtonClickedCallback(
			[this](HyButton *pThis)
			{
				if(IsInputAllowed() == false)
					return;

				GameInfo gameInfo = Compositorium::Get()->GetAlphaJumpGame(m_QueuedGamesArray[0].GetConsole(), pThis->GetUtf8String());
				BrowseAtGame(gameInfo);
			});
	}

	//InsertSpacer(HYSIZEPOLICY_Fixed, BROWSEPAGE_TOP_SPACER_HEIGHT);
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
			if(iGameIndex == NUM_GAMES_PER_PAGE / 4)
				InsertSpacer(HYSIZEPOLICY_Fixed, BROWSEPAGE_SPINE_SPACER, hTopRow);

			HyLayoutHandle hGame = InsertLayout(HYORIENT_Vertical, hTopRow);
			InsertWidget(m_GameTitleLabels[iGameIndex], hGame);
			InsertWidget(m_GameBtns[iGameIndex], hGame);
		}
		else
		{
			if(iGameIndex == NUM_GAMES_PER_PAGE / 4 * 3)
				InsertSpacer(HYSIZEPOLICY_Fixed, BROWSEPAGE_SPINE_SPACER, hBotRow);

			HyLayoutHandle hGame = InsertLayout(HYORIENT_Vertical, hBotRow);
			InsertWidget(m_GameTitleLabels[iGameIndex], hGame);
			InsertWidget(m_GameBtns[iGameIndex], hGame);
		}
	}

	HyLayoutHandle hAlphaJump = InsertLayout(HYORIENT_Horizontal);
	InsertWidget(m_BackBtn, hAlphaJump);
	for(int i = 0; i < 26; ++i)
		InsertWidget(m_AlphaJumpBtn[i], hAlphaJump);
	InsertSpacer(HYSIZEPOLICY_Expanding);
}

/*virtual*/ BrowsePage::~BrowsePage()
{
}

void BrowsePage::BrowseAtGame(GameInfo gameInfo)
{
	std::vector<GameInfo> gameList = Compositorium::Get()->GetNextGames(gameInfo, NUM_GAMES_PER_PAGE);
	HyAssert(gameList.size() == NUM_GAMES_PER_PAGE, "BrowsePage::BrowseAtGame() - gameList.size() != NUM_GAMES_PER_PAGE");

	std::array<GameInfo, NUM_GAMES_PER_PAGE> gameArray = { gameList[0], gameList[1], gameList[2], gameList[3], gameList[4], gameList[5], gameList[6], gameList[7] };
	SetList(gameArray);
}

void BrowsePage::SetList(std::array<GameInfo, NUM_GAMES_PER_PAGE> &gameList)
{
	if(gameList.empty())
		return;

	m_QueuedGamesArray = gameList;
	m_iHoverGameIndex = -1;

	m_hPrevGame = m_QueuedGamesArray[0];
	for(int i = 0; i < NUM_GAMES_PER_PAGE; ++i)
		m_GameTitleLabels[i].SetText(m_QueuedGamesArray[i].GetName());
	m_hNextGame = m_QueuedGamesArray[NUM_GAMES_PER_PAGE - 1];

	if(m_ReloadCooldownTimer.IsRunning() && m_ReloadCooldownTimer.IsExpired() == false)
		m_ReloadCooldownTimer.InitStart(BROWSEPAGE_LOAD_COOLDOWN);

	m_eReloadState = RELOADSTATE_TryLoad;
}

void BrowsePage::ReturnBoxart(HyTexturedQuad2d &boxartRef)
{
	m_GameBtns[m_iHoverGameIndex].ChildAppend(boxartRef);
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
			m_BackBtn.SetState(Compositorium::Get()->GetConsoleIndex(m_QueuedGamesArray[0].GetConsole()));

			for(int i = 0; i < NUM_GAMES_PER_PAGE; ++i)
			{
				std::string sBestMatchingLogoFile = Compositorium::Get()->GetBestMedia(m_QueuedGamesArray[i], MEDIATYPE_Boxarts);
				m_GameBoxarts[i].Uninit();
				m_GameBoxarts[i].SetVisible(false);
				if(sBestMatchingLogoFile.empty() == false)
					m_GameBoxarts[i].Init(Compositorium::Get()->GetMediaPath(m_QueuedGamesArray[i].GetConsole(), MEDIATYPE_Boxarts) + sBestMatchingLogoFile, HyTextureInfo(), &m_GameBtns[i]);
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
		{
			std::vector<GameInfo> gameList = Compositorium::Get()->GetNextGames(m_hNextGame, NUM_GAMES_PER_PAGE);
			HyAssert(gameList.size() == NUM_GAMES_PER_PAGE, "BrowsePage::BrowseAtGame() - gameList.size() != NUM_GAMES_PER_PAGE");

			std::array<GameInfo, NUM_GAMES_PER_PAGE> gameArray = { gameList[0], gameList[1], gameList[2], gameList[3], gameList[4], gameList[5], gameList[6], gameList[7] };
			SetList(gameArray);


		}
		break;

	case RELOADSTATE_PrevPage:
		if(m_hPrevGame.IsValid())
		{
			std::vector<GameInfo> gameList = Compositorium::Get()->GetPrevGames(m_hPrevGame, NUM_GAMES_PER_PAGE);
			HyAssert(gameList.size() == NUM_GAMES_PER_PAGE, "BrowsePage::BrowseAtGame() - gameList.size() != NUM_GAMES_PER_PAGE");

			std::array<GameInfo, NUM_GAMES_PER_PAGE> gameArray = { gameList[0], gameList[1], gameList[2], gameList[3], gameList[4], gameList[5], gameList[6], gameList[7] };
			SetList(gameArray);
		}
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
					//int iGameWidth = GAMEBROWSER_WIDTH / (NUM_GAMES_PER_PAGE / 2) - 35;
					//if(m_iHoverGameIndex != -1)
					//{
					//	m_GameBtns[m_iHoverGameIndex].Setup(HyPanelInit(iGameWidth, BROWSEPAGE_GAME_HEIGHT, 0, HyColor(0.0f, 0.0f, 0.0f, 0.0f)));
					//	//m_GameBtns[m_iHoverGameIndex].SetTint(HyColor::White);
					//}

					m_iHoverGameIndex = i;
					//m_GameBtns[m_iHoverGameIndex].SetTint(HyColor::LightGray);
					//m_GameBtns[m_iHoverGameIndex].Setup(HyPanelInit(iGameWidth, BROWSEPAGE_GAME_HEIGHT, 0, HyColor(0.0f, 0.0f, 0.0f, 0.7f)));

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
