#ifndef Collection_h__
#define Collection_h__

#include "pch.h"

class EditPage : public HyUiContainer
{
	HyLabel			m_GameTitleLabel;
	HyButton		m_BackBtn;
	HyLabel			m_BoxartLabel;
	HyButton		m_ForwardBtn;
	
	HyCheckBox		m_GameBlindCheckBox;
	HyCheckBox		m_GameOwnedCheckBox;
	HyCheckBox		m_GameWishlistCheckBox;
	HyCheckBox		m_GamePlayedCheckBox;
	HyCheckBox		m_GameInterestedCheckBox;
	HyCheckBox		m_GameEvergreenCheckBox;
	HyCheckBox		m_GameBeatenCheckBox;
	HyCheckBox		m_Game100PercentCheckBox;
	HyCheckBox		m_GameSpeedrunningCheckBox;

	HyLabel			m_GameFirstPlayedOnStreamLabel;
	HyButton		m_GameFirstPlayedOnStreamBtn;
	
	HyLabel			m_GameElapsedPlayTimeLabel;
	HyButton		m_GameElapsedPlayTimeResetBtn;
	HyButton		m_GameElapsedPlayTimeHrBckBtn;
	HyButton		m_GameElapsedPlayTimeMinBckBtn;
	HyButton		m_GameElapsedPlayTimeMinFwdBtn;
	HyButton		m_GameElapsedPlayTimeHrFwdBtn;

	HyLabel			m_GameBeatenOnStreamLabel;
	HyButton		m_GameBeatenOnStreamBtn;

	HyLineEdit		m_NotesLineEdit;

	//HyButton		m_SaveBtn;
	//HyButton		m_ResetBtn;

public:
	EditPage(HyEntity2d *pParent);
	virtual ~EditPage();

	void SetGame(GameStats &gameStats);

protected:
	
};

class Collection : public HyEntity2d
{
	EditPage		m_EditPage;

public:
	Collection(HyEntity2d *pParent);
	virtual ~Collection();
};

#endif // Collection_h__
