#ifndef NowPlaying_h__
#define NowPlaying_h__

#include "pch.h"
#include "IComponent.h"

class NowPlaying : public IComponent
{
	HyCheckBox						m_CtrlPanel_CheckBox;
	HyButton						m_CtrlPanel_BrowseBtn;

	HyEntity2d						m_InfoEnt;
	std::vector<HyTexturedQuad2d *>	m_BoxArtList;
	int32							m_iBoxArtIndex;
	std::vector<HyTexturedQuad2d *>	m_TitleArtList;
	int32							m_iTitleArtIndex;
	HyText2d						m_DescriptionText;
	
	HyEntity2d						m_NowPlayingEnt;
	HyTexturedQuad2d				m_Logo;
	HyText2d						m_NowPlayingText;
	HyText2d						m_GameNameText;

	HyEntity2d						m_TimeEnt;
	HyStopwatch						m_Stopwatch;
	HyLabel							m_TimeLabel;
	HyText2d						m_GameTimeText;

	enum ReloadState
	{
		RELOADSTATE_Idle,
		RELOADSTATE_FadingOut,
		RELOADSTATE_Reload,
	};
	ReloadState						m_eReloadState;
	std::string						m_sHtmlFilePath;

public:
	NowPlaying(HyEntity2d *pParent = nullptr);
	virtual ~NowPlaying();

	virtual void PopulateCtrlPanel(CtrlPanel &ctrlPanel) override;

protected:
	virtual void OnUpdate() override;
};

#endif // NowPlaying_h__
