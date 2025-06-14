#include "pch.h"
#include "EditPage.h"
#include "GameBrowser.h"
#include "Compositorium.h"
#include "Melody.h"

#define EDITPAGE_WIDGET_SPACING 5
#define EDITPAGE_SPINE_WIDTH 100
#define EDITPAGE_PAGE_WIDTH ((GAMEBROWSER_LAYOUT_WIDTH - EDITPAGE_SPINE_WIDTH - (EDITPAGE_WIDGET_SPACING * 5)) / 2)

#define EDITPAGE_GAME_TITLE_HEIGHT 64
#define EDITPAGE_MEDIA_SIZE 100
#define EDITPAGE_BOXART_HEIGHT (GAMEBROWSER_LAYOUT_HEIGHT - EDITPAGE_GAME_TITLE_HEIGHT - EDITPAGE_MEDIA_SIZE - (EDITPAGE_WIDGET_SPACING * 4))

#define EDITPAGE_INFO_HEIGHT 32
#define EDITPAGE_CENTER_MARGIN 10
#define EDITPAGE_CHECKBOX_SIZE 20
#define EDITPAGE_CHECKBOX_SPACING 5
#define EDITPAGE_DATA_HEIGHT 50

EditPage::EditPage(HyEntity2d *pParent) :
	HyUiContainer(HYORIENT_Horizontal, HyUiPanelInit(GAMEBROWSER_WIDTH, GAMEBROWSER_HEIGHT), pParent),
	m_pBoxartRef(nullptr),
	m_GameTitle(HyUiPanelInit(EDITPAGE_PAGE_WIDTH, EDITPAGE_GAME_TITLE_HEIGHT), HyUiTextInit("MainText"), this),
	m_GameBoxart(HyUiPanelInit(EDITPAGE_PAGE_WIDTH, EDITPAGE_BOXART_HEIGHT), this),
	m_GameMedia(HyUiPanelInit(EDITPAGE_MEDIA_SIZE, EDITPAGE_MEDIA_SIZE), this),
	m_Info(),
	m_InfoDescription(HyUiPanelInit(EDITPAGE_PAGE_WIDTH - (EDITPAGE_WIDGET_SPACING * 3), (GAMEBROWSER_LAYOUT_HEIGHT / 4) + 50), HyUiTextInit("Description"), this),
	m_InfoReleasedLabel(HyUiPanelInit((EDITPAGE_PAGE_WIDTH / 2) - EDITPAGE_CENTER_MARGIN - (EDITPAGE_WIDGET_SPACING * 3), EDITPAGE_INFO_HEIGHT), HyUiTextInit("Label"), this),
	m_InfoReleasedText(HyUiPanelInit((EDITPAGE_PAGE_WIDTH / 2) - EDITPAGE_CENTER_MARGIN - (EDITPAGE_WIDGET_SPACING * 3), EDITPAGE_INFO_HEIGHT), HyUiTextInit("CtrlPanel"), this),
	m_InfoGenreLabel(HyUiPanelInit((EDITPAGE_PAGE_WIDTH / 2) - EDITPAGE_CENTER_MARGIN - (EDITPAGE_WIDGET_SPACING * 3), EDITPAGE_INFO_HEIGHT), HyUiTextInit("Label"), this),
	m_InfoGenreText(HyUiPanelInit((EDITPAGE_PAGE_WIDTH / 2) - EDITPAGE_CENTER_MARGIN - (EDITPAGE_WIDGET_SPACING * 3), EDITPAGE_INFO_HEIGHT), HyUiTextInit("CtrlPanel"), this),
	m_InfoDeveloperLabel(HyUiPanelInit((EDITPAGE_PAGE_WIDTH / 2) - EDITPAGE_CENTER_MARGIN - (EDITPAGE_WIDGET_SPACING * 3), EDITPAGE_INFO_HEIGHT), HyUiTextInit("Label"), this),
	m_InfoDeveloperText(HyUiPanelInit((EDITPAGE_PAGE_WIDTH / 2) - EDITPAGE_CENTER_MARGIN - (EDITPAGE_WIDGET_SPACING * 3), EDITPAGE_INFO_HEIGHT), HyUiTextInit("CtrlPanel"), this),
	m_InfoPublisherLabel(HyUiPanelInit((EDITPAGE_PAGE_WIDTH / 2) - EDITPAGE_CENTER_MARGIN - (EDITPAGE_WIDGET_SPACING * 3), EDITPAGE_INFO_HEIGHT), HyUiTextInit("Label"), this),
	m_InfoPublisherText(HyUiPanelInit((EDITPAGE_PAGE_WIDTH / 2) - EDITPAGE_CENTER_MARGIN - (EDITPAGE_WIDGET_SPACING * 3), EDITPAGE_INFO_HEIGHT), HyUiTextInit("CtrlPanel"), this),
	m_DataBlindCheckBox(HyUiPanelInit(EDITPAGE_CHECKBOX_SIZE, EDITPAGE_CHECKBOX_SIZE, 1, HyColor::Black), HyUiTextInit("Description"), this),
	m_DataOwnedCheckBox(HyUiPanelInit(EDITPAGE_CHECKBOX_SIZE, EDITPAGE_CHECKBOX_SIZE, 1, HyColor::Black), HyUiTextInit("Description"), this),
	m_DataWishlistCheckBox(HyUiPanelInit(EDITPAGE_CHECKBOX_SIZE, EDITPAGE_CHECKBOX_SIZE, 1, HyColor::Black), HyUiTextInit("Description"), this),
	m_DataPlayedCheckBox(HyUiPanelInit(EDITPAGE_CHECKBOX_SIZE, EDITPAGE_CHECKBOX_SIZE, 1, HyColor::Black), HyUiTextInit("Description"), this),
	m_DataInterestedCheckBox(HyUiPanelInit(EDITPAGE_CHECKBOX_SIZE, EDITPAGE_CHECKBOX_SIZE, 1, HyColor::Black), HyUiTextInit("Description"), this),
	m_DataEvergreenCheckBox(HyUiPanelInit(EDITPAGE_CHECKBOX_SIZE, EDITPAGE_CHECKBOX_SIZE, 1, HyColor::Black), HyUiTextInit("Description"), this),
	m_DataBeatenCheckBox(HyUiPanelInit(EDITPAGE_CHECKBOX_SIZE, EDITPAGE_CHECKBOX_SIZE, 1, HyColor::Black), HyUiTextInit("Description"), this),
	m_Data100PercentCheckBox(HyUiPanelInit(EDITPAGE_CHECKBOX_SIZE, EDITPAGE_CHECKBOX_SIZE, 1, HyColor::Black), HyUiTextInit("Description"), this),
	m_DataSpeedrunningCheckBox(HyUiPanelInit(EDITPAGE_CHECKBOX_SIZE, EDITPAGE_CHECKBOX_SIZE, 1, HyColor::Black), HyUiTextInit("Description"), this),
	m_DataFirstPlayedOnStreamLabel(HyUiPanelInit(200, EDITPAGE_DATA_HEIGHT), HyUiTextInit("CtrlPanel"), this),
	m_DataFirstPlayedOnStreamValue(HyUiPanelInit(200, EDITPAGE_DATA_HEIGHT, 1), HyUiTextInit("MainText"), this),
	m_DataFirstPlayedOnStreamBtn(HyUiPanelInit(100, EDITPAGE_DATA_HEIGHT, 1), HyUiTextInit("CtrlPanel"), this),
	m_DataElapsedPlayTimeLabel(HyUiPanelInit(200, EDITPAGE_DATA_HEIGHT, 1), HyUiTextInit("MainText"), this),
	m_DataElapsedPlayTimeResetBtn(HyUiPanelInit(50, EDITPAGE_DATA_HEIGHT, 1), HyUiTextInit("CtrlPanel"), this),
	m_DataElapsedPlayTimeHrBckBtn(HyUiPanelInit(50, EDITPAGE_DATA_HEIGHT, 1), HyUiTextInit("CtrlPanel"), this),
	m_DataElapsedPlayTimeMinBckBtn(HyUiPanelInit(50, EDITPAGE_DATA_HEIGHT, 1), HyUiTextInit("CtrlPanel"), this),
	m_DataElapsedPlayTimeMinFwdBtn(HyUiPanelInit(50, EDITPAGE_DATA_HEIGHT, 1), HyUiTextInit("CtrlPanel"), this),
	m_DataElapsedPlayTimeHrFwdBtn(HyUiPanelInit(50, EDITPAGE_DATA_HEIGHT, 1), HyUiTextInit("CtrlPanel"), this),
	m_DataBeatenOnStreamLabel(HyUiPanelInit(200, EDITPAGE_DATA_HEIGHT), HyUiTextInit("CtrlPanel"), this),
	m_DataBeatenOnStreamValue(HyUiPanelInit(200, EDITPAGE_DATA_HEIGHT, 1), HyUiTextInit("MainText"), this),
	m_DataBeatenOnStreamBtn(HyUiPanelInit(100, EDITPAGE_DATA_HEIGHT, 1), HyUiTextInit("CtrlPanel"), this),
	m_DataNotes(HyUiPanelInit(EDITPAGE_PAGE_WIDTH - (EDITPAGE_WIDGET_SPACING * 3), 200, 1), HyUiTextInit("CtrlPanel"), this)
{
	m_RootLayout.SetMargins(GAMEBROWSER_MARGINS, EDITPAGE_WIDGET_SPACING);
	SetDefaultWidgetSpacing(EDITPAGE_WIDGET_SPACING, true);

	m_GameTitle.SetTextState(1);
	m_GameTitle.SetButtonClickedCallback(
		[this](HyButton *pThis)
		{
			if(IsInputAllowed() == false)
				return;

			static_cast<GameBrowser *>(ParentGet())->BrowseAtGame(m_Info);
		});

	m_GameTitle.SetText("<No Game>");
	m_InfoDescription.SetTextState(1);
	m_InfoDescription.SetAsColumn();
	m_InfoReleasedLabel.SetText("Released");
	m_InfoGenreLabel.SetText("Genre");
	m_InfoDeveloperLabel.SetText("Developer");
	m_InfoPublisherLabel.SetText("Publisher");
	m_DataBlindCheckBox.SetText("Blind");
	m_DataOwnedCheckBox.SetText("Owned");
	m_DataWishlistCheckBox.SetText("Wishlist");
	m_DataFirstPlayedOnStreamLabel.SetText("First Played On Stream");
	m_DataFirstPlayedOnStreamBtn.SetText("TimeStamp");
	m_DataPlayedCheckBox.SetText("Played");
	m_DataInterestedCheckBox.SetText("Interest");
	m_DataEvergreenCheckBox.SetText("Evergreen");
	m_DataBeatenOnStreamLabel.SetText("Beaten On Stream");
	m_DataBeatenOnStreamBtn.SetText("TimeStamp");
	m_DataBeatenCheckBox.SetText("Beaten");
	m_Data100PercentCheckBox.SetText("100%");
	m_DataSpeedrunningCheckBox.SetText("Speedrunning");
	m_DataElapsedPlayTimeResetBtn.SetText("R");
	m_DataElapsedPlayTimeHrBckBtn.SetText("<|");
	m_DataElapsedPlayTimeMinBckBtn.SetText("<");
	m_DataElapsedPlayTimeMinFwdBtn.SetText(">");
	m_DataElapsedPlayTimeHrFwdBtn.SetText("|>");

	HyLayoutHandle hPage1 = InsertLayout(HYORIENT_Vertical);
	InsertSpacer(HYSIZEPOLICY_Fixed, EDITPAGE_SPINE_WIDTH);
	HyLayoutHandle hPage2 = InsertLayout(HYORIENT_Vertical);

	// PAGE 1
	InsertWidget(m_GameTitle, hPage1);
	InsertWidget(m_GameBoxart, hPage1);
	InsertWidget(m_GameMedia, hPage1);

	// PAGE 2
	InsertWidget(m_InfoDescription, hPage2);
	//
	HyLayoutHandle hInfoMain = InsertLayout(HYORIENT_Horizontal, hPage2);
	HyLayoutHandle hInfoColumn1 = InsertLayout(HYORIENT_Vertical, hInfoMain);
	//InsertSpacer(HYSIZEPOLICY_Fixed, EDITPAGE_CENTER_MARGIN, hInfoColumn1);
	HyLayoutHandle hInfoColumn2 = InsertLayout(HYORIENT_Vertical, hInfoMain);
	InsertWidget(m_InfoReleasedLabel, hInfoColumn1);
	InsertWidget(m_InfoReleasedText, hInfoColumn1);
	InsertWidget(m_InfoGenreLabel, hInfoColumn2);
	InsertWidget(m_InfoGenreText, hInfoColumn2);
	InsertWidget(m_InfoDeveloperLabel, hInfoColumn1);
	InsertWidget(m_InfoDeveloperText, hInfoColumn1);
	InsertWidget(m_InfoPublisherLabel, hInfoColumn2);
	InsertWidget(m_InfoPublisherText, hInfoColumn2);
	//
	HyLayoutHandle hDataMain = InsertLayout(HYORIENT_Vertical, hPage2);
	HyLayoutHandle hCheckBoxRow1 = InsertLayout(HYORIENT_Horizontal, hDataMain);
	InsertWidget(m_DataBlindCheckBox, hCheckBoxRow1);
	InsertSpacer(HYSIZEPOLICY_Fixed, EDITPAGE_CHECKBOX_SPACING, hCheckBoxRow1);
	InsertWidget(m_DataOwnedCheckBox, hCheckBoxRow1);
	InsertSpacer(HYSIZEPOLICY_Fixed, EDITPAGE_CHECKBOX_SPACING, hCheckBoxRow1);
	InsertWidget(m_DataWishlistCheckBox, hCheckBoxRow1);
	//InsertSpacer(HYSIZEPOLICY_Expanding, 0, hCheckBoxRow1);

	//HyLayoutHandle hCheckBoxRow2 = InsertLayout(HYORIENT_Horizontal, hDataMain);
	InsertWidget(m_DataPlayedCheckBox, hCheckBoxRow1);
	InsertSpacer(HYSIZEPOLICY_Fixed, EDITPAGE_CHECKBOX_SPACING, hCheckBoxRow1);
	InsertWidget(m_DataInterestedCheckBox, hCheckBoxRow1);
	InsertSpacer(HYSIZEPOLICY_Fixed, EDITPAGE_CHECKBOX_SPACING, hCheckBoxRow1);
	InsertWidget(m_DataEvergreenCheckBox, hCheckBoxRow1);
	//InsertSpacer(HYSIZEPOLICY_Expanding, 0, hCheckBoxRow2);

	HyLayoutHandle hFirstPlayedRow = InsertLayout(HYORIENT_Horizontal, hDataMain);
	InsertWidget(m_DataFirstPlayedOnStreamLabel, hFirstPlayedRow);
	InsertWidget(m_DataFirstPlayedOnStreamValue, hFirstPlayedRow);
	InsertWidget(m_DataFirstPlayedOnStreamBtn, hFirstPlayedRow);

	HyLayoutHandle hCheckBoxRow3 = InsertLayout(HYORIENT_Horizontal, hDataMain);
	InsertWidget(m_DataBeatenCheckBox, hCheckBoxRow3);
	InsertSpacer(HYSIZEPOLICY_Fixed, EDITPAGE_CHECKBOX_SPACING, hCheckBoxRow3);
	InsertWidget(m_Data100PercentCheckBox, hCheckBoxRow3);
	InsertSpacer(HYSIZEPOLICY_Fixed, EDITPAGE_CHECKBOX_SPACING, hCheckBoxRow3);
	InsertWidget(m_DataSpeedrunningCheckBox, hCheckBoxRow3);
	//InsertSpacer(HYSIZEPOLICY_Expanding, 0, hCheckBoxRow3);

	HyLayoutHandle hBeatenRow = InsertLayout(HYORIENT_Horizontal, hDataMain);
	InsertWidget(m_DataBeatenOnStreamLabel, hBeatenRow);
	InsertWidget(m_DataBeatenOnStreamValue, hBeatenRow);
	InsertWidget(m_DataBeatenOnStreamBtn, hBeatenRow);
	
	HyLayoutHandle hElapsedPlayedRow = InsertLayout(HYORIENT_Horizontal, hDataMain);
	InsertWidget(m_DataElapsedPlayTimeResetBtn, hElapsedPlayedRow);
	InsertWidget(m_DataElapsedPlayTimeHrBckBtn, hElapsedPlayedRow);
	InsertWidget(m_DataElapsedPlayTimeMinBckBtn, hElapsedPlayedRow);
	InsertWidget(m_DataElapsedPlayTimeLabel, hElapsedPlayedRow);
	InsertWidget(m_DataElapsedPlayTimeMinFwdBtn, hElapsedPlayedRow);
	InsertWidget(m_DataElapsedPlayTimeHrFwdBtn, hElapsedPlayedRow);

	InsertWidget(m_DataNotes, hDataMain);
	InsertSpacer(HYSIZEPOLICY_Expanding, 0);
}

/*virtual*/ EditPage::~EditPage()
{
}

void EditPage::SetGame(HyTexturedQuad2d &boxartRef, glm::vec2 ptBoxartPos, GameStats &gameStats)
{
	m_Info = Compositorium::Get()->GetGame(gameStats.GetGameId());

	m_pBoxartRef = &boxartRef;
	ChildAppend(*m_pBoxartRef);
	m_pBoxartRef->pos.Set(ptBoxartPos);

	glm::vec2 vScaleDest(EDITPAGE_PAGE_WIDTH, EDITPAGE_BOXART_HEIGHT);
	m_pBoxartRef->scale.SetAll(1.0f);
	float fScale = std::min(vScaleDest.x / m_pBoxartRef->GetWidth(), vScaleDest.y / m_pBoxartRef->GetHeight());
	m_pBoxartRef->scale.Tween(fScale, fScale, 1.0f); //BezierQuick(vScaleDest, true, 0.64f, 1.0f, HyTween::QuadIn);

	glm::vec2 ptDest = GetWidgetPos(m_GameBoxart);
	ptDest += glm::vec2(EDITPAGE_PAGE_WIDTH * 0.5f, EDITPAGE_BOXART_HEIGHT * 0.5f);
	ptDest -= glm::vec2(m_pBoxartRef->GetWidth() * 0.5f, m_pBoxartRef->GetHeight() * 0.5f);
	m_pBoxartRef->pos.BezierQuick(ptDest, true, 0.64f, 1.0f, HyTween::QuadIn);

	m_pBoxartRef->rot.Tween(8.0f * (HyRand::Boolean() ? -1.0f : 1.0f), 0.5f, HyTween::QuadInOut, 0.0f,
		[](IHyNode *pThis)
		{
			static_cast<HyTexturedQuad2d *>(pThis)->rot.Tween(0.0f, 0.5f, HyTween::QuadInOut);
		});

	m_GameTitle.SetText(m_Info.GetName());

	m_InfoDescription.SetText(m_Info.GetDescription());
	for(int iDescTextState = m_InfoDescription.GetNumTextStates() - 1; iDescTextState >= 0; --iDescTextState)
	{
		m_InfoDescription.SetTextState(iDescTextState);
		if(m_InfoDescription.GetTextHeight() <= m_InfoDescription.GetHeight())
			break;
	}


	m_InfoReleasedText.SetText(m_Info.GetRelease());
	m_InfoGenreText.SetText(m_Info.GetGenre());
	m_InfoDeveloperText.SetText(m_Info.GetDeveloper());
	m_InfoPublisherText.SetText(m_Info.GetPublisher());

	m_DataBlindCheckBox.SetChecked(gameStats.IsStatusFlagSet(STATUS_Blind));
	m_DataOwnedCheckBox.SetChecked(gameStats.IsStatusFlagSet(STATUS_Owned));
	m_DataWishlistCheckBox.SetChecked(gameStats.IsStatusFlagSet(STATUS_Wishlist));
	m_DataPlayedCheckBox.SetChecked(gameStats.IsStatusFlagSet(STATUS_Played));
	m_DataInterestedCheckBox.SetChecked(gameStats.IsStatusFlagSet(STATUS_Interested));
	m_DataEvergreenCheckBox.SetChecked(gameStats.IsStatusFlagSet(STATUS_Evergreen));
	m_DataBeatenCheckBox.SetChecked(gameStats.IsStatusFlagSet(STATUS_Beaten));
	m_Data100PercentCheckBox.SetChecked(gameStats.IsStatusFlagSet(STATUS_100Percent));
	m_DataSpeedrunningCheckBox.SetChecked(gameStats.IsStatusFlagSet(STATUS_Speedrunning));
	m_DataFirstPlayedOnStreamValue.SetText(gameStats.GetFirstPlayedOnStream());
	m_DataElapsedPlayTimeLabel.SetText(std::to_string(gameStats.GetElapsedPlayTime()));
	m_DataBeatenOnStreamValue.SetText(gameStats.GetBeatenOnStream());
	m_DataNotes.SetText(gameStats.GetNotes());
}

HyTexturedQuad2d *EditPage::GetBoxart() const
{
	return m_pBoxartRef;
}
