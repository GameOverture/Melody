#ifndef Crt_h__
#define Crt_h__

#include "pch.h"
#include "Channel.h"

#define CRT_MAX_CHANNELS 10

class Crt : public HyEntity2d
{
	std::vector<Channel *>	m_ChannelList;

	HySprite2d				m_Screen;
	HyEntity2d				m_Channel;
	HySprite2d				m_Frame;
	HySprite2d				m_Nob;

	HyText2d				m_VcrTime;
	
	HyStencil				m_Stencil;

public:
	Crt(HyEntity2d *pParent = nullptr);
	virtual ~Crt();

	void TogglePower();

	void SetChannel(uint32 uiChannelIndex);

protected:
	virtual void OnUpdate() override;
};

#endif // Crt_h__
