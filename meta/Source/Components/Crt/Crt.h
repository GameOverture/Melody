#ifndef Crt_h__
#define Crt_h__

#include "pch.h"
#include "IComponent.h"
#include "Channel.h"
#include "Message.h"

#define CRT_MAX_CHANNELS 1//0

class Crt : public IComponent
{
	std::vector<Channel *>	m_ChannelList;
	int32					m_iChannelIndex;

	HySprite2d				m_Screen;
	HyEntity2d				m_Channel;
	HySprite2d				m_Frame;
	HySprite2d				m_Nob;

	HyText2d				m_VcrTimeHrs;
	HyText2d				m_VcrTimeMins;
	
	HyStencil				m_Stencil;

	Message					m_Message;

public:
	Crt(HyEntity2d *pParent = nullptr);
	virtual ~Crt();

	void TogglePower();

	void SetChannel(int32 iChannelIndex);
	void NudgeChannel(int32 iIndexOffset);

protected:
	virtual void OnUpdate() override;
};

#endif // Crt_h__
