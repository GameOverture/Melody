#ifndef Docket_h__
#define Docket_h__

#include "pch.h"
#include "IComponent.h"

#define NUM_DOCKET_GAMES 6

class NowPlaying;

class Docket : public IComponent
{
	NowPlaying &			m_NowPlayingRef;

	HyTexturedQuad2d		m_Thumbnails[NUM_DOCKET_GAMES];
	HyButton				m_CtrlPanel_ThumbnailBtn[NUM_DOCKET_GAMES];
	HyButton				m_CtrlPanel_BrowseBtn[NUM_DOCKET_GAMES];
	HyButton				m_CtrlPanel_ClearBtn[NUM_DOCKET_GAMES];

	enum ReloadState
	{
		RELOADSTATE_Idle,
		RELOADSTATE_FadingOut,
		RELOADSTATE_Reload,
	};
	ReloadState						m_eReloadState;
	
	std::vector<GameStats>			m_GameList;

public:
	Docket(NowPlaying &nowPlayingRef, HyEntity2d *pParent = nullptr);
	virtual ~Docket();

	virtual void PopulateCtrlPanel(CtrlPanel &ctrlPanel) override;
	virtual void Show(float fDuration) override;
	virtual void Hide(float fDuration) override;

	void AddGame(uint32 uiIndex, std::string sGameId);
	void ClearGame(uint32 uiIndex);

protected:
	virtual void OnUpdate() override;

	void SaveDocketSettings();
};

#endif // Docket_h__
