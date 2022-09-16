#include "pch.h"
#include "Crt.h"

Crt::Crt(HyEntity2d *pParent /*= nullptr*/) :
	HyEntity2d(pParent),
	m_Screen("CRT", "Screen", this),
	m_Channel(this),
	m_Frame("CRT", "Frame", this),
	m_Nob("CRT", "Nob", this),
	m_VcrTime("CRT", "VCR", this)
{
	const int32 iScreenX = 148;
	const int32 iScreenY = 263;
	
	m_Screen.pos.Set(iScreenX, iScreenY);

	m_Channel.pos.Set(iScreenX, iScreenY);
	m_Channel.scale_pivot.Set(CRT_SCREEN_WIDTH * 0.5f, CRT_SCREEN_HEIGHT * 0.5f);
	m_Channel.scale.Set(0.01f, 0.01f);
	m_Channel.SetVisible(false);
	m_Channel.SetStencil(&m_Stencil);
	
	m_Nob.pos.Set(1120.0f, 730.0f);
	m_VcrTime.pos.Set(584, 1040);

	m_Stencil.AddMask(m_Screen);

	for(uint32 i = 0; i < CRT_MAX_CHANNELS; ++i)
	{
		Channel *pNewChannel = HY_NEW Channel(m_Screen, &m_Channel);
		m_ChannelList.push_back(pNewChannel);
	}
}

/*virtual*/ Crt::~Crt()
{
}

void Crt::TogglePower()
{
	if(m_Channel.IsVisible() == false)
	{
		m_Channel.SetVisible(true);
		m_Channel.scale.Tween(1.0f, 0.01f, 0.2f, HyTween::Linear, [](IHyNode *pThis) { static_cast<HyEntity2d *>(pThis)->scale.Tween(1.0f, 1.0f, 0.4f, HyTween::BounceOut); });
	}
	else
	{
	}
}

void Crt::SetChannel(uint32 uiChannelIndex)
{
	uiChannelIndex = HyClamp(uiChannelIndex, 0u, CRT_MAX_CHANNELS - 1u);
	m_Nob.rot.Tween(uiChannelIndex, 0.2f, HyTween::QuadInOut);
}

/*virtual*/ void Crt::OnUpdate() /*override*/
{
	// VCR Time
	__time64_t long_time;
	_time64(&long_time);
	// Convert to local time.
	tm newtime;
	errno_t err = _localtime64_s(&newtime, &long_time);
	if(newtime.tm_hour > 12)	// Convert from 24-hour
		newtime.tm_hour -= 12;	// to 12-hour clock.
	if(newtime.tm_hour == 0)	// Set hour to 12 if midnight.
		newtime.tm_hour = 12;

	m_VcrTime.SetText(std::to_string(newtime.tm_hour) + " " + std::to_string(newtime.tm_min));

	if(HyEngine::Input().IsActionReleased(INPUT_CrtPowerToggle))
		TogglePower();
}
