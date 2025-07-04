#include "pch.h"
#include "GameBrowser.h"
#include "CtrlPanel.h"
#include "Compositorium.h"

#include <fstream>

GameBrowser::GameBrowser(HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_GameBrowser, pParent),
	m_CtrlPanel_MobyGameQuery(HyUiPanelInit(200, 32, 2), HyNodePath("", "CtrlPanel")),
	m_CtrlPanel_MobySearchBtn(HyUiPanelInit(32, 32, 2), HyNodePath("", "CtrlPanel")),
	m_CtrlPanel_SetGameBtn(HyUiPanelInit(32, 32, 2), HyNodePath("", "CtrlPanel")),
	m_CtrlPanel_SaveBtn(HyUiPanelInit(32, 32, 2), HyNodePath("", "CtrlPanel")),
	m_PixelBook("PixelBook", this),
	m_eState(STATE_Inactive),
	m_ConsolePage(this),
	m_BrowsePage(this),
	m_EditPage(this),
	m_MobySelectDlg("Importing Game Data", std::vector<std::string>(), HyUiPanelInit(420, 420, 2), HyUiPanelInit(), HyUiTextInit("CtrlPanel"), this)
{
	m_CtrlPanel_MobySearchBtn.SetText("Moby");
	m_CtrlPanel_MobySearchBtn.SetButtonClickedCallback(
		[this](HyButton *pButton)
		{
			std::string sGameName = m_CtrlPanel_MobyGameQuery.GetUtf8String();
			if(sGameName.empty())
				return;
			std::string sResponse = Compositorium::Get()->GetMobyGame(sGameName);
			if(TryImportMoby(sResponse))
				m_eState = STATE_MobyImport;
		});

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

	m_PixelBook.scale.Set(2.5f, 2.5f);

	m_ConsolePage.Hide(true);
	m_ConsolePage.pos.Set(GAMEBROWSER_WIDTH * -0.5f, GAMEBROWSER_HEIGHT * -0.5f);
	
	m_BrowsePage.Hide(true);
	m_BrowsePage.pos.Set(GAMEBROWSER_WIDTH * -0.5f, GAMEBROWSER_HEIGHT * -0.5f);
	
	m_EditPage.Hide(true);
	m_EditPage.pos.Set(GAMEBROWSER_WIDTH * -0.5f, GAMEBROWSER_HEIGHT * -0.5f);

	m_MobySelectDlg.Hide(true);
}

/*virtual*/ GameBrowser::~GameBrowser()
{
}

/*virtual*/ void GameBrowser::PopulateCtrlPanel(CtrlPanel &ctrlPanel) /*override*/
{
	HyLayoutHandle hMobyRow = ctrlPanel.InsertLayout(HYORIENT_Horizontal);
	ctrlPanel.InsertWidget(m_CtrlPanel_MobyGameQuery, hMobyRow);
	ctrlPanel.InsertWidget(m_CtrlPanel_MobySearchBtn, hMobyRow);

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
	pos.Set(0.0f, -1500.0f);
	pos.Tween(0.0f, 100.0f, fDuration, HyTween::QuadInOut, 0.0f,
		[](IHyNode *pThis)
		{
			static_cast<GameBrowser *>(pThis)->pos.Tween(0.0f, 0.0f, 0.5f, HyTween::QuadIn);
		});


	rot.Set(-20.0f);
	rot.Tween(8.0f, fDuration, HyTween::QuadInOut, 0.0f,
		[](IHyNode *pThis)
		{
			static_cast<GameBrowser *>(pThis)->rot.Tween(0.0f, 1.5f, HyTween::BounceOut);
		});

	Defer(fDuration - 0.25f,
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

void GameBrowser::ShowConsoles()
{
	m_PixelBook.SetState(PIXELBOOK_PageFwd);
	m_PixelBook.SetAnimCtrl(HYANIMCTRL_ResetAndPlay);

	m_BrowsePage.SetInputAllowed(false);
	m_EditPage.SetInputAllowed(false);
	if(m_BrowsePage.IsVisible())
	{
		m_BrowsePage.alpha.Tween(0.0f, 0.75f, HyTween::Linear, 0.0f,
			[this](IHyNode *pThis)
			{
				static_cast<BrowsePage *>(pThis)->SetVisible(false);
			});
	}
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
	m_ConsolePage.SetVisible(true);
	m_ConsolePage.alpha.Set(0.0f);
	m_ConsolePage.alpha.Tween(1.0f, 0.75f, HyTween::Linear, 0.0f,
		[](IHyNode *pThis)
		{
			static_cast<ConsolePage *>(pThis)->SetInputAllowed(true);
		});
}

void GameBrowser::BrowseAtGame(GameInfo gameInfo)
{
	m_PixelBook.SetState(PIXELBOOK_PageFwd);
	m_PixelBook.SetAnimCtrl(HYANIMCTRL_ResetAndPlay);

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

void GameBrowser::SetGame(HyTexturedQuad2d &boxartRef, glm::vec2 ptBoxartPos, GameStats &gameStats)
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

	m_EditPage.SetGame(boxartRef, ptBoxartPos, gameStats);
	m_EditPage.SetVisible(true);
	m_EditPage.alpha.Tween(1.0f, 0.75f, HyTween::Linear, 0.0f,
		[](IHyNode *pThis)
		{
			static_cast<EditPage *>(pThis)->SetInputAllowed(true);
		});

	//m_ePageState = PAGE_Edit;
}

void GameBrowser::NextPage()
{
	m_PixelBook.SetState(PIXELBOOK_PageFwd);
	m_PixelBook.SetAnimCtrl(HYANIMCTRL_ResetAndPlay);
}

void GameBrowser::PrevPage()
{
	m_PixelBook.SetState(PIXELBOOK_PageBck);
	m_PixelBook.SetAnimCtrl(HYANIMCTRL_ResetAndPlay);
}

bool GameBrowser::IsShowing() const
{
	return m_eState > STATE_BookIntro;
}

bool GameBrowser::TryImportMoby(const std::string &sResponse)
{
	// Error check the response
	m_MobyResponseDoc.SetObject();
	if(m_MobyResponseDoc.Parse(sResponse.c_str()).HasParseError())
	{
		HyLogError("GameBrowser::ImportMoby: Failed to parse JSON response");
		return false;
	}
	HyAssert(m_MobyResponseDoc.IsObject(), "GameBrowser::ImportMoby - wasn't an object");
	if(m_MobyResponseDoc.HasMember("games") == false)
	{
		HyLogError("GameBrowser::ImportMoby: No games found in JSON response");
		return false;
	}
	HyAssert(m_MobyResponseDoc["games"].IsArray(), "GameBrowser::ImportMoby - wasn't an array");
	if(m_MobyResponseDoc["games"].Size() == 0)
	{
		HyLogWarning("GameBrowser::ImportMoby: No games found in JSON response");
		return false;
	}

	std::vector<std::string> sGameList;
	HyJsonArray gamesArray = m_MobyResponseDoc["games"].GetArray();
	for(size_t i = 0; i < gamesArray.Size(); ++i)
	{
		HyJsonObj gameObj = gamesArray[i].GetObject();
		if(gameObj.HasMember("game_id") == false || gameObj["game_id"].IsString() == false)
		{
			HyLogError("GameBrowser::ImportMoby: No game_id found in JSON response");
			return false;
		}
		std::string sId = gameObj["game_id"].GetString();
		
		if(gameObj.HasMember("title") == false || gameObj["title"].IsString() == false)
		{
			HyLogError("GameBrowser::ImportMoby: No title found in JSON response");
			return false;
		}
		sGameList.push_back(gameObj["title"].GetString());
	}

	m_MobySelectDlg.SetRadioList(sGameList);
	m_MobySelectDlg.Show();

	return true;
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

		GameInfo gameInfo = Compositorium::Get()->GetGame(sUrlKey);
		GameStats gameStats = Compositorium::Get()->GetGameStats(gameInfo);

		m_BrowsePage.BrowseAtGame(gameInfo);
		m_sHtmlFilePath.clear();
	}

	switch(m_eState)
	{
	case STATE_Inactive:
		break;

	case STATE_BookIntro:
		if(m_PixelBook.GetState() == PIXELBOOK_Opening)
		{
			m_ConsolePage.Show();
			m_ConsolePage.alpha.Tween(1.0f, 0.75f, HyTween::Linear, 0.0f,
				[this](IHyNode *pThis)
				{
					m_ConsolePage.SetInputAllowed(true);
				});

			m_eState = STATE_Consoles;
		}
		break;
			
	case STATE_Consoles:
		break;
			
	case STATE_Browse:
		break;
	
	case STATE_Edit:
		break;

	case STATE_MobyImport: { // Already error checked 'm_MobyResponseDoc' 
		HyJsonArray gamesArray = m_MobyResponseDoc["games"].GetArray();
		std::vector<std::string> sGameNames;
		for(size_t i = 0; i < gamesArray.Size(); ++i)
		{
			HyJsonObj gameObj = gamesArray[i].GetObject();
			if(gameObj.HasMember("title") == false)
				continue;
			sGameNames.push_back(gameObj["title"].GetString());
		}
		m_MobySelectDlg.SetRadioList(sGameNames);
		m_MobySelectDlg.Show();

		m_eState = STATE_MobySelectGame;
		break; }

	case STATE_MobySelectGame:
		break;
	}
}
