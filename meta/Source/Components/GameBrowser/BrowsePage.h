#ifndef BrowsePage_h__
#define BrowsePage_h__

#include "pch.h"

#include <array>

#define NUM_GAMES_PER_PAGE	8

class BrowsePage : public HyUiContainer
{
	HyButton				m_PrevBtn;
	GameInfo				m_hPrevGame;
	HyLabel					m_GameTitleLabels[NUM_GAMES_PER_PAGE];
	HyButton				m_GameBtns[NUM_GAMES_PER_PAGE];
	HyTexturedQuad2d		m_GameBoxarts[NUM_GAMES_PER_PAGE];
	int32					m_iHoverGameIndex;
	HyButton				m_NextBtn;
	GameInfo				m_hNextGame;

	HyButton				m_BackBtn;
	HyButton				m_AlphaJumpBtn[26];

	std::array<GameInfo, NUM_GAMES_PER_PAGE>	m_QueuedGamesArray;
	enum ReloadState
	{
		RELOADSTATE_Idle,
		RELOADSTATE_TryLoad,
		RELOADSTATE_Reload,
		RELOADSTATE_NextPage,
		RELOADSTATE_PrevPage
	};
	ReloadState				m_eReloadState;
	HyTimer					m_ReloadCooldownTimer;

public:
	BrowsePage(HyEntity2d *pParent = nullptr);
	virtual ~BrowsePage();

	void BrowseAtGame(GameInfo gameInfo);

	void SetList(std::array<GameInfo, NUM_GAMES_PER_PAGE> &gameList);

	void ReturnBoxart(HyTexturedQuad2d &boxartRef);

protected:
	void OnContainerUpdate() override;
};

#endif // BrowsePage_h__
