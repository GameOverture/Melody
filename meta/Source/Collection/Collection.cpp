#include "pch.h"
#include "Collection.h"
#include "Compositorium.h"

EditPage::EditPage(HyEntity2d *pParent) :
	HyUiContainer(HYORIENT_Vertical, HyPanelInit(1870, 1030, 0, HyColor(0.0f, 0.0f, 0.0f, 0.5f)), pParent),
	m_GameTitleLabel(HyPanelInit(1870, 100), "MainText", this),
	m_BackBtn(HyPanelInit(200, 200), "MainText", this),
	m_BoxartLabel(HyPanelInit(1400, 500), this),
	m_ForwardBtn(HyPanelInit(200, 200), "MainText", this),
	m_GameBlindCheckBox(HyPanelInit(100, 100), "MainText", this),
	m_GameOwnedCheckBox(HyPanelInit(100, 100), "MainText", this),
	m_GameWishlistCheckBox(HyPanelInit(100, 100), "MainText", this),
	m_GamePlayedCheckBox(HyPanelInit(100, 100), "MainText", this),
	m_GameInterestedCheckBox(HyPanelInit(100, 100), "MainText", this),
	m_GameEvergreenCheckBox(HyPanelInit(100, 100), "MainText", this),
	m_GameBeatenCheckBox(HyPanelInit(100, 100), "MainText", this),
	m_Game100PercentCheckBox(HyPanelInit(100, 100), "MainText", this),
	m_GameSpeedrunningCheckBox(HyPanelInit(100, 100), "MainText", this),
	m_GameFirstPlayedOnStreamLabel(HyPanelInit(500, 100), "MainText", this),
	m_GameFirstPlayedOnStreamBtn(HyPanelInit(200, 100), "MainText", this),
	m_GameElapsedPlayTimeLabel(HyPanelInit(500, 100), "MainText", this),
	m_GameElapsedPlayTimeResetBtn(HyPanelInit(50, 50), "MainText", this),
	m_GameElapsedPlayTimeHrBckBtn(HyPanelInit(50, 50), "MainText", this),
	m_GameElapsedPlayTimeMinBckBtn(HyPanelInit(50, 50), "MainText", this),
	m_GameElapsedPlayTimeMinFwdBtn(HyPanelInit(50, 50), "MainText", this),
	m_GameElapsedPlayTimeHrFwdBtn(HyPanelInit(50, 50), "MainText", this),
	m_GameBeatenOnStreamLabel(HyPanelInit(500, 100), "MainText", this),
	m_GameBeatenOnStreamBtn(HyPanelInit(200, 100), "MainText", this),
	m_NotesLineEdit(HyPanelInit(1000, 200), "MainText", this)
{
	InsertWidget(m_GameTitleLabel);
	HyLayoutHandle hRow = InsertLayout(HYORIENT_Horizontal);
	InsertWidget(m_BackBtn, hRow);
	InsertWidget(m_BoxartLabel, hRow);
	InsertWidget(m_ForwardBtn, hRow);
	HyLayoutHandle hStatusRow1 = InsertLayout(HYORIENT_Horizontal);
	InsertWidget(m_GameBlindCheckBox, hStatusRow1);
	InsertWidget(m_GameOwnedCheckBox, hStatusRow1);
	InsertWidget(m_GameWishlistCheckBox, hStatusRow1);
	HyLayoutHandle hStatusRow2 = InsertLayout(HYORIENT_Horizontal);
	InsertWidget(m_GamePlayedCheckBox, hStatusRow2);
	InsertWidget(m_GameInterestedCheckBox, hStatusRow2);
	InsertWidget(m_GameEvergreenCheckBox, hStatusRow2);
	HyLayoutHandle hStatusRow3 = InsertLayout(HYORIENT_Horizontal);
	InsertWidget(m_GameBeatenCheckBox, hStatusRow3);
	InsertWidget(m_Game100PercentCheckBox, hStatusRow3);
	InsertWidget(m_GameSpeedrunningCheckBox, hStatusRow3);
	HyLayoutHandle hDatesRow = InsertLayout(HYORIENT_Horizontal);
	InsertWidget(m_GameFirstPlayedOnStreamLabel, hDatesRow);
	InsertWidget(m_GameFirstPlayedOnStreamBtn, hDatesRow);
	InsertSpacer(HYSIZEPOLICY_Expanding, 0, hDatesRow);
	InsertWidget(m_GameBeatenOnStreamLabel, hDatesRow);
	InsertWidget(m_GameBeatenOnStreamBtn, hDatesRow);
	HyLayoutHandle hElapsedPlayTimeRow = InsertLayout(HYORIENT_Horizontal);
	InsertWidget(m_GameElapsedPlayTimeResetBtn, hElapsedPlayTimeRow);
	InsertWidget(m_GameElapsedPlayTimeHrBckBtn, hElapsedPlayTimeRow);
	InsertWidget(m_GameElapsedPlayTimeMinBckBtn, hElapsedPlayTimeRow);
	InsertWidget(m_GameElapsedPlayTimeLabel, hElapsedPlayTimeRow);
	InsertWidget(m_GameElapsedPlayTimeMinFwdBtn, hElapsedPlayTimeRow);
	InsertWidget(m_GameElapsedPlayTimeHrFwdBtn, hElapsedPlayTimeRow);
	InsertSpacer(HYSIZEPOLICY_Expanding, 0, hElapsedPlayTimeRow);
	InsertWidget(m_NotesLineEdit);
}

/*virtual*/ EditPage::~EditPage()
{
}

void EditPage::SetGame(GameStats &gameStats)
{
	GameInfo gameInfo = Compositorium::Get()->GetGame(gameStats.m_eConsole, gameStats.m_sGameId);

	m_GameTitleLabel.SetText(Compositorium::Get()->GetGameName(gameInfo));
	m_GameBlindCheckBox.SetChecked(gameStats.m_uiStatusFlags & STATUS_Blind);
	m_GameOwnedCheckBox.SetChecked(gameStats.m_uiStatusFlags & STATUS_Owned);
	m_GameWishlistCheckBox.SetChecked(gameStats.m_uiStatusFlags & STATUS_Wishlist);
	m_GamePlayedCheckBox.SetChecked(gameStats.m_uiStatusFlags & STATUS_Played);
	m_GameInterestedCheckBox.SetChecked(gameStats.m_uiStatusFlags & STATUS_Interested);
	m_GameEvergreenCheckBox.SetChecked(gameStats.m_uiStatusFlags & STATUS_Evergreen);
	m_GameBeatenCheckBox.SetChecked(gameStats.m_uiStatusFlags & STATUS_Beaten);
	m_Game100PercentCheckBox.SetChecked(gameStats.m_uiStatusFlags & STATUS_100Percent);
	m_GameSpeedrunningCheckBox.SetChecked(gameStats.m_uiStatusFlags & STATUS_Speedrunning);
	m_GameFirstPlayedOnStreamLabel.SetText(gameStats.m_sDateTime_FirstPlayedOnStream);
	m_GameElapsedPlayTimeLabel.SetText(std::to_string(gameStats.m_dElapsedPlayTime));
	m_GameBeatenOnStreamLabel.SetText(gameStats.m_sDateTime_BeatenOnStream);
	m_NotesLineEdit.SetText(gameStats.m_sNotes);
}

Collection::Collection(HyEntity2d *pParent) :
	HyEntity2d(pParent),
	m_EditPage(this)
{
}

/*virtual*/ Collection::~Collection()
{
}


