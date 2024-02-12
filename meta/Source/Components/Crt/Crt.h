#ifndef Crt_h__
#define Crt_h__

#include "pch.h"
#include "IComponent.h"
#include "Channel.h"
#include "MessageCycle.h"
#include "VgMusic.h"
#include "Music.h"

class VgMusic;

#define NUM_VOLUME_BARS 20

class Crt : public IComponent
{
	HyCheckBox				m_CtrlPanel_CheckBox;
	HyButton				m_CtrlPanel_ZoomBtn;
	HyButton				m_CtrlPanel_UnzoomBtn;

	int32					m_iChannelIndex;

	HySprite2d				m_Screen;
	HyEntity2d				m_ChannelStack;
	HySprite2d				m_ScreenOverlay;
	HySprite2d				m_Static;
	HySprite2d				m_Frame;
	HySprite2d				m_Nob;

	HyText2d				m_VcrTimeHrs;
	HyText2d				m_VcrTimeMins;

	HyText2d				m_VolumeText;
	HyPrimitive2d			m_VolumeBar[NUM_VOLUME_BARS];
	float					m_fVolumeShowTime;
	
	HyStencil				m_Stencil;

	Channel					m_ChannelStatic;
	Music					m_ChannelMusic;
	Channel					m_ChannelGame;
	
	std::vector<Channel *>	m_ChannelList;
	
	enum CrtState
	{
		CRTSTATE_Off,
		CRTSTATE_PreChangeChannel,
		CRTSTATE_ChangeChannel,
		CRTSTATE_PostChangeChannel,
		CRTSTATE_Idle
	};
	CrtState				m_eChannelState;
	float					m_fElapsedTime;

public:
	Crt(VgMusic &vgMusicRef, HyEntity2d *pParent = nullptr);
	virtual ~Crt();

	virtual void PopulateCtrlPanel(CtrlPanel &ctrlPanel) override;

	virtual void Show(float fDuration) override;
	virtual void Hide(float fDuration) override;

	bool IsPowerOn() const;
	void TogglePower(bool bPowerOn);

	void SetChannel(int32 iChannelIndex);
	void NudgeChannel(int32 iIndexOffset);

	void SetVolume(float fVolume);

protected:
	virtual void OnUpdate() override;
};

#endif // Crt_h__
