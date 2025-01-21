#include "pch.h"
#include "NowPlaying.h"
#include "CtrlPanel.h"
#include "Melody.h"
#include <commdlg.h>
#include <fstream>

#define NOWPLAYING_BOUNCE_AMT 25
#define NOWPLAYING_HEIGHT 100

NowPlaying::NowPlaying(HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_NowPlaying, pParent),
	m_CtrlPanel_CheckBox(HyPanelInit(32, 32, 2), HyNodePath("", "CtrlPanel")),
	m_CtrlPanel_BrowseBtn(HyPanelInit(100, 32, 2), HyNodePath("", "CtrlPanel")),
	m_InfoEnt(this),
	m_iBoxArtIndex(-1),
	m_iTitleArtIndex(-1),
	m_DescriptionText("Description", &m_InfoEnt),
	m_NowPlayingEnt(this),
	m_Logo(&m_NowPlayingEnt),
	m_NowPlayingText("Label", &m_NowPlayingEnt),
	m_GameNameText("MainText", &m_NowPlayingEnt),
	m_TimeEnt(this),
	m_TimeLabel(HyPanelInit(LIVESPLIT_WIDTH * 0.6f, 100, 1, HyColor::Black), "MainText", &m_TimeEnt),
	m_GameTimeText("MainText", &m_TimeEnt),
	m_eReloadState(RELOADSTATE_Idle)
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
				m_sHtmlFilePath = ofn.lpstrFile;
				
				m_NowPlayingEnt.SetVisible(true);
				m_NowPlayingEnt.alpha.Set(1.0f);
				m_NowPlayingEnt.alpha.Tween(0.0f, 1.0f);

				m_InfoEnt.SetVisible(true);
				m_InfoEnt.alpha.Set(0.0f);
				m_InfoEnt.alpha.Tween(0.0f, 1.0f);

				m_eReloadState = RELOADSTATE_FadingOut;
			}
		});

	m_NowPlayingEnt.SetVisible(false);
	m_NowPlayingEnt.SetDisplayOrder(DISPLAYORDER_AboveMonitor);
	m_NowPlayingText.SetText("Now Playing");
	m_NowPlayingText.SetAsScaleBox((LIVESPLIT_WIDTH / 3) - NOWPLAYING_BOUNCE_AMT, NOWPLAYING_HEIGHT);
	
	m_InfoEnt.SetVisible(false);
	m_DescriptionText.SetAsColumn(LIVESPLIT_WIDTH);
	m_DescriptionText.pos.Set(0, LIVESPLIT_HEIGHT / 2);

	m_GameTimeText.SetText("Game Time:");
	m_GameTimeText.pos.Set(25, 75);
	m_GameTimeText.SetAsScaleBox(LIVESPLIT_WIDTH * 0.3f, 50);
	m_TimeLabel.pos.Set(100, 0);
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
	if(m_NowPlayingText.pos.IsAnimating() == false)
	{
		m_NowPlayingText.pos.TweenOffset(-NOWPLAYING_BOUNCE_AMT, 0, 1.0f, HyTween::QuadInOut, 20.0f,
			[](IHyNode *pThis)
			{
				static_cast<HyText2d *>(pThis)->pos.TweenOffset(NOWPLAYING_BOUNCE_AMT, 0, 0.5f, HyTween::BounceOut, 0.0f);
			});
	}

	m_TimeLabel.SetText(m_Stopwatch.ToString());

	switch(m_eReloadState)
	{
	case RELOADSTATE_Idle:
		break;

	case RELOADSTATE_FadingOut:
		if(m_NowPlayingEnt.alpha.IsAnimating() == false && m_InfoEnt.alpha.IsAnimating() == false)
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

			// 
			GameObj gameObj = Compositorium::Get()->GetGame(Compositorium::Get()->GetConsoleFromPath(m_sHtmlFilePath), sUrlKey);

			m_GameNameText.SetText(Compositorium::Get()->GetGameName(gameObj));

			std::string sBestMatchingLogoFile = Compositorium::Get()->GetBestMedia(gameObj, MEDIATYPE_Logos);
			m_Logo.Uninit();
			if(sBestMatchingLogoFile.empty() == false)
				m_Logo.Init(sBestMatchingLogoFile, HyTextureInfo(), &m_NowPlayingEnt);

			for(int i = 0; i < m_BoxArtList.size(); ++i)
				delete m_BoxArtList[i];
			m_BoxArtList.clear();
			std::vector<std::string> boxartsFilePathList = Compositorium::Get()->GetMediaList(gameObj, MEDIATYPE_Boxarts);
			for(int i = 0; i < boxartsFilePathList.size(); ++i)
			{
				HyTexturedQuad2d *pNewBoxArt = HY_NEW HyTexturedQuad2d(boxartsFilePathList[i], HyTextureInfo(), &m_InfoEnt);
				m_BoxArtList.push_back(pNewBoxArt);
			}
			m_iBoxArtIndex = m_BoxArtList.empty() ? -1 : 0;

			for(int i = 0; i < m_TitleArtList.size(); ++i)
				delete m_TitleArtList[i];
			m_TitleArtList.clear();
			std::vector<std::string> titleartsFilePathList = Compositorium::Get()->GetMediaList(gameObj, MEDIATYPE_Titles);
			for(int i = 0; i < titleartsFilePathList.size(); ++i)
			{
				HyTexturedQuad2d *pNewTitleArt = HY_NEW HyTexturedQuad2d(titleartsFilePathList[i], HyTextureInfo(), &m_InfoEnt);
				m_TitleArtList.push_back(pNewTitleArt);
			}
			m_iTitleArtIndex = m_TitleArtList.empty() ? -1 : 0;

			m_DescriptionText.SetText(Compositorium::Get()->GetGameDescription(gameObj));
			m_DescriptionText.ParentDetach();
			m_InfoEnt.ChildAppend(m_DescriptionText);

			Load();
			m_eReloadState = RELOADSTATE_Reload;
		}
		break;

	case RELOADSTATE_Reload:
		if(IsLoaded())
		{
			m_NowPlayingText.SetVisible(true);

			if(m_Logo.IsLoadDataValid())
			{
				TransformTexture(m_Logo,
					glm::ivec2((LIVESPLIT_WIDTH / 3) * 2, NOWPLAYING_HEIGHT),
					glm::vec2(LIVESPLIT_WIDTH / 2 + ((LIVESPLIT_WIDTH / 3) / 2), LIVESPLIT_HEIGHT));

				m_NowPlayingText.pos.Set(m_Logo.pos.GetX() - (LIVESPLIT_WIDTH / 3) + NOWPLAYING_BOUNCE_AMT, LIVESPLIT_HEIGHT - (NOWPLAYING_HEIGHT * 0.5f));
				m_GameNameText.SetVisible(false);
			}
			else
			{
				m_GameNameText.SetVisible(true);
				m_GameNameText.SetAsLine();

				if(m_GameNameText.GetWidth() > (LIVESPLIT_WIDTH - (m_NowPlayingText.GetWidth() + NOWPLAYING_BOUNCE_AMT)))
				{
					m_GameNameText.SetAsScaleBox(LIVESPLIT_WIDTH - (m_NowPlayingText.GetWidth() + NOWPLAYING_BOUNCE_AMT), NOWPLAYING_HEIGHT);
					m_GameNameText.pos.Set(m_NowPlayingText.GetWidth() + NOWPLAYING_BOUNCE_AMT, LIVESPLIT_HEIGHT - (NOWPLAYING_HEIGHT * 0.5f));
				}
				else
				{
					float fUsedWidth = (m_NowPlayingText.GetWidth() + NOWPLAYING_BOUNCE_AMT) + m_GameNameText.GetWidth();
					m_GameNameText.pos.Set((LIVESPLIT_WIDTH - fUsedWidth) * 0.5f, static_cast<float>(LIVESPLIT_HEIGHT));
					m_GameNameText.pos.Offset(m_NowPlayingText.GetWidth() + NOWPLAYING_BOUNCE_AMT, -15.0f);
				}
				const int iPadding = 10;
				m_NowPlayingText.pos.Set(m_GameNameText.pos.GetX() - (LIVESPLIT_WIDTH / 3) + NOWPLAYING_BOUNCE_AMT - iPadding, LIVESPLIT_HEIGHT - (NOWPLAYING_HEIGHT * 0.5f));
			}

			for(int i = 0; i < m_BoxArtList.size(); ++i)
			{
				m_BoxArtList[i]->SetVisible(i == 0);
				TransformTexture(*m_BoxArtList[i],
					glm::ivec2(LIVESPLIT_WIDTH, (LIVESPLIT_HEIGHT / 2) - (NOWPLAYING_HEIGHT / 2)),
					glm::vec2(LIVESPLIT_WIDTH / 2, (LIVESPLIT_HEIGHT / 2) + ((LIVESPLIT_HEIGHT / 2) * 0.5f)));
			}

			for(int i = 0; i < m_TitleArtList.size(); ++i)
			{
				m_TitleArtList[i]->SetVisible(false);
				TransformTexture(*m_TitleArtList[i],
					glm::ivec2(LIVESPLIT_WIDTH, (LIVESPLIT_HEIGHT / 2) - (NOWPLAYING_HEIGHT / 2)),
					glm::vec2(LIVESPLIT_WIDTH / 2, (LIVESPLIT_HEIGHT / 2) + ((LIVESPLIT_HEIGHT / 2) * 0.5f)));
			}

			for(int iDescTextState = m_DescriptionText.GetNumStates() - 1; iDescTextState >= 0; --iDescTextState)
			{
				m_DescriptionText.SetState(iDescTextState);
				if(m_DescriptionText.GetHeight() <= 275)
					break;
			}
			HyLog("Description State: " << m_DescriptionText.GetState());

			m_InfoEnt.alpha.Tween(1.0f, 1.0f);
			m_NowPlayingEnt.alpha.Tween(1.0f, 1.0f);
			m_eReloadState = RELOADSTATE_Idle;
		}
		break;
	}
}
