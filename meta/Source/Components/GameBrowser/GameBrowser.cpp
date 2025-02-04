#include "pch.h"
#include "GameBrowser.h"
#include "CtrlPanel.h"
#include "Compositorium.h"

#include <fstream>

GameBrowser::GameBrowser(HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_GameBrowser, pParent),
	m_CtrlPanel_SetGameBtn(HyPanelInit(32, 32, 2), HyNodePath("", "CtrlPanel")),
	m_CtrlPanel_SaveBtn(HyPanelInit(32, 32, 2), HyNodePath("", "CtrlPanel")),
	m_ePageState(PAGE_Consoles),
	m_ConsolePage(this),
	m_BrowsePage(this),
	m_EditPage(this)
{
	m_CtrlPanel_CheckBox.SetText("Game Browser");
	m_CtrlPanel_CheckBox.SetCheckedChangedCallback(
		[this](HyCheckBox *pCheckBox)
		{
			if(pCheckBox->IsChecked())
				Show(0.5f);
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

	ShowConsoles();
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
	IComponent::Show(fDuration);
}

/*virtual*/ void GameBrowser::Hide(float fDuration) /*override*/
{
	IComponent::Hide(fDuration);
}

void GameBrowser::ShowConsoles()
{
	m_ConsolePage.SetVisible(true);
	m_ConsolePage.SetInputAllowed(true);

	m_BrowsePage.SetVisible(false);
	m_BrowsePage.SetInputAllowed(false);

	m_EditPage.SetVisible(false);
	m_EditPage.SetInputAllowed(false);
	
	m_ePageState = PAGE_Consoles;
}

void GameBrowser::BrowseAtGame(GameInfo gameInfo)
{
	m_BrowsePage.BrowseAtGame(gameInfo);

	m_ConsolePage.SetVisible(false);
	m_ConsolePage.SetInputAllowed(false);

	m_BrowsePage.SetVisible(true);
	m_BrowsePage.SetInputAllowed(true);

	m_EditPage.SetVisible(false);
	m_EditPage.SetInputAllowed(false);

	m_ePageState = PAGE_Browse;
}

void GameBrowser::SetGame(GameStats &gameStats)
{
	m_EditPage.SetGame(gameStats);

	m_ConsolePage.SetVisible(false);
	m_ConsolePage.SetInputAllowed(false);

	m_BrowsePage.SetVisible(false);
	m_BrowsePage.SetInputAllowed(false);

	m_EditPage.SetVisible(true);
	m_EditPage.SetInputAllowed(true);

	m_ePageState = PAGE_Edit;
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
		m_ePageState = PAGE_Browse;

		m_sHtmlFilePath.clear();
	}
}
