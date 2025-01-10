#ifndef Monitor_h__
#define Monitor_h__

#include "pch.h"
#include "IComponent.h"

class Monitor : public IComponent
{
	HyCheckBox			m_CtrlPanel_CheckBox;
	HyCheckBox			m_CtrlPanel_LiveSplit;

	enum MonitorChannel
	{
		MONITORCHANNEL_NoSignal = 0,
		MONITORCHANNEL_ObsFull,
		MONITORCHANNEL_ObsPartial,
		MONITORCHANNEL_Brb,

		NUM_MONITORCHANNELS
	};
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

	HyPrimitive2d		m_LiveSplitMask;
	HySprite2d			m_Shadow;
	HyPrimitive2d		m_Background;
	HyPrimitive2d		m_ObsMask;
	HySprite2d			m_Brb;
	HySprite2d			m_Frame;

	HyStopwatch			m_ElapsedTime;
	HyText2d			m_ElapsedTimeText;
	
	HyText2d			m_ChannelText;

public:
	Monitor(HyEntity2d *pParent = nullptr);
	virtual ~Monitor();

	virtual void PopulateCtrlPanel(CtrlPanel &ctrlPanel) override;

	virtual void Show(float fDuration) override;
	virtual void Hide(float fDuration) override;

	bool IsDivider() const;
	bool IsBrb() const;

protected:
	virtual void OnUpdate() override;

	void SetChannel(int iChannelIndex);
};

#endif // Monitor_h__
