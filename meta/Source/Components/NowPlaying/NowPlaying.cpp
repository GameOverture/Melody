#include "pch.h"
#include "NowPlaying.h"
#include "CtrlPanel.h"
#include "Melody.h"

#include <fstream>

#define NOWPLAYING_BOUNCE_AMT 25
#define NOWPLAYING_HEIGHT 100
#define NOWPLAYING_SLIDESHOW_DUR 30.0
#define NOWPLAYING_DESC_WIDTH (LIVESPLIT_WIDTH * 0.66f)
#define NOWPLAYING_DESC_HEIGHT ((LIVESPLIT_HEIGHT/2) - NOWPLAYING_TIMER_HEIGHT)
#define NOWPLAYING_DETAILS_WIDTH (LIVESPLIT_WIDTH - NOWPLAYING_DESC_WIDTH)
#define NOWPLAYING_TIMER_HEIGHT 75


NowPlaying::NowPlaying(HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_NowPlaying, pParent),
	m_CtrlPanel_BrowseBtn(HyPanelInit(100, 32, 2), HyNodePath("", "CtrlPanel")),
	m_CtrlPanel_StopwatchCheckBox(HyPanelInit(25, 25, 2), HyNodePath("", "CtrlPanel")),
	m_CtrlPanel_StopwatchResetBtn(HyPanelInit(15, 15, 2), HyNodePath("", "CtrlPanel")),
	m_CtrlPanel_StopwatchHrBck(HyPanelInit(15, 15, 2), HyNodePath("", "CtrlPanel")),
	m_CtrlPanel_StopwatchMinBck(HyPanelInit(15, 15, 2), HyNodePath("", "CtrlPanel")),
	m_CtrlPanel_StopwatchStartPause(HyPanelInit(50, 28, 2), HyNodePath("", "CtrlPanel")),
	m_CtrlPanel_StopwatchMinFwd(HyPanelInit(15, 15, 2), HyNodePath("", "CtrlPanel")),
	m_CtrlPanel_StopwatchHrFwd(HyPanelInit(15, 15, 2), HyNodePath("", "CtrlPanel")),
	m_InfoEnt(this),
	m_iSlideShowIndex(0),
	m_DescriptionArea(&m_InfoEnt),
	m_DescriptionText("Description", &m_InfoEnt),
	m_Details(HYORIENT_Vertical, HyPanelInit(NOWPLAYING_DETAILS_WIDTH, NOWPLAYING_DESC_HEIGHT), &m_InfoEnt),
	m_ReleaseLabel(HyPanelInit(NOWPLAYING_DETAILS_WIDTH, 32), "Label"),
	m_ReleaseText(HyPanelInit(NOWPLAYING_DETAILS_WIDTH, 45), "CtrlPanel"),
	m_DevLabel(HyPanelInit(NOWPLAYING_DETAILS_WIDTH, 32), "Label"),
	m_DevText(HyPanelInit(NOWPLAYING_DETAILS_WIDTH, 45), "CtrlPanel"),
	m_PubLabel(HyPanelInit(NOWPLAYING_DETAILS_WIDTH, 32), "Label"),
	m_PubText(HyPanelInit(NOWPLAYING_DETAILS_WIDTH, 45), "CtrlPanel"),
	m_NowPlayingEnt(this),
	m_Logo(&m_NowPlayingEnt),
	m_NowPlayingText("Label", &m_NowPlayingEnt),
	m_GameNameText("MainText", &m_NowPlayingEnt),
	m_TimeEnt(this),
	m_TimeLabel(HyPanelInit(LIVESPLIT_WIDTH * 0.8f, NOWPLAYING_TIMER_HEIGHT, 1, HyColor::Black, HyColor::White), "MainText", HyMargins<float>(5,5,5,15), &m_TimeEnt),
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
			std::string sHtmlFilePath = Compositorium::Get()->OpenHtmlFileDlg();

			// Display the Open File dialog box
			if(sHtmlFilePath.empty() == false)
			{
				m_sHtmlFilePath = sHtmlFilePath;
				
				m_NowPlayingEnt.SetVisible(true);
				m_NowPlayingEnt.alpha.Set(1.0f);
				m_NowPlayingEnt.alpha.Tween(0.0f, 1.0f);

				m_InfoEnt.SetVisible(true);
				m_InfoEnt.alpha.Set(1.0f);
				m_InfoEnt.alpha.Tween(0.0f, 1.0f);

				m_eReloadState = RELOADSTATE_FadingOut;
			}
		});

	m_CtrlPanel_StopwatchCheckBox.SetText("Time");
	m_CtrlPanel_StopwatchCheckBox.SetCheckedChangedCallback(
		[this](HyButton *pButton)
		{
			if(pButton->IsChecked())
			{
				m_TimeEnt.SetVisible(true);
				m_TimeEnt.pos.Tween(0, 0, 1.25f, HyTween::QuadOut);
			}
			else
				m_TimeEnt.pos.Tween(0, -150, 1.25f, HyTween::QuadOut, 0.0f, [](IHyNode *pThis) { pThis->SetVisible(false); });
		});
	m_CtrlPanel_StopwatchResetBtn.SetText("0");
	m_CtrlPanel_StopwatchResetBtn.SetButtonClickedCallback(
		[this](HyButton *pButton)
		{
			m_Stopwatch.Reset();
		});
	m_CtrlPanel_StopwatchHrBck.SetText("<|");
	m_CtrlPanel_StopwatchHrBck.SetButtonClickedCallback(
		[this](HyButton *pButton)
		{
			m_Stopwatch.OffsetElapsedTime(-60.0 * 60.0);
		});
	m_CtrlPanel_StopwatchMinBck.SetText("<");
	m_CtrlPanel_StopwatchMinBck.SetButtonClickedCallback(
		[this](HyButton *pButton)
		{
			m_Stopwatch.OffsetElapsedTime(-60.0);
		});
	m_CtrlPanel_StopwatchStartPause.SetText("Play/Pause");
	m_CtrlPanel_StopwatchStartPause.SetButtonClickedCallback(
		[this](HyButton *pButton)
		{
			if(m_Stopwatch.IsRunning())
				m_Stopwatch.Pause();
			else
				m_Stopwatch.Start();
		});
	m_CtrlPanel_StopwatchMinFwd.SetText(">");
	m_CtrlPanel_StopwatchMinFwd.SetButtonClickedCallback(
		[this](HyButton *pButton)
		{
			m_Stopwatch.OffsetElapsedTime(60.0);
		});
	m_CtrlPanel_StopwatchHrFwd.SetText("|>");
	m_CtrlPanel_StopwatchHrFwd.SetButtonClickedCallback(
		[this](HyButton *pButton)
		{
			m_Stopwatch.OffsetElapsedTime(60.0 * 60.0);
		});

	m_NowPlayingEnt.SetVisible(false);
	m_NowPlayingEnt.SetDisplayOrder(DISPLAYORDER_AboveMonitor);
	m_NowPlayingText.SetText("Now Playing");
	m_NowPlayingText.SetAsScaleBox((LIVESPLIT_WIDTH / 3) - NOWPLAYING_BOUNCE_AMT, NOWPLAYING_HEIGHT);
	
	m_InfoEnt.SetVisible(false);
	m_DescriptionArea.SetTint(HyColor::Black);
	m_DescriptionArea.alpha.Set(0.2f);
	m_DescriptionText.SetAsColumn(LIVESPLIT_WIDTH * 0.66f);
	m_DescriptionText.pos.Set(0, LIVESPLIT_HEIGHT / 2);

	m_Details.pos.Set(NOWPLAYING_DESC_WIDTH, static_cast<float>(NOWPLAYING_TIMER_HEIGHT));
	m_Details.SetDefaultWidgetSpacing(0, true);

	m_ReleaseLabel.SetAlignment(HYALIGN_Left);
	m_ReleaseLabel.SetText("Released");
	m_ReleaseText.SetAlignment(HYALIGN_Left);
	m_ReleaseText.SetAsBox(true, true);
	m_DevLabel.SetAlignment(HYALIGN_Left);
	m_DevLabel.SetText("Developer");
	m_DevText.SetAlignment(HYALIGN_Left);
	m_DevText.SetAsBox(true, true);
	m_PubLabel.SetAlignment(HYALIGN_Left);
	m_PubLabel.SetText("Publisher");
	m_PubText.SetAlignment(HYALIGN_Left);
	m_PubText.SetAsBox(true, true);

	m_TimeEnt.SetVisible(false);
	m_TimeEnt.pos.Set(0, -150);
	m_GameTimeText.SetText("Elapsed Game Time:");
	m_GameTimeText.pos.Set(5, 40);
	m_GameTimeText.SetAsScaleBox(LIVESPLIT_WIDTH / 2, 55);
}

/*virtual*/ NowPlaying::~NowPlaying()
{
	for(int i = 0; i < m_SlideShowList.size(); ++i)
		delete m_SlideShowList[i];
}

/*virtual*/ void NowPlaying::PopulateCtrlPanel(CtrlPanel &ctrlPanel) /*override*/
{
	HyLayoutHandle hRow = ctrlPanel.InsertLayout(HYORIENT_Horizontal);
	ctrlPanel.InsertWidget(m_CtrlPanel_CheckBox, hRow);
	ctrlPanel.InsertWidget(m_CtrlPanel_BrowseBtn, hRow);
	ctrlPanel.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);
	HyLayoutHandle hTimeRow = ctrlPanel.InsertLayout(HYORIENT_Horizontal);
	ctrlPanel.InsertWidget(m_CtrlPanel_StopwatchCheckBox, hTimeRow);
	ctrlPanel.InsertWidget(m_CtrlPanel_StopwatchResetBtn, hTimeRow);
	ctrlPanel.InsertWidget(m_CtrlPanel_StopwatchHrBck, hTimeRow);
	ctrlPanel.InsertWidget(m_CtrlPanel_StopwatchMinBck, hTimeRow);
	ctrlPanel.InsertWidget(m_CtrlPanel_StopwatchStartPause, hTimeRow);
	ctrlPanel.InsertWidget(m_CtrlPanel_StopwatchMinFwd, hTimeRow);
	ctrlPanel.InsertWidget(m_CtrlPanel_StopwatchHrFwd, hTimeRow);
}

void NowPlaying::ShowGameTime(bool bShow)
{
	m_CtrlPanel_StopwatchCheckBox.SetChecked(bShow);
}

/*virtual*/ void NowPlaying::Show(float fDuration) /*override*/
{
	alpha.Set(0.0f);
	alpha.Tween(1.0f, fDuration);
	SetVisible(true);

	Melody::RefreshCamera();
}

/*virtual*/ void NowPlaying::Hide(float fDuration) /*override*/
{
	alpha.Tween(0.0f, fDuration, HyTween::Linear, 0.0f,
		[this](IHyNode *pThis)
		{
			pThis->SetVisible(false);
			Melody::RefreshCamera();
		});
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

			GameInfo gameObj = Compositorium::Get()->GetGame(Compositorium::Get()->GetConsoleFromPath(m_sHtmlFilePath), sUrlKey);

			m_GameNameText.SetText(gameObj.GetName());

			std::string sMediaPath = Compositorium::Get()->GetMediaPath(gameObj.GetConsole(), MEDIATYPE_Logos);
			std::string sBestMatchingLogoFile = Compositorium::Get()->GetBestMedia(gameObj, MEDIATYPE_Logos);
			m_Logo.Uninit();
			if(sBestMatchingLogoFile.empty() == false)
				m_Logo.Init(sMediaPath + sBestMatchingLogoFile, HyTextureInfo(), &m_NowPlayingEnt);

			for(int i = 0; i < m_SlideShowList.size(); ++i)
				delete m_SlideShowList[i];
			m_SlideShowList.clear();

			sMediaPath = Compositorium::Get()->GetMediaPath(gameObj.GetConsole(), MEDIATYPE_Boxarts);
			std::vector<std::string> boxartsFilePathList = gameObj.GetMediaList(MEDIATYPE_Boxarts);
			for(int i = 0; i < boxartsFilePathList.size(); ++i)
			{
				HyTexturedQuad2d *pNewBoxArt = HY_NEW HyTexturedQuad2d(sMediaPath + boxartsFilePathList[i], HyTextureInfo(), &m_InfoEnt);
				m_SlideShowList.push_back(pNewBoxArt);
			}
			sMediaPath = Compositorium::Get()->GetMediaPath(gameObj.GetConsole(), MEDIATYPE_Titles);
			std::vector<std::string> titleartsFilePathList = gameObj.GetMediaList(MEDIATYPE_Titles);
			for(int i = 0; i < titleartsFilePathList.size(); ++i)
			{
				HyTexturedQuad2d *pNewTitleArt = HY_NEW HyTexturedQuad2d(sMediaPath + titleartsFilePathList[i], HyTextureInfo(), &m_InfoEnt);
				m_SlideShowList.push_back(pNewTitleArt);
			}
			HyRand::Shuffle(m_SlideShowList);
			m_iSlideShowIndex = 0;

			m_DescriptionText.SetText(gameObj.GetDescription());
			m_DescriptionText.ParentDetach();
			m_InfoEnt.ChildAppend(m_DescriptionText);

			m_ReleaseText.SetText(gameObj.GetRelease());
			m_DevText.SetText(gameObj.GetDeveloper());
			m_PubText.SetText(gameObj.GetPublisher());

			m_Details.ClearItems();
			bool bFirstDetail = true;
			uint32 uiSpacingAmt = 0;
			if(m_ReleaseText.GetUtf8String().empty() == false)
			{
				if(bFirstDetail == false)
					m_Details.InsertSpacer(HYSIZEPOLICY_Fixed, uiSpacingAmt);
				bFirstDetail = false;
				
				HyLayoutHandle hRow = m_Details.InsertLayout(HYORIENT_Horizontal);
				m_Details.InsertWidget(m_ReleaseLabel, hRow);
				m_Details.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);
				m_Details.InsertWidget(m_ReleaseText);

				m_ReleaseLabel.SetVisible(true);
				m_ReleaseText.SetVisible(true);
			}
			else
			{
				m_ReleaseLabel.SetVisible(false);
				m_ReleaseText.SetVisible(false);
			}
			if(m_DevText.GetUtf8String().empty() == false)
			{
				if(bFirstDetail == false)
					m_Details.InsertSpacer(HYSIZEPOLICY_Fixed, uiSpacingAmt);
				bFirstDetail = false;

				HyLayoutHandle hRow = m_Details.InsertLayout(HYORIENT_Horizontal);
				m_Details.InsertWidget(m_DevLabel, hRow);
				m_Details.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);
				m_Details.InsertWidget(m_DevText);

				m_DevLabel.SetVisible(true);
				m_DevText.SetVisible(true);
			}
			else
			{
				m_DevLabel.SetVisible(false);
				m_DevText.SetVisible(false);
			}
			if(m_PubText.GetUtf8String().empty() == false)
			{
				if(bFirstDetail == false)
					m_Details.InsertSpacer(HYSIZEPOLICY_Fixed, uiSpacingAmt);
				bFirstDetail = false;

				HyLayoutHandle hRow = m_Details.InsertLayout(HYORIENT_Horizontal);
				m_Details.InsertWidget(m_PubLabel, hRow);
				m_Details.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);
				m_Details.InsertWidget(m_PubText);

				m_PubLabel.SetVisible(true);
				m_PubText.SetVisible(true);
			}
			else
			{
				m_PubLabel.SetVisible(false);
				m_PubText.SetVisible(false);
			}
			m_Details.InsertSpacer();

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

			for(int i = 0; i < m_SlideShowList.size(); ++i)
			{
				m_SlideShowList[i]->SetVisible(i == 0);
				TransformTexture(*m_SlideShowList[i],
					glm::ivec2(LIVESPLIT_WIDTH, (LIVESPLIT_HEIGHT / 2) - (NOWPLAYING_HEIGHT / 2)),
					glm::vec2(LIVESPLIT_WIDTH / 2, (LIVESPLIT_HEIGHT / 2) + ((LIVESPLIT_HEIGHT / 2) * 0.5f)));
			}
			if(m_SlideShowList.size() > 1)
				m_SlideShowTimer.InitStart(NOWPLAYING_SLIDESHOW_DUR);

			for(int iDescTextState = m_DescriptionText.GetNumStates() - 3; iDescTextState >= 0; --iDescTextState)
			{
				m_DescriptionText.SetState(iDescTextState);
				if(m_DescriptionText.GetHeight() <= NOWPLAYING_DESC_HEIGHT)
					break;
			}
			if(m_DescriptionText.GetHeight() > NOWPLAYING_DESC_HEIGHT)
			{
				m_DescriptionArea.SetAsBox(LIVESPLIT_WIDTH, NOWPLAYING_DESC_HEIGHT + m_DescriptionText.GetLineBreakHeight());
				m_DescriptionArea.pos.Set(0, NOWPLAYING_TIMER_HEIGHT);
				
				m_DescriptionText.SetScissor(HyRect(0.0f, -m_DescriptionArea.GetHeight() + m_DescriptionText.GetLineBreakHeight(), NOWPLAYING_DESC_WIDTH, m_DescriptionArea.GetHeight()));
			}
			else
			{
				float fDetailsHeight = m_Details.GetSize().y - m_PubText.pos.GetY();
				float fDescender = fabs(m_DescriptionText.GetLineDescender());
				if(m_DescriptionText.GetHeight() + fDescender < fDetailsHeight)
				{
					m_DescriptionArea.SetAsBox(LIVESPLIT_WIDTH, fDetailsHeight);
					m_DescriptionArea.pos.Set(0.0f, m_DescriptionText.pos.GetY() - fDetailsHeight + m_DescriptionText.GetLineBreakHeight());
				}
				else
				{
					m_DescriptionArea.SetAsBox(LIVESPLIT_WIDTH, m_DescriptionText.GetHeight() + fDescender);
					m_DescriptionArea.pos.Set(0.0f, m_DescriptionText.pos.GetY() - m_DescriptionText.GetHeight() + m_DescriptionText.GetLineBreakHeight() - fDescender);
				}

				m_DescriptionText.ClearScissor(true);
			}

			m_Details.pos.Set(NOWPLAYING_DESC_WIDTH + 5, NOWPLAYING_TIMER_HEIGHT + m_DescriptionText.GetLineBreakHeight());

			m_InfoEnt.alpha.Tween(1.0f, 1.0f);
			m_NowPlayingEnt.alpha.Tween(1.0f, 1.0f);
			m_eReloadState = RELOADSTATE_Idle;
		}
		break;
	}

	if(IsLoaded() && m_SlideShowList.size() > 1 && m_SlideShowTimer.IsExpired())
	{
		int iCurIndex = m_iSlideShowIndex;
		int iNextIndex = (m_iSlideShowIndex + 1) % m_SlideShowList.size();
		
		const float fSlideTime = 0.5f;
		m_SlideShowList[iCurIndex]->alpha.Tween(0.0f, fSlideTime, HyTween::Linear, 0.0f, [](IHyNode *pThis) { pThis->SetVisible(false); });
		m_SlideShowList[iNextIndex]->SetVisible(true);
		m_SlideShowList[iNextIndex]->alpha.Set(0.0f);
		m_SlideShowList[iNextIndex]->alpha.Tween(1.0f, fSlideTime);

		m_iSlideShowIndex = iNextIndex;
		m_SlideShowTimer.InitStart(NOWPLAYING_SLIDESHOW_DUR);
	}
}
