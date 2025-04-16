#include "pch.h"
#include "Docket.h"
#include "CtrlPanel.h"
#include "Compositorium.h"
#include "NowPlaying.h"

#include <fstream>

Docket::Docket(NowPlaying &nowPlayingRef, HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_Docket, pParent),
	m_NowPlayingRef(nowPlayingRef)
{
	for(int i = 0; i < NUM_DOCKET_GAMES; ++i)
	{
		m_CtrlPanel_ThumbnailBtn[i].Setup(HyUiPanelInit(32, 32, 1), HyUiTextInit());
		m_CtrlPanel_ThumbnailBtn[i].SetTag(i);
		m_CtrlPanel_ThumbnailBtn[i].SetButtonClickedCallback(
			[this](HyButton *pButton)
			{
				m_NowPlayingRef.SetGame(pButton->GetUtf8String());
			});

		m_CtrlPanel_BrowseBtn[i].Setup(HyUiPanelInit(16, 16, 1), HyUiTextInit("CtrlPanel"));
		m_CtrlPanel_BrowseBtn[i].SetText("+");
		m_CtrlPanel_BrowseBtn[i].SetTag(i);
		m_CtrlPanel_BrowseBtn[i].SetButtonClickedCallback(
			[this](HyButton *pButton)
			{
				std::string sHtmlFilePath = Compositorium::Get()->OpenHtmlFileDlg();
				if(sHtmlFilePath.empty() == false)
				{
					std::string sGameId;
					// Read the first line of the HTML file which is the URL key
					std::ifstream infile(sHtmlFilePath, std::ifstream::in | std::ios::binary | std::ios::ate);
					HyAssert(infile, "HyReadTextFile invalid file: " << sHtmlFilePath);
					infile.seekg(0, std::ios::beg);
					std::getline(infile, sGameId);
					infile.close();
					sGameId.erase(std::remove_if(sGameId.begin(), sGameId.end(), [](char c) { return std::isspace(c); }), sGameId.end()); // Remove whitespace and newline characters
					sGameId.erase(0, 30); // Remove the first portion of the sUrlKey "https://gamefaqs.gamespot.com/"

					AddGame(pButton->GetTag(), sGameId);
				}
			});

		m_CtrlPanel_ClearBtn[i].Setup(HyUiPanelInit(16, 16, 1), HyUiTextInit("CtrlPanel"));
		m_CtrlPanel_ClearBtn[i].SetText("X");
		m_CtrlPanel_ClearBtn[i].SetTag(i);
		m_CtrlPanel_ClearBtn[i].SetButtonClickedCallback(
			[this](HyButton *pButton)
			{
				m_CtrlPanel_ThumbnailBtn[pButton->GetTag()].SetText("");
			});
	}

	m_CtrlPanel_CheckBox.SetText("Show Docket");
	m_CtrlPanel_CheckBox.SetCheckedChangedCallback(
		[this](HyCheckBox *pCheckBox)
		{
			if(pCheckBox->IsChecked())
				Show(0.5f);
			else
				Hide(0.5f);
		});
}

/*virtual*/ Docket::~Docket()
{
}

/*virtual*/ void Docket::PopulateCtrlPanel(CtrlPanel &ctrlPanel) /*override*/
{
	HyLayoutHandle hRow = ctrlPanel.InsertLayout(HYORIENT_Horizontal);
	ctrlPanel.InsertWidget(m_CtrlPanel_CheckBox, hRow);
	ctrlPanel.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);
	
	const int iNUM_PER_ROW = 3;
	// Place 'iNUM_PER_ROW' button groups in a row
	hRow = HY_UNUSED_HANDLE;
	for(int i = 0; i < NUM_DOCKET_GAMES; ++i)
	{
		if(i % iNUM_PER_ROW == 0)
			hRow = ctrlPanel.InsertLayout(HYORIENT_Horizontal);

		ctrlPanel.InsertWidget(m_CtrlPanel_ThumbnailBtn[i], hRow);
		ctrlPanel.InsertWidget(m_CtrlPanel_BrowseBtn[i], hRow);
		ctrlPanel.InsertWidget(m_CtrlPanel_ClearBtn[i], hRow);
	}
}

/*virtual*/ void Docket::Show(float fDuration) /*override*/
{
	IComponent::Show(fDuration);
}

/*virtual*/ void Docket::Hide(float fDuration) /*override*/
{
}

void Docket::AddGame(uint32 uiIndex, std::string sGameId)
{
	m_CtrlPanel_ThumbnailBtn[uiIndex].SetText(sGameId);

	GameInfo game = Compositorium::Get()->GetGame(sGameId);
	std::string sMediaPath = Compositorium::Get()->GetMediaPath(game.GetConsole(), MEDIATYPE_Boxarts);
	std::string sBoxartPath = Compositorium::Get()->GetBestMedia(game, MEDIATYPE_Boxarts);
	std::string sPath = sMediaPath + sBoxartPath;

	m_CtrlPanel_ThumbnailBtn[uiIndex].Setup(HyUiPanelInit(HYTYPE_TexturedQuad, HyNodePath(sPath.c_str()), 32, 32));
	m_CtrlPanel_ThumbnailBtn[uiIndex].Load();
}

void Docket::ClearGame(uint32 uiIndex)
{
	m_CtrlPanel_ThumbnailBtn[uiIndex].SetText("");
}

void Docket::LoadFile()
{
	std::string sFilePath = Compositorium::Get()->GetRootPath() + "Docket.txt";
	std::vector<char> sFileContents;
	HyIO::ReadTextFile(sFilePath.c_str(), sFileContents);
	
	m_GameList.clear();
	std::string sLine;
	std::istringstream iss(sFileContents.data());
	while(std::getline(iss, sLine))
	{
		std::istringstream issLine(sLine);
		std::string sConsole;
		std::string sGameId;
		double dElapsedTime;
		std::getline(issLine, sConsole, ';');
		std::getline(issLine, sGameId, ';');
		issLine >> dElapsedTime;
		//GameStats game;
		GameConsole eConsole = Compositorium::Get()->GetConsoleFromName(sConsole);
		//std::string sGameId = 
		//game.m_sGameId = sGameId;
		//game.m_dElapsedPlayTime = dElapsedTime;
		//m_GameList.push_back(game);
	}
}

void Docket::SaveFile()
{
	std::string sFilePath = Compositorium::Get()->GetRootPath() + "Docket.txt";

	std::string sFileContents;
	//for(auto &game : m_GameList)
	//	sFileContents += Compositorium::Get()->GetConsoleName(game.m_eConsole) + ";" + game.m_sGameId + ";" + std::to_string(game.m_dElapsedPlayTime) + "\n";

	HyIO::WriteTextFile(sFilePath.c_str(), sFileContents.c_str());
}

/*virtual*/ void Docket::OnUpdate() /*override*/
{
	switch(m_eReloadState)
	{
	case RELOADSTATE_Idle:
		break;

	case RELOADSTATE_FadingOut: {
		break; }

	case RELOADSTATE_Reload:
		break;
	}
}
