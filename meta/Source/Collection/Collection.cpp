#include "pch.h"
#include "Collection.h"
#include "Compositorium.h"

Collection *Collection::sm_pInstance = nullptr;

#define CHECKBOX_SIZE 32

EditPage::EditPage(HyEntity2d *pParent) :
	HyUiContainer(HYORIENT_Vertical, HyPanelInit(1870, 1030, 0, HyColor(0.0f, 0.0f, 0.0f, 0.5f)), pParent),
	m_GameTitleLabel(HyPanelInit(1870, 100), "MainText", this),
	m_BackBtn(HyPanelInit(200, 200, 3), "MainText", this),
	m_BoxartLabel(HyPanelInit(1400, 500), this),
	m_ForwardBtn(HyPanelInit(200, 200, 3), "MainText", this),
	m_GameBlindCheckBox(HyPanelInit(CHECKBOX_SIZE, CHECKBOX_SIZE, 1), "CtrlPanel", this),
	m_GameOwnedCheckBox(HyPanelInit(CHECKBOX_SIZE, CHECKBOX_SIZE, 1), "CtrlPanel", this),
	m_GameWishlistCheckBox(HyPanelInit(CHECKBOX_SIZE, CHECKBOX_SIZE, 1), "CtrlPanel", this),
	m_GamePlayedCheckBox(HyPanelInit(CHECKBOX_SIZE, CHECKBOX_SIZE, 1), "CtrlPanel", this),
	m_GameInterestedCheckBox(HyPanelInit(CHECKBOX_SIZE, CHECKBOX_SIZE, 1), "CtrlPanel", this),
	m_GameEvergreenCheckBox(HyPanelInit(CHECKBOX_SIZE, CHECKBOX_SIZE, 1), "CtrlPanel", this),
	m_GameBeatenCheckBox(HyPanelInit(CHECKBOX_SIZE, CHECKBOX_SIZE, 1), "CtrlPanel", this),
	m_Game100PercentCheckBox(HyPanelInit(CHECKBOX_SIZE, CHECKBOX_SIZE, 1), "CtrlPanel", this),
	m_GameSpeedrunningCheckBox(HyPanelInit(CHECKBOX_SIZE, CHECKBOX_SIZE, 1), "CtrlPanel", this),
	m_GameFirstPlayedOnStreamLabel(HyPanelInit(200, 50, 1), "CtrlPanel", this),
	m_GameFirstPlayedOnStreamValue(HyPanelInit(200, 50, 1), "MainText", this),
	m_GameFirstPlayedOnStreamBtn(HyPanelInit(100, 50, 1), "CtrlPanel", this),
	m_GameElapsedPlayTimeLabel(HyPanelInit(200, 50, 1), "MainText", this),
	m_GameElapsedPlayTimeResetBtn(HyPanelInit(50, 50, 1), "CtrlPanel", this),
	m_GameElapsedPlayTimeHrBckBtn(HyPanelInit(50, 50, 1), "CtrlPanel", this),
	m_GameElapsedPlayTimeMinBckBtn(HyPanelInit(50, 50, 1), "CtrlPanel", this),
	m_GameElapsedPlayTimeMinFwdBtn(HyPanelInit(50, 50, 1), "CtrlPanel", this),
	m_GameElapsedPlayTimeHrFwdBtn(HyPanelInit(50, 50, 1), "CtrlPanel", this),
	m_GameBeatenOnStreamLabel(HyPanelInit(200, 50, 1), "CtrlPanel", this),
	m_GameBeatenOnStreamValue(HyPanelInit(200, 50, 1), "MainText", this),
	m_GameBeatenOnStreamBtn(HyPanelInit(100, 50, 1), "CtrlPanel", this),
	m_NotesLineEdit(HyPanelInit(800, 200, 1), "CtrlPanel", this)
{
	m_GameTitleLabel.SetText("<No Game>");
	InsertWidget(m_GameTitleLabel);

	HyLayoutHandle hMainRow = InsertLayout(HYORIENT_Horizontal);
	m_BackBtn.SetText("<");
	InsertWidget(m_BackBtn, hMainRow);
	InsertWidget(m_BoxartLabel, hMainRow);
	m_ForwardBtn.SetText(">");
	InsertWidget(m_ForwardBtn, hMainRow);

	HyLayoutHandle hSubRow = InsertLayout(HYORIENT_Horizontal);

	const int iCheckBoxSpacing = 32;
	HyLayoutHandle hColumn = InsertLayout(HYORIENT_Vertical, hSubRow);
	HyLayoutHandle hStatusRow1 = InsertLayout(HYORIENT_Horizontal, hColumn);
	m_GameBlindCheckBox.SetText("Blind");
	InsertWidget(m_GameBlindCheckBox, hStatusRow1);
	InsertSpacer(HYSIZEPOLICY_Fixed, iCheckBoxSpacing, hStatusRow1);
	m_GameOwnedCheckBox.SetText("Owned");
	InsertWidget(m_GameOwnedCheckBox, hStatusRow1);
	InsertSpacer(HYSIZEPOLICY_Fixed, iCheckBoxSpacing, hStatusRow1);
	m_GameWishlistCheckBox.SetText("Wishlist");
	InsertWidget(m_GameWishlistCheckBox, hStatusRow1);
	InsertSpacer(HYSIZEPOLICY_Expanding, 0, hStatusRow1);
	m_GameFirstPlayedOnStreamLabel.SetText("First Played On Stream");
	InsertWidget(m_GameFirstPlayedOnStreamLabel, hStatusRow1);
	InsertWidget(m_GameFirstPlayedOnStreamValue, hStatusRow1);
	m_GameFirstPlayedOnStreamBtn.SetText("TimeStamp");
	InsertWidget(m_GameFirstPlayedOnStreamBtn, hStatusRow1);

	HyLayoutHandle hStatusRow2 = InsertLayout(HYORIENT_Horizontal, hColumn);
	m_GamePlayedCheckBox.SetText("Played");
	InsertWidget(m_GamePlayedCheckBox, hStatusRow2);
	InsertSpacer(HYSIZEPOLICY_Fixed, iCheckBoxSpacing, hStatusRow2);
	m_GameInterestedCheckBox.SetText("Interest");
	InsertWidget(m_GameInterestedCheckBox, hStatusRow2);
	InsertSpacer(HYSIZEPOLICY_Fixed, iCheckBoxSpacing, hStatusRow2);
	m_GameEvergreenCheckBox.SetText("Evergreen");
	InsertWidget(m_GameEvergreenCheckBox, hStatusRow2);
	InsertSpacer(HYSIZEPOLICY_Expanding, 0, hStatusRow2);
	m_GameBeatenOnStreamLabel.SetText("Beaten On Stream");
	InsertWidget(m_GameBeatenOnStreamLabel, hStatusRow2);
	InsertWidget(m_GameBeatenOnStreamValue, hStatusRow2);
	m_GameBeatenOnStreamBtn.SetText("TimeStamp");
	InsertWidget(m_GameBeatenOnStreamBtn, hStatusRow2);

	HyLayoutHandle hStatusRow3 = InsertLayout(HYORIENT_Horizontal, hColumn);
	m_GameBeatenCheckBox.SetText("Beaten");
	InsertWidget(m_GameBeatenCheckBox, hStatusRow3);
	InsertSpacer(HYSIZEPOLICY_Fixed, iCheckBoxSpacing, hStatusRow3);
	m_Game100PercentCheckBox.SetText("100%");
	InsertWidget(m_Game100PercentCheckBox, hStatusRow3);
	InsertSpacer(HYSIZEPOLICY_Fixed, iCheckBoxSpacing, hStatusRow3);
	m_GameSpeedrunningCheckBox.SetText("Speedrunning");
	InsertWidget(m_GameSpeedrunningCheckBox, hStatusRow3);
	InsertSpacer(HYSIZEPOLICY_Expanding, 0, hStatusRow3);
	m_GameElapsedPlayTimeResetBtn.SetText("R");
	InsertWidget(m_GameElapsedPlayTimeResetBtn, hStatusRow3);
	m_GameElapsedPlayTimeHrBckBtn.SetText("<|");
	InsertWidget(m_GameElapsedPlayTimeHrBckBtn, hStatusRow3);
	m_GameElapsedPlayTimeMinBckBtn.SetText("<");
	InsertWidget(m_GameElapsedPlayTimeMinBckBtn, hStatusRow3);
	InsertWidget(m_GameElapsedPlayTimeLabel, hStatusRow3);
	m_GameElapsedPlayTimeMinFwdBtn.SetText(">");
	InsertWidget(m_GameElapsedPlayTimeMinFwdBtn, hStatusRow3);
	m_GameElapsedPlayTimeHrFwdBtn.SetText("|>");
	InsertWidget(m_GameElapsedPlayTimeHrFwdBtn, hStatusRow3);
	
	InsertWidget(m_NotesLineEdit, hSubRow);
	InsertSpacer(HYSIZEPOLICY_Expanding, 0);
}

/*virtual*/ EditPage::~EditPage()
{
}

void EditPage::SetGame(GameStats &gameStats)
{
	GameInfo gameInfo = Compositorium::Get()->GetGame(gameStats.GetConsole(), gameStats.GetGameId());

	m_GameTitleLabel.SetText(Compositorium::Get()->GetGameName(gameInfo));
	m_GameBlindCheckBox.SetChecked(gameStats.IsStatusFlagSet(STATUS_Blind));
	m_GameOwnedCheckBox.SetChecked(gameStats.IsStatusFlagSet(STATUS_Owned));
	m_GameWishlistCheckBox.SetChecked(gameStats.IsStatusFlagSet(STATUS_Wishlist));
	m_GamePlayedCheckBox.SetChecked(gameStats.IsStatusFlagSet(STATUS_Played));
	m_GameInterestedCheckBox.SetChecked(gameStats.IsStatusFlagSet(STATUS_Interested));
	m_GameEvergreenCheckBox.SetChecked(gameStats.IsStatusFlagSet(STATUS_Evergreen));
	m_GameBeatenCheckBox.SetChecked(gameStats.IsStatusFlagSet(STATUS_Beaten));
	m_Game100PercentCheckBox.SetChecked(gameStats.IsStatusFlagSet(STATUS_100Percent));
	m_GameSpeedrunningCheckBox.SetChecked(gameStats.IsStatusFlagSet(STATUS_Speedrunning));
	m_GameFirstPlayedOnStreamValue.SetText(gameStats.GetFirstPlayedOnStream());
	m_GameElapsedPlayTimeLabel.SetText(std::to_string(gameStats.GetElapsedPlayTime()));
	m_GameBeatenOnStreamValue.SetText(gameStats.GetBeatenOnStream());
	m_NotesLineEdit.SetText(gameStats.GetNotes());
}

Collection::Collection(HyEntity2d *pParent /*= nullptr*/) :
	HyEntity2d(pParent),
	m_EditPage(this)
{
	HyAssert(sm_pInstance == nullptr, "Compositorium::Compositorium() - Only one instance of Compositorium allowed");
	sm_pInstance = this;
}

/*virtual*/ Collection::~Collection()
{
	sm_pInstance = nullptr;
}

void Collection::ShowEditPage(GameStats &gameStats)
{
	m_EditPage.SetGame(gameStats);
}
