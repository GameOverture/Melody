#ifndef Crt_h__
#define Crt_h__

#include "pch.h"
#include "IComponent.h"
#include "Channel.h"
#include "Message.h"

class VgMusic;

class Crt : public IComponent
{
	int32					m_iChannelIndex;

	HySprite2d				m_Screen;
	HyEntity2d				m_ChannelStack;		// Only holds IChannel entities
	HySprite2d				m_ScreenOverlay;
	HySprite2d				m_Frame;
	HySprite2d				m_Nob;

	HyText2d				m_VcrTimeHrs;
	HyText2d				m_VcrTimeMins;
	
	HyStencil				m_Stencil;

	Message					m_Message;

public:
	Crt(VgMusic &vgMusicRef, HyEntity2d *pParent = nullptr);
	virtual ~Crt();

	virtual void Show(float fDuration) override;
	virtual void Hide(float fDuration) override;

	bool IsPowerOn() const;
	void TogglePower(bool bPowerOn);

	void SetChannel(int32 iChannelIndex);
	void NudgeChannel(int32 iIndexOffset);

protected:
	virtual void OnUpdate() override;
};

#endif // Crt_h__
