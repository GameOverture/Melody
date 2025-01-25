#ifndef Monitor_h__
#define Monitor_h__

#include "pch.h"
#include "IComponent.h"

#define SHADOW_ALPHA 0.42f

enum MonitorChannel
{
	MONITORCHANNEL_NoSignal = 0,
	MONITORCHANNEL_ObsFull,
	//MONITORCHANNEL_ObsPartial,
	MONITORCHANNEL_Brb,

	NUM_MONITORCHANNELS
};

class Monitor : public IComponent
{
	HyRadioButton		m_CtrlPanel_radChannel[NUM_MONITORCHANNELS];
	HyButtonGroup		m_BtnGrp;
	int32				m_iChannelIndex;
	float				m_fChannelShowTime;
	enum MonitorState
	{
		MONITORSTATE_Idle,
		MONITORSTATE_PreChangeChannel,
		MONITORSTATE_ChangingChannel,
		MONITORSTATE_PostChangeChannel,
	};
	MonitorState		m_eMonitorState;
	float				m_fElapsedTime;

	HySprite2d			m_Shadow;
	HyPrimitive2d		m_Background;
	HySprite2d			m_Brb;
	HySprite2d			m_Frame;
	HyPrimitive2d		m_NoSignal;
	HyPrimitive2d		m_ObsMask;

	HyStopwatch			m_ElapsedTime;
	HyText2d			m_ElapsedTimeText;
	
	HyText2d			m_ChannelText;

public:
	Monitor(HyEntity2d *pParent = nullptr);
	virtual ~Monitor();

	void SetChannel(MonitorChannel eChannel);

	HySprite2d &GetShadow();

	virtual void PopulateCtrlPanel(CtrlPanel &ctrlPanel) override;

	virtual void Show(float fDuration) override;
	virtual void Hide(float fDuration) override;

	bool IsBrb() const;

protected:
	virtual void OnUpdate() override;

	void OnChannelChange(int iChannelIndex);
};

#endif // Monitor_h__
