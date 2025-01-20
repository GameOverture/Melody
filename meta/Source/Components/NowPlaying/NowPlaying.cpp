#include "pch.h"
#include "NowPlaying.h"
#include "CtrlPanel.h"
#include "Melody.h"
#include <commdlg.h>
#include <fstream>

#define NOWPLAYING_BOUNCE_AMT 25

NowPlaying::NowPlaying(HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_NowPlaying, pParent),
	m_CtrlPanel_CheckBox(HyPanelInit(32, 32, 2), HyNodePath("", "CtrlPanel")),
	m_CtrlPanel_BrowseBtn(HyPanelInit(100, 32, 2), HyNodePath("", "CtrlPanel")),
	m_NowPlayingText("Label", this),
	m_iBoxArtIndex(-1),
	m_iTitleArtIndex(-1),
	m_Logo(this),
	m_Description("Description", this),
	m_bReloading(false)
{
	m_CtrlPanel_CheckBox.SetText("Now Playing");
	m_CtrlPanel_CheckBox.SetCheckedChangedCallback(
		[this](HyCheckBox *pCheckBox)
		{
			if(pCheckBox->IsChecked())
				Show(0.5f);
			else
				Hide(0.5f);
		});

	m_CtrlPanel_BrowseBtn.SetText("Browse");
	m_CtrlPanel_BrowseBtn.SetButtonClickedCallback(
		[this](HyButton *pButton)
		{
			// Browse for a file
			OPENFILENAME ofn;       // Common dialog box structure
			char szFile[260];       // Buffer for file name

			// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = GetConsoleWindow();  // Use the console window as parent
			ofn.lpstrFile = szFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = "HTML files\0*.HTML;*.HTM\0All files\0*.*\0";  // Filter for .html and .htm files
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = nullptr;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = nullptr;
			ofn.lpstrTitle = "Select an HTML file";
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			// Display the Open File dialog box
			if(GetOpenFileName(&ofn) == TRUE)
			{
				// Read the first line of 'ofn.lpstrFile' file which is the URL key
				std::ifstream infile(ofn.lpstrFile, std::ifstream::in | std::ios::binary | std::ios::ate);
				HyAssert(infile, "HyReadTextFile invalid file: " << ofn.lpstrFile);
				infile.seekg(0, std::ios::beg);
				std::string sUrlKey;
				std::getline(infile, sUrlKey);
				infile.close();
				// Remove whitespace and newline characters from sUrlKey
				sUrlKey.erase(std::remove_if(sUrlKey.begin(), sUrlKey.end(), [](char c) { return std::isspace(c); }), sUrlKey.end());
				sUrlKey.erase(0, 30); // Remove the first portion of the sUrlKey "https://gamefaqs.gamespot.com/"

				GameConsole eConsole = Compositorium::Get()->GetConsoleFromPath(ofn.lpstrFile);
				GameObj gameObj = Compositorium::Get()->GetGame(eConsole, sUrlKey);

				m_NowPlayingText.SetText(Compositorium::Get()->GetGameName(gameObj));

				std::string sBestMatchingLogoFile = Compositorium::Get()->GetBestMedia(gameObj, MEDIATYPE_Logos);
				m_Logo.Uninit();
				if(sBestMatchingLogoFile.empty() == false)
					m_Logo.Init(sBestMatchingLogoFile, HyTextureInfo(), this);

				for(int i = 0; i < m_BoxArtList.size(); ++i)
					delete m_BoxArtList[i];
				m_BoxArtList.clear();
				std::vector<std::string> boxartsFilePathList = Compositorium::Get()->GetMediaList(gameObj, MEDIATYPE_Boxarts);
				for(int i = 0; i < boxartsFilePathList.size(); ++i)
				{
					HyTexturedQuad2d *pNewBoxArt = HY_NEW HyTexturedQuad2d(boxartsFilePathList[i], HyTextureInfo(), this);
					m_BoxArtList.push_back(pNewBoxArt);
				}
				m_iBoxArtIndex = m_BoxArtList.empty() ? -1 : 0;

				for(int i = 0; i < m_TitleArtList.size(); ++i)
					delete m_TitleArtList[i];
				m_TitleArtList.clear();
				std::vector<std::string> titleartsFilePathList = Compositorium::Get()->GetMediaList(gameObj, MEDIATYPE_Titles);
				for(int i = 0; i < titleartsFilePathList.size(); ++i)
				{
					HyTexturedQuad2d *pNewTitleArt = HY_NEW HyTexturedQuad2d(titleartsFilePathList[i], HyTextureInfo(), this);
					m_TitleArtList.push_back(pNewTitleArt);
				}
				m_iTitleArtIndex = m_TitleArtList.empty() ? -1 : 0;

				m_Description.SetText(Compositorium::Get()->GetGameDescription(gameObj));
				m_Description.ParentDetach();
				ChildAppend(m_Description);

				Load();
				m_bReloading = true;
			}
		});

	m_Description.SetAsColumn(LIVESPLIT_WIDTH);
	m_Description.pos.Set(0, LIVESPLIT_HEIGHT / 2);
}

/*virtual*/ NowPlaying::~NowPlaying()
{
	for(int i = 0; i < m_BoxArtList.size(); ++i)
		delete m_BoxArtList[i];
	for(int i = 0; i < m_TitleArtList.size(); ++i)
		delete m_TitleArtList[i];
}

/*virtual*/ void NowPlaying::PopulateCtrlPanel(CtrlPanel &ctrlPanel) /*override*/
{
	HyLayoutHandle hRow = ctrlPanel.InsertLayout(HYORIENT_Horizontal);
	ctrlPanel.InsertWidget(m_CtrlPanel_CheckBox, hRow);
	ctrlPanel.InsertWidget(m_CtrlPanel_BrowseBtn, hRow);
	ctrlPanel.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);
}

/*virtual*/ void NowPlaying::OnUpdate() /*override*/
{
	if(m_bReloading && IsLoaded())
	{
		int iNOWPLAYING_HEIGHT = 100;
		if(m_Logo.IsLoadDataValid())
		{
			TransformTexture(m_Logo,
				glm::ivec2((LIVESPLIT_WIDTH / 3) * 2, iNOWPLAYING_HEIGHT),
				glm::vec2(LIVESPLIT_WIDTH / 2 + ((LIVESPLIT_WIDTH / 3) / 2), LIVESPLIT_HEIGHT));
			m_Logo.SetDisplayOrder(DISPLAYORDER_AboveMonitor);

			m_NowPlayingText.SetText("Now Playing");
			m_NowPlayingText.pos.Set(m_Logo.pos.GetX() - (LIVESPLIT_WIDTH / 3) + NOWPLAYING_BOUNCE_AMT, LIVESPLIT_HEIGHT - (iNOWPLAYING_HEIGHT * 0.5f));
			m_NowPlayingText.SetAsScaleBox((LIVESPLIT_WIDTH / 3) - NOWPLAYING_BOUNCE_AMT, iNOWPLAYING_HEIGHT);
			m_NowPlayingText.SetDisplayOrder(DISPLAYORDER_AboveMonitor);
		}
		else
			iNOWPLAYING_HEIGHT = 0;

		for(int i = 0; i < m_BoxArtList.size(); ++i)
		{
			m_BoxArtList[i]->SetVisible(i == 0);
			TransformTexture(*m_BoxArtList[i],
				glm::ivec2(LIVESPLIT_WIDTH, (LIVESPLIT_HEIGHT / 2) - (iNOWPLAYING_HEIGHT / 2)),
				glm::vec2(LIVESPLIT_WIDTH / 2, -(iNOWPLAYING_HEIGHT / 4) + (LIVESPLIT_HEIGHT / 2) + ((LIVESPLIT_HEIGHT / 2) * 0.5f)));
		}

		for(int i = 0; i < m_TitleArtList.size(); ++i)
		{
			m_TitleArtList[i]->SetVisible(false);
			TransformTexture(*m_TitleArtList[i],
				glm::ivec2(LIVESPLIT_WIDTH, (LIVESPLIT_HEIGHT / 2) - (iNOWPLAYING_HEIGHT / 2)),
				glm::vec2(LIVESPLIT_WIDTH / 2, -(iNOWPLAYING_HEIGHT / 4) + (LIVESPLIT_HEIGHT / 2) + ((LIVESPLIT_HEIGHT / 2) * 0.5f)));
		}

		for(int iDescTextState = m_Description.GetNumStates() - 1; iDescTextState >= 0; --iDescTextState)
		{
			m_Description.SetState(iDescTextState);
			if(m_Description.GetHeight() <= 275)
				break;
		}
		HyLog("Description State: " << m_Description.GetState());

		m_bReloading = false;
	}

	if(m_NowPlayingText.pos.IsAnimating() == false)
	{
		m_NowPlayingText.pos.TweenOffset(-NOWPLAYING_BOUNCE_AMT, 0, 1.0f, HyTween::QuadInOut, 20.0f,
			[](IHyNode *pThis)
			{
				static_cast<HyText2d *>(pThis)->pos.TweenOffset(NOWPLAYING_BOUNCE_AMT, 0, 0.5f, HyTween::BounceOut, 0.0f);
			});
	}

	//if(m_iBoxArtIndex
}
