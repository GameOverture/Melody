#ifndef Crt_h__
#define Crt_h__

#include "pch.h"
#include "IComponent.h"
#include "Channel.h"
#include "MessageCycle.h"
#include "VgMusic.h"
#include "Music.h"

class Monitor;
class VgMusic;
class InputViewer;

#define NUM_VOLUME_BARS 20

class Crt : public IComponent
{
	MessageCycle &			m_MsgCycleRef;
	InputViewer &			m_InputViewerRef;

	HyCheckBox				m_CtrlPanel_CheckBox;
	HyButton				m_CtrlPanel_btnGame;
	HyButton				m_CtrlPanel_btnMusic;
	HyButton				m_CtrlPanel_btnStatic;

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

	HyText2d				m_ChannelText;
	float					m_fChannelShowTime;
	
	HyStencil				m_Stencil;

	Channel					m_ChannelStatic;
	Music					m_ChannelMusic;
	Channel					m_ChannelGame;
	
	std::vector<Channel *>	m_ChannelList;
	
	enum CrtState
	{
		CRTSTATE_Off,
		CRTSTATE_PreChangeChannel,
		CRTSTATE_ChangingChannel,
		CRTSTATE_PostChangeChannel,
		CRTSTATE_Idle
	};
	CrtState				m_eChannelState;
	float					m_fElapsedTime;

public:
	Crt(VgMusic &vgMusicRef, MessageCycle &msgCycleRef, InputViewer &inputViewerRef, HyEntity2d *pParent = nullptr);
	virtual ~Crt();

	virtual void PopulateCtrlPanel(CtrlPanel &ctrlPanel) override;

	virtual void Show(float fDuration) override;
	virtual void Hide(float fDuration) override;

	bool IsPowerOn() const;
	void TogglePower(bool bPowerOn);

	int32 GetChannel() const;
	void SetChannel(int32 iChannelIndex);
	void NudgeChannel(int32 iIndexOffset);

	void SetVolume(float fVolume);

protected:
	virtual void OnUpdate() override;
};

#endif // Crt_h__
