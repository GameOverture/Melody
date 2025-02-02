#include "pch.h"
#include "BrowsePage.h"
#include "Compositorium.h"
#include "Melody.h"

#define BROWSEPAGE_LOAD_COOLDOWN 0.5f

BrowsePage::BrowsePage(HyEntity2d *pParent /*= nullptr*/) :
	HyUiContainer(HYORIENT_Vertical, HyPanelInit(GAMEBROWSER_WIDTH, GAMEBROWSER_HEIGHT, 0, HyColor(0.0f, 0.0f, 0.0f, 0.5f)), pParent),
	m_TitleLabel(HyPanelInit(GAMEBROWSER_WIDTH, 64), "MainText", this),
	m_PrevBtn(HyPanelInit(50, 420, 2), "MainText"),
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
		m_GameDescLabels[iGameIndex].Setup(HyPanelInit(iGameWidth, (GAMEBROWSER_HEIGHT / 2) - 110/*, 2, HyColor(0.0f, 0.0f, 0.0f, 1.0f)*/));
		m_GameDescLabels[iGameIndex].ChildAppend(m_GameBoxarts[iGameIndex]);
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
				SetGame(gameInfo);
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
			InsertWidget(m_GameDescLabels[iGameIndex], hGame);
		}
		else
		{
			HyLayoutHandle hGame = InsertLayout(HYORIENT_Vertical, hBotRow);
			InsertWidget(m_GameTitleLabels[iGameIndex], hGame);
			InsertWidget(m_GameDescLabels[iGameIndex], hGame);
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

void BrowsePage::SetGame(GameInfo gameInfo)
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

	m_hPrevGame = Compositorium::Get()->GetPrevGames(m_QueuedGameList[0], 2)[1];
	for(int i = 0; i < m_QueuedGameList.size(); ++i)
		m_GameTitleLabels[i].SetText(m_QueuedGameList[i].GetName());
	m_hNextGame = Compositorium::Get()->GetNextGames(m_QueuedGameList[m_QueuedGameList.size() - 1], 2)[0];

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
					m_GameBoxarts[i].Init(Compositorium::Get()->GetMediaPath(m_QueuedGameList[i].GetConsole(), MEDIATYPE_Boxarts) + sBestMatchingLogoFile, HyTextureInfo(), &m_GameDescLabels[i]);
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
					glm::vec2 vSize(m_GameDescLabels[i].GetWidth(), m_GameDescLabels[i].GetHeight());
					TransformTexture(m_GameBoxarts[i], vSize, vSize * 0.5f);

					float fGapSize = m_GameTitleLabels[i].pos.GetY();
					fGapSize -= m_GameBoxarts[i].pos.GetY();
					fGapSize -= m_GameBoxarts[i].GetHeight(m_GameBoxarts[i].scale.GetY());
					m_GameBoxarts[i].pos.Offset(0.0f, fGapSize);

					m_GameBoxarts[i].SetVisible(true);
					m_GameBoxarts[i].alpha.Set(0.0f);
					m_GameBoxarts[i].alpha.Tween(1.0f, 0.32f);
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
		SetList(Compositorium::Get()->GetNextGames(m_hNextGame, NUM_GAMES_PER_PAGE));
		break;

	case RELOADSTATE_PrevPage:
		SetList(Compositorium::Get()->GetNextGames(m_hPrevGame, NUM_GAMES_PER_PAGE));
		break;
	}
}
