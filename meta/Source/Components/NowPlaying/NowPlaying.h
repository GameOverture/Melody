#ifndef NowPlaying_h__
#define NowPlaying_h__

#include "pch.h"
#include "IComponent.h"

class NowPlaying : public IComponent
{
	HyButton						m_CtrlPanel_BrowseBtn;
	HyCheckBox						m_CtrlPanel_StopwatchCheckBox;
	HyButton						m_CtrlPanel_StopwatchResetBtn;
	HyButton						m_CtrlPanel_StopwatchHrBck;
	HyButton						m_CtrlPanel_StopwatchMinBck;
	HyButton						m_CtrlPanel_StopwatchStartPause;
	HyButton						m_CtrlPanel_StopwatchMinFwd;
	HyButton						m_CtrlPanel_StopwatchHrFwd;

	HyEntity2d						m_InfoEnt;
	std::vector<HyTexturedQuad2d *>	m_SlideShowList;
	int32							m_iSlideShowIndex;
	HyTimer							m_SlideShowTimer;
	HyPrimitive2d					m_DescriptionArea;
	HyText2d						m_DescriptionText;
	HyUiContainer					m_Details;
	HyLabel							m_ReleaseLabel;
	HyLabel							m_ReleaseText;
	HyLabel							m_DevLabel;
	HyLabel							m_DevText;
	HyLabel							m_PubLabel;
	HyLabel							m_PubText;
	
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

	void ShowGameTime(bool bShow);

	virtual void Show(float fDuration) override;
	virtual void Hide(float fDuration) override;

protected:
	virtual void OnUpdate() override;
};

#endif // NowPlaying_h__
