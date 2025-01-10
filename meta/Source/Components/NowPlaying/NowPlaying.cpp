#include "pch.h"
#include "NowPlaying.h"
#include "CtrlPanel.h"
#include <commdlg.h>


NowPlaying::NowPlaying(HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_NowPlaying, pParent),
	m_CtrlPanel_CheckBox(HyPanelInit(32, 32, 2), HyNodePath("", "CtrlPanel")),
	m_CtrlPanel_BrowseBtn(HyPanelInit(100, 32, 2), HyNodePath("", "CtrlPanel")),
	m_Description(HyPanelInit(LIVESPLIT_WIDTH, LIVESPLIT_HEIGHT), HyNodePath("", "CtrlPanel"), this)
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
				std::vector<char> fileContents;
				HyIO::ReadTextFile(ofn.lpstrFile, fileContents);

				std::string html(fileContents.begin(), fileContents.end());
				std::regex div_regex(R"(<div class="body game_desc">([\s\S]*?)</div>)");

				std::smatch matches;
				if(std::regex_search(html, matches, div_regex))
				{
					// The first capture group contains the text inside the div
					std::string sDescription = HyIO::HtmlDecode(matches[1].str());
					sDescription = std::regex_replace(sDescription, std::regex("[\t\r]"), "");

					m_Description.SetText(sDescription);
				}
				else
					m_Description.SetText("");

				std::regex pageTitle_regex(R"(<h1 class="page-title">(.*)</h1>)");
				if(std::regex_search(html, matches, pageTitle_regex))
				{
					// The first capture group contains the text inside the div
					std::string sTitle = HyIO::HtmlDecode(matches[1].str());
					m_Title.SetText(sTitle);
				}
				else
					m_Title.SetText("");
				
				// Get the 2nd parent directory of the file to get to the root directory for this platform
				std::string sFilePath(ofn.lpstrFile);
				std::string sMediaDirectory = HyIO::GetDirectoryFromPath(sFilePath); // First strip the file name
				sMediaDirectory = HyIO::GetDirectoryFromPath(sMediaDirectory); // Then go back one directory
				sMediaDirectory += "media/Named_Boxarts";
				
				std::vector<std::string> tempDirFileList;
				tempDirFileList = HyIO::GetFileList(sMediaDirectory, ".png", false);

				std::vector<std::string> sMatchingFileList;
				for(auto &sFile : tempDirFileList)
				{
					std::string sBaseFileName = HyIO::GetFileNameFromPath(sFile);
					// Get the portion of the sBaseFileName before the first open parenthesis
					sBaseFileName = sBaseFileName.substr(0, sBaseFileName.find('('));
					// Remove any trailing whitespace
					sBaseFileName = std::regex_replace(sBaseFileName, std::regex("\\s+$"), "");

					// Do case insensitive compare and any ignore whitespace
					if(std::equal(sBaseFileName.begin(), sBaseFileName.end(), m_Title.GetUtf8String().begin(), m_Title.GetUtf8String().end(),
						[](char a, char b)
						{
							return tolower(a) == tolower(b);
						}))
					{
						sMatchingFileList.push_back(sFile);
					}
				}

				// Choose the best matching file - ideally the first one with (USA) in the name
				std::string sBestMatchingFile;
				for(auto &sFile : sMatchingFileList)
				{
					if(sBestMatchingFile.empty())
						sBestMatchingFile = sFile;
					else if(sFile.find("(USA)") != std::string::npos)
					{
						sBestMatchingFile = sFile;
						break;
					}
				}

				m_BoxArt.Init(sBestMatchingFile, HyTextureInfo(), this);
				m_BoxArt.Load();
			}
		});

	m_Description.SetAsColumn();
}

/*virtual*/ NowPlaying::~NowPlaying()
{
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
}
