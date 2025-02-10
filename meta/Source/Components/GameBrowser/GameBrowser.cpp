#include "pch.h"
#include "GameBrowser.h"
#include "CtrlPanel.h"
#include "Compositorium.h"

#include <fstream>

GameBrowser::GameBrowser(HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_GameBrowser, pParent),
	m_CtrlPanel_SetGameBtn(HyPanelInit(32, 32, 2), HyNodePath("", "CtrlPanel")),
	m_CtrlPanel_SaveBtn(HyPanelInit(32, 32, 2), HyNodePath("", "CtrlPanel")),
	m_PixelBook("PixelBook", this),
	m_eState(STATE_Inactive),
	m_ConsolePage(this),
	m_BrowsePage(this),
	m_EditPage(this)
{
	m_CtrlPanel_CheckBox.SetText("Game Browser");
	m_CtrlPanel_CheckBox.SetCheckedChangedCallback(
		[this](HyCheckBox *pCheckBox)
		{
			if(pCheckBox->IsChecked())
				Show(1.5f);
			else
				Hide(0.5f);
		});

	m_CtrlPanel_SetGameBtn.SetText("Open");
	m_CtrlPanel_SetGameBtn.SetButtonClickedCallback(
		[this](HyButton *pButton)
		{
			m_sHtmlFilePath = Compositorium::Get()->OpenHtmlFileDlg();
		});

	m_CtrlPanel_SaveBtn.SetText("Save");
	m_CtrlPanel_SaveBtn.SetButtonClickedCallback(
		[this](HyButton *pButton)
		{
			//SaveFile();
		});

	m_PixelBook.scale.Set(2.6f, 2.6f);

	m_ConsolePage.Hide(true);
	m_ConsolePage.pos.Set(GAMEBROWSER_WIDTH * -0.5f, GAMEBROWSER_HEIGHT * -0.5f);
	
	m_BrowsePage.Hide(true);
	m_BrowsePage.pos.Set(GAMEBROWSER_WIDTH * -0.5f, GAMEBROWSER_HEIGHT * -0.5f);
	
	m_EditPage.Hide(true);
	m_EditPage.pos.Set(GAMEBROWSER_WIDTH * -0.5f, GAMEBROWSER_HEIGHT * -0.5f);
}

/*virtual*/ GameBrowser::~GameBrowser()
{
}

/*virtual*/ void GameBrowser::PopulateCtrlPanel(CtrlPanel &ctrlPanel) /*override*/
{
	HyLayoutHandle hRow = ctrlPanel.InsertLayout(HYORIENT_Horizontal);
	ctrlPanel.InsertWidget(m_CtrlPanel_CheckBox, hRow);
	ctrlPanel.InsertWidget(m_CtrlPanel_SetGameBtn, hRow);
	ctrlPanel.InsertWidget(m_CtrlPanel_SaveBtn, hRow);
	ctrlPanel.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);
}

/*virtual*/ void GameBrowser::Show(float fDuration) /*override*/
{
	SetVisible(true);

	m_PixelBook.SetState(PIXELBOOK_Closed);
	m_PixelBook.pos.Set(0.0f, -1500.0f);
	m_PixelBook.pos.Tween(0.0f, 0.0f, fDuration, HyTween::QuadInOut);
	m_PixelBook.rot.Set(-20.0f);
	m_PixelBook.rot.Tween(.0f, fDuration, HyTween::QuadInOut);

	Defer(fDuration - 0.5f,
		[this](HyEntity2d *pThis)
		{
			m_PixelBook.SetState(PIXELBOOK_Opening);
		});
	
	m_eState = STATE_BookIntro;
}

/*virtual*/ void GameBrowser::Hide(float fDuration) /*override*/
{
	SetVisible(false);
}

void GameBrowser::BrowseAtGame(GameInfo gameInfo)
{
	m_EditPage.SetInputAllowed(false);
	m_ConsolePage.SetInputAllowed(false);

	if(m_EditPage.IsVisible())
	{
		HyTexturedQuad2d *pBoxart = m_EditPage.GetBoxart();
		if(pBoxart)
			m_BrowsePage.ReturnBoxart(*pBoxart);
		
		m_EditPage.alpha.Tween(0.0f, 0.75f, HyTween::Linear, 0.0f,
			[this](IHyNode *pThis)
			{
				static_cast<EditPage *>(pThis)->SetVisible(false);
			});
	}
	
	if(m_ConsolePage.IsVisible())
	{
		m_ConsolePage.alpha.Tween(0.0f, 0.75f, HyTween::Linear, 0.0f,
			[this](IHyNode *pThis)
			{
				static_cast<ConsolePage *>(pThis)->SetVisible(false);
			});
	}

	m_BrowsePage.BrowseAtGame(gameInfo);
	m_BrowsePage.SetVisible(true);
	m_BrowsePage.alpha.Set(0.0f);
	m_BrowsePage.alpha.Tween(1.0f, 0.75f, HyTween::Linear, 0.0f,
		[](IHyNode *pThis)
		{
			static_cast<BrowsePage *>(pThis)->SetInputAllowed(true);
		});

	//m_ePageState = PAGE_Browse;
}

void GameBrowser::SetGame(HyTexturedQuad2d &boxartRef, GameStats &gameStats)
{
	m_ConsolePage.SetInputAllowed(false);
	m_BrowsePage.SetInputAllowed(false);

	if(m_BrowsePage.IsVisible())
	{
		m_BrowsePage.alpha.Tween(0.0f, 0.75f, HyTween::Linear, 0.0f,
			[this](IHyNode *pThis)
			{
				static_cast<BrowsePage *>(pThis)->SetVisible(false);
			});
	}
	
	if(m_ConsolePage.IsVisible())
	{
		m_ConsolePage.alpha.Tween(0.0f, 0.75f, HyTween::Linear, 0.0f,
			[this](IHyNode *pThis)
			{
				static_cast<ConsolePage *>(pThis)->SetVisible(false);
			});
	}

	m_EditPage.SetGame(boxartRef, gameStats);
	m_EditPage.SetVisible(true);
	m_EditPage.alpha.Tween(1.0f, 0.75f, HyTween::Linear, 0.0f,
		[](IHyNode *pThis)
		{
			static_cast<EditPage *>(pThis)->SetInputAllowed(true);
		});

	//m_ePageState = PAGE_Edit;
}

/*virtual*/ void GameBrowser::OnUpdate() /*override*/
{
	if(m_sHtmlFilePath.empty() == false)
	{
		// Read the first line of the HTML file which is the URL key
		std::string sUrlKey;
		std::ifstream infile(m_sHtmlFilePath, std::ifstream::in | std::ios::binary | std::ios::ate);
		HyAssert(infile, "HyReadTextFile invalid file: " << m_sHtmlFilePath);
		infile.seekg(0, std::ios::beg);
		std::getline(infile, sUrlKey);
		infile.close();
		sUrlKey.erase(std::remove_if(sUrlKey.begin(), sUrlKey.end(), [](char c) { return std::isspace(c); }), sUrlKey.end()); // Remove whitespace and newline characters
		sUrlKey.erase(0, 30); // Remove the first portion of the sUrlKey "https://gamefaqs.gamespot.com/"

		GameInfo gameInfo = Compositorium::Get()->GetGame(Compositorium::Get()->GetConsoleFromPath(m_sHtmlFilePath), sUrlKey);
		GameStats gameStats = Compositorium::Get()->GetGameStats(gameInfo);

		m_BrowsePage.BrowseAtGame(gameInfo);
		m_sHtmlFilePath.clear();
	}

	switch(m_eState)
	{
	case STATE_Inactive:
		break;

	case STATE_BookIntro:
		if(m_PixelBook.pos.IsAnimating() == false)
		{
			m_ConsolePage.Show();
			m_ConsolePage.alpha.Tween(1.0f, 0.75f, HyTween::Linear, 0.0f,
				[this](IHyNode *pThis)
				{
					m_ConsolePage.SetInputAllowed(true);
				});

		}
		break;
			
	case STATE_Consoles:
		break;
			
	case STATE_Browse:
		break;
	
	case STATE_Edit:
		break;
	}
}
