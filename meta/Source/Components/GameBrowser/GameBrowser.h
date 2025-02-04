#ifndef GameBrowser_h__
#define GameBrowser_h__

#include "pch.h"
#include "IComponent.h"
#include "ConsolePage.h"
#include "BrowsePage.h"
#include "EditPage.h"

class GameBrowser : public IComponent
{
	HyButton						m_CtrlPanel_SetGameBtn;
	HyButton						m_CtrlPanel_SaveBtn;

	std::string						m_sHtmlFilePath;

	enum PageType
	{
		PAGE_Consoles = 0,
		PAGE_Browse,
		PAGE_Edit,
	};
	PageType						m_ePageState;
	
	ConsolePage						m_ConsolePage;
	BrowsePage						m_BrowsePage;
	EditPage						m_EditPage;

public:
	GameBrowser(HyEntity2d *pParent = nullptr);
	virtual ~GameBrowser();

	virtual void PopulateCtrlPanel(CtrlPanel &ctrlPanel) override;
	virtual void Show(float fDuration) override;
	virtual void Hide(float fDuration) override;

	void ShowConsoles();
	void BrowseAtGame(GameInfo gameInfo);
	void SetGame(GameStats &gameStats);

protected:
	virtual void OnUpdate() override;
};

#endif // GameBrowser_h__
