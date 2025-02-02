#ifndef GameBrowser_h__
#define GameBrowser_h__

#include "pch.h"
#include "IComponent.h"
#include "BrowsePage.h"
#include "EditPage.h"

class GameBrowser : public IComponent
{
	HyButton						m_CtrlPanel_SetGameBtn;
	HyButton						m_CtrlPanel_SaveBtn;

	std::string						m_sHtmlFilePath;

	enum PageType
	{
		PAGE_Browse = 0,
		PAGE_Edit,
	};
	PageType						m_ePageState;
	BrowsePage						m_BrowsePage;
	EditPage						m_EditPage;

public:
	GameBrowser(HyEntity2d *pParent = nullptr);
	virtual ~GameBrowser();

	virtual void PopulateCtrlPanel(CtrlPanel &ctrlPanel) override;
	virtual void Show(float fDuration) override;
	virtual void Hide(float fDuration) override;

protected:
	virtual void OnUpdate() override;
};

#endif // GameBrowser_h__
