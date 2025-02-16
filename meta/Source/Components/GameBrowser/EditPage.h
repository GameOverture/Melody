#ifndef EditPage_h__
#define EditPage_h__

#include "pch.h"

class EditPage : public HyUiContainer
{
	HyTexturedQuad2d *	m_pBoxartRef;

	HyButton			m_GameTitle;
	HyLabel				m_GameBoxart;
	HyLabel				m_GameMedia;

	GameInfo			m_Info;
	HyLabel				m_InfoDescription;
	HyLabel				m_InfoReleasedLabel;
	HyLabel				m_InfoReleasedText;
	HyLabel 			m_InfoGenreLabel;
	HyLabel 			m_InfoGenreText;
	HyLabel				m_InfoDeveloperLabel;
	HyLabel				m_InfoDeveloperText;
	HyLabel				m_InfoPublisherLabel;
	HyLabel				m_InfoPublisherText;

	HyCheckBox			m_DataBlindCheckBox;
	HyCheckBox			m_DataOwnedCheckBox;
	HyCheckBox			m_DataWishlistCheckBox;
	HyCheckBox			m_DataPlayedCheckBox;
	HyCheckBox			m_DataInterestedCheckBox;
	HyCheckBox			m_DataEvergreenCheckBox;
	HyCheckBox			m_DataBeatenCheckBox;
	HyCheckBox			m_Data100PercentCheckBox;
	HyCheckBox			m_DataSpeedrunningCheckBox;

	HyLabel				m_DataFirstPlayedOnStreamLabel;
	HyLabel				m_DataFirstPlayedOnStreamValue;
	HyButton			m_DataFirstPlayedOnStreamBtn;

	HyLabel				m_DataElapsedPlayTimeLabel;
	HyButton			m_DataElapsedPlayTimeResetBtn;
	HyButton			m_DataElapsedPlayTimeHrBckBtn;
	HyButton			m_DataElapsedPlayTimeMinBckBtn;
	HyButton			m_DataElapsedPlayTimeMinFwdBtn;
	HyButton			m_DataElapsedPlayTimeHrFwdBtn;

	HyLabel				m_DataBeatenOnStreamLabel;
	HyLabel				m_DataBeatenOnStreamValue;
	HyButton			m_DataBeatenOnStreamBtn;

	HyLineEdit			m_DataNotes;

	//HyButton			m_SaveBtn;
	//HyButton			m_ResetBtn;

public:
	EditPage(HyEntity2d *pParent);
	virtual ~EditPage();

	void SetGame(HyTexturedQuad2d &boxartRef, glm::vec2 ptBoxartPos, GameStats &gameStats);

	HyTexturedQuad2d *GetBoxart() const;

protected:

};

#endif // EditPage_h__
