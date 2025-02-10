#include "pch.h"
#include "EditPage.h"
#include "GameBrowser.h"
#include "Compositorium.h"
#include "Melody.h"

#define CHECKBOX_SIZE 20

EditPage::EditPage(HyEntity2d *pParent) :
	HyUiContainer(HYORIENT_Vertical, HyPanelInit(GAMEBROWSER_WIDTH, GAMEBROWSER_HEIGHT), pParent),
	m_pBoxartRef(nullptr),
	m_GameTitleLabel(HyPanelInit(GAMEBROWSER_WIDTH, 64), "MainText", this),
	m_BackBtn(HyPanelInit(200, 200, 3), "MainText", this),
	m_BoxartLabel(HyPanelInit(1400, 500), this),
	m_ForwardBtn(HyPanelInit(200, 200, 3), "MainText", this),
	m_GameBlindCheckBox(HyPanelInit(CHECKBOX_SIZE, CHECKBOX_SIZE, 1, HyColor::Black), "Description", this),
	m_GameOwnedCheckBox(HyPanelInit(CHECKBOX_SIZE, CHECKBOX_SIZE, 1, HyColor::Black), "Description", this),
	m_GameWishlistCheckBox(HyPanelInit(CHECKBOX_SIZE, CHECKBOX_SIZE, 1, HyColor::Black), "Description", this),
	m_GamePlayedCheckBox(HyPanelInit(CHECKBOX_SIZE, CHECKBOX_SIZE, 1, HyColor::Black), "Description", this),
	m_GameInterestedCheckBox(HyPanelInit(CHECKBOX_SIZE, CHECKBOX_SIZE, 1, HyColor::Black), "Description", this),
	m_GameEvergreenCheckBox(HyPanelInit(CHECKBOX_SIZE, CHECKBOX_SIZE, 1, HyColor::Black), "Description", this),
	m_GameBeatenCheckBox(HyPanelInit(CHECKBOX_SIZE, CHECKBOX_SIZE, 1, HyColor::Black), "Description", this),
	m_Game100PercentCheckBox(HyPanelInit(CHECKBOX_SIZE, CHECKBOX_SIZE, 1, HyColor::Black), "Description", this),
	m_GameSpeedrunningCheckBox(HyPanelInit(CHECKBOX_SIZE, CHECKBOX_SIZE, 1, HyColor::Black), "Description", this),
	m_GameFirstPlayedOnStreamLabel(HyPanelInit(200, 50), "CtrlPanel", this),
	m_GameFirstPlayedOnStreamValue(HyPanelInit(200, 50, 1), "MainText", this),
	m_GameFirstPlayedOnStreamBtn(HyPanelInit(100, 50, 1), "CtrlPanel", this),
	m_GameElapsedPlayTimeLabel(HyPanelInit(200, 50, 1), "MainText", this),
	m_GameElapsedPlayTimeResetBtn(HyPanelInit(50, 50, 1), "CtrlPanel", this),
	m_GameElapsedPlayTimeHrBckBtn(HyPanelInit(50, 50, 1), "CtrlPanel", this),
	m_GameElapsedPlayTimeMinBckBtn(HyPanelInit(50, 50, 1), "CtrlPanel", this),
	m_GameElapsedPlayTimeMinFwdBtn(HyPanelInit(50, 50, 1), "CtrlPanel", this),
	m_GameElapsedPlayTimeHrFwdBtn(HyPanelInit(50, 50, 1), "CtrlPanel", this),
	m_GameBeatenOnStreamLabel(HyPanelInit(200, 50), "CtrlPanel", this),
	m_GameBeatenOnStreamValue(HyPanelInit(200, 50, 1), "MainText", this),
	m_GameBeatenOnStreamBtn(HyPanelInit(100, 50, 1), "CtrlPanel", this),
	m_NotesLineEdit(HyPanelInit(800, 200, 1), "CtrlPanel", this)
{
	m_RootLayout.SetMargins(0, 0, 0, 0, 5);

	m_GameTitleLabel.SetTextState(1);
	m_GameTitleLabel.SetText("<No Game>");
	m_GameTitleLabel.SetButtonClickedCallback(
		[this](HyButton *pThis)
		{
			if(IsInputAllowed() == false)
				return;

			static_cast<GameBrowser *>(ParentGet())->BrowseAtGame(m_GameInfo);
		});
	InsertWidget(m_GameTitleLabel);

	HyLayoutHandle hMainRow = InsertLayout(HYORIENT_Horizontal);
	m_BackBtn.SetText("<");
	InsertWidget(m_BackBtn, hMainRow);
	InsertWidget(m_BoxartLabel, hMainRow);




	//m_DescriptionText.SetText(gameObj.GetDescription());
	//m_DescriptionText.ParentDetach();
	//m_InfoEnt.ChildAppend(m_DescriptionText);

	//m_ReleaseText.SetText(gameObj.GetRelease());
	//m_DevText.SetText(gameObj.GetDeveloper());
	//m_PubText.SetText(gameObj.GetPublisher());

	//m_Details.ClearItems();
	//bool bFirstDetail = true;
	//uint32 uiSpacingAmt = 0;
	//if(m_ReleaseText.GetUtf8String().empty() == false)
	//{
	//	if(bFirstDetail == false)
	//		m_Details.InsertSpacer(HYSIZEPOLICY_Fixed, uiSpacingAmt);
	//	bFirstDetail = false;

	//	HyLayoutHandle hRow = m_Details.InsertLayout(HYORIENT_Horizontal);
	//	m_Details.InsertWidget(m_ReleaseLabel, hRow);
	//	m_Details.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);
	//	m_Details.InsertWidget(m_ReleaseText);

	//	m_ReleaseLabel.SetVisible(true);
	//	m_ReleaseText.SetVisible(true);
	//}
	//else
	//{
	//	m_ReleaseLabel.SetVisible(false);
	//	m_ReleaseText.SetVisible(false);
	//}
	//if(m_DevText.GetUtf8String().empty() == false)
	//{
	//	if(bFirstDetail == false)
	//		m_Details.InsertSpacer(HYSIZEPOLICY_Fixed, uiSpacingAmt);
	//	bFirstDetail = false;

	//	HyLayoutHandle hRow = m_Details.InsertLayout(HYORIENT_Horizontal);
	//	m_Details.InsertWidget(m_DevLabel, hRow);
	//	m_Details.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);
	//	m_Details.InsertWidget(m_DevText);

	//	m_DevLabel.SetVisible(true);
	//	m_DevText.SetVisible(true);
	//}
	//else
	//{
	//	m_DevLabel.SetVisible(false);
	//	m_DevText.SetVisible(false);
	//}
	//if(m_PubText.GetUtf8String().empty() == false)
	//{
	//	if(bFirstDetail == false)
	//		m_Details.InsertSpacer(HYSIZEPOLICY_Fixed, uiSpacingAmt);
	//	bFirstDetail = false;

	//	HyLayoutHandle hRow = m_Details.InsertLayout(HYORIENT_Horizontal);
	//	m_Details.InsertWidget(m_PubLabel, hRow);
	//	m_Details.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);
	//	m_Details.InsertWidget(m_PubText);

	//	m_PubLabel.SetVisible(true);
	//	m_PubText.SetVisible(true);
	//}
	//else
	//{
	//	m_PubLabel.SetVisible(false);
	//	m_PubText.SetVisible(false);
	//}
	//m_Details.InsertSpacer();





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

void EditPage::SetGame(HyTexturedQuad2d &boxartRef, GameStats &gameStats)
{
	m_GameInfo = Compositorium::Get()->GetGame(gameStats.GetConsole(), gameStats.GetGameId());
	m_pBoxartRef = &boxartRef;

	const glm::mat4 &mtxSceneRef = m_pBoxartRef->GetSceneTransform(0.0f);
	glm::vec3 vScale(1.0f);
	glm::quat quatRot;
	glm::vec3 ptTranslation;
	glm::vec3 vSkew;
	glm::vec4 vPerspective;
	glm::decompose(mtxSceneRef, vScale, quatRot, ptTranslation, vSkew, vPerspective);

	boxartRef.ParentDetach();

	glm::vec2 ptPosStart = ptTranslation;
	glm::vec2 vScaleStart = vScale;
	TransformTexture(*m_pBoxartRef, glm::ivec2(1400, 900), glm::vec2(1920 / 2.0f, 1080 / 2));
	glm::vec2 ptPosDest = m_pBoxartRef->pos.Get();
	glm::vec2 vScaleDest = m_pBoxartRef->scale.Get();

	m_pBoxartRef->pos.Set(ptPosStart);
	m_pBoxartRef->scale.Set(vScaleStart);
	m_pBoxartRef->pos.BezierQuick(ptPosDest, true, 0.64f, 1.0f, HyTween::QuadIn);
	m_pBoxartRef->scale.Tween(vScaleDest.x, vScaleDest.y, 1.0f); //BezierQuick(vScaleDest, true, 0.64f, 1.0f, HyTween::QuadIn);
	m_pBoxartRef->rot.Tween(8.0f * (HyRand::Boolean() ? -1.0f : 1.0f), 0.5f, HyTween::QuadInOut, 0.0f,
		[](IHyNode *pThis)
		{
			static_cast<HyTexturedQuad2d *>(pThis)->rot.Tween(0.0f, 0.5f, HyTween::QuadInOut);
		});

	m_GameTitleLabel.SetText(m_GameInfo.GetName());
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

HyTexturedQuad2d *EditPage::GetBoxart() const
{
	return m_pBoxartRef;
}
