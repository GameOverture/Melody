#ifndef GameBrowser_h__
#define GameBrowser_h__

#include "pch.h"
#include "IComponent.h"
#include "ConsolePage.h"
#include "BrowsePage.h"
#include "EditPage.h"

class GameBrowser : public IComponent
{
	HyTextField						m_CtrlPanel_MobyGameQuery;
	HyButton						m_CtrlPanel_MobySearchBtn;

	HyButton						m_CtrlPanel_SetGameBtn;
	HyButton						m_CtrlPanel_SaveBtn;

	std::string						m_sHtmlFilePath;

	enum PixelBookState
	{
		PIXELBOOK_Closed = 0,
		PIXELBOOK_Opening,
		PIXELBOOK_PageFwd,
		PIXELBOOK_PageBck,
	};
	HySprite2d						m_PixelBook;

	enum State
	{
		STATE_Inactive = 0,
		STATE_BookIntro,
		STATE_Consoles,
		STATE_Browse,
		STATE_Edit,

		STATE_MobyImport,
		STATE_MobySelectGame
	};
	State							m_eState;
	
	ConsolePage						m_ConsolePage;
	BrowsePage						m_BrowsePage;
	EditPage						m_EditPage;

	HyJsonDoc						m_MobyResponseDoc;
	HyRadioDialog					m_MobySelectDlg;

public:
	GameBrowser(HyEntity2d *pParent = nullptr);
	virtual ~GameBrowser();

	virtual void PopulateCtrlPanel(CtrlPanel &ctrlPanel) override;
	virtual void Show(float fDuration) override;
	virtual void Hide(float fDuration) override;

	void ShowConsoles();
	void BrowseAtGame(GameInfo gameInfo);
	void SetGame(HyTexturedQuad2d &boxartRef, glm::vec2 ptBoxartPos, GameStats &gameStats);

	void NextPage();
	void PrevPage();

	bool IsShowing() const;

	bool TryImportMoby(const std::string &sResponse);

protected:
	virtual void OnUpdate() override;
};

#endif // GameBrowser_h__
