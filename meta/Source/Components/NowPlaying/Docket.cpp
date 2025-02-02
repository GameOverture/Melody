#include "pch.h"
#include "Docket.h"
#include "CtrlPanel.h"
#include "Compositorium.h"

#include <fstream>

Docket::Docket(HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_Docket, pParent),
	m_CtrlPanel_AddGameBtn(HyPanelInit(20, 20, 2), HyNodePath("", "CtrlPanel")),
	m_CtrlPanel_SaveBtn(HyPanelInit(32, 32, 2), HyNodePath("", "CtrlPanel")),
	m_CtrlPanel_ShowCollection(HyPanelInit(32, 32, 2), HyNodePath("", "CtrlPanel"))
{
	m_CtrlPanel_CheckBox.SetText("Docket");
	m_CtrlPanel_CheckBox.SetCheckedChangedCallback(
		[this](HyCheckBox *pCheckBox)
		{
			if(pCheckBox->IsChecked())
				Show(0.5f);
			else
				Hide(0.5f);
		});

	m_CtrlPanel_AddGameBtn.SetText("+");
	m_CtrlPanel_AddGameBtn.SetButtonClickedCallback(
		[this](HyButton *pButton)
		{
			m_sHtmlFilePath = Compositorium::Get()->OpenHtmlFileDlg();
		});

	m_CtrlPanel_SaveBtn.SetText("Save");
	m_CtrlPanel_SaveBtn.SetButtonClickedCallback(
		[this](HyButton *pButton)
		{
			SaveFile();
		});

	m_CtrlPanel_ShowCollection.SetText("Open");
	m_CtrlPanel_ShowCollection.SetCheckedChangedCallback(
		[this](HyCheckBox *pCheckBox)
		{
		});
}

/*virtual*/ Docket::~Docket()
{
}

/*virtual*/ void Docket::PopulateCtrlPanel(CtrlPanel &ctrlPanel) /*override*/
{
	HyLayoutHandle hRow = ctrlPanel.InsertLayout(HYORIENT_Horizontal);
	ctrlPanel.InsertWidget(m_CtrlPanel_CheckBox, hRow);
	ctrlPanel.InsertWidget(m_CtrlPanel_AddGameBtn, hRow);
	ctrlPanel.InsertWidget(m_CtrlPanel_SaveBtn, hRow);
	ctrlPanel.InsertWidget(m_CtrlPanel_ShowCollection, hRow);
	ctrlPanel.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);
}

/*virtual*/ void Docket::Show(float fDuration) /*override*/
{
	IComponent::Show(fDuration);
}

/*virtual*/ void Docket::Hide(float fDuration) /*override*/
{
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


		//GameStats game;
		//game.m_eConsole = obj.GetConsole();
		//game.m_sGameId = obj.GetGameId();
		//game.m_dElapsedPlayTime = 0.0;
		//m_GameList.push_back(game);

		m_sHtmlFilePath.clear();
	}

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
