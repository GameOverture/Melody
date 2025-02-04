#ifndef BrowsePage_h__
#define BrowsePage_h__

#include "pch.h"

#define NUM_GAMES_PER_PAGE	10

class BrowsePage : public HyUiContainer
{
	HyButton				m_TitleLabel;

	HyButton				m_PrevBtn;
	GameInfo				m_hPrevGame;
	HyLabel					m_GameTitleLabels[NUM_GAMES_PER_PAGE];
	HyButton				m_GameBtns[NUM_GAMES_PER_PAGE];
	HyTexturedQuad2d		m_GameBoxarts[NUM_GAMES_PER_PAGE];
	int32					m_iHoverGameIndex;
	HyButton				m_NextBtn;
	GameInfo				m_hNextGame;

	HyButton				m_AlphaJumpBtn[26];

	std::vector<GameInfo>	m_QueuedGameList;
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

	void SetList(std::vector<GameInfo> &gameList);

protected:
	void OnContainerUpdate() override;
};

#endif // BrowsePage_h__
