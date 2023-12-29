#include "pch.h"
#include "Crt.h"
#include "VgMusic.h"
#include "Music.h"

#define CRT_SHRINK_AMT 0.01f

Crt::Crt(VgMusic &vgMusicRef, HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_Crt, pParent),
	m_Screen("CRT", "Screen", this),
	m_Channel(this),
	m_Frame("CRT", "Frame", this),
	m_Nob("CRT", "Nob", this),
	m_VcrTimeHrs("CRT", "VCR", this),
	m_VcrTimeMins("CRT", "VCR", this)
{
	const int32 iScreenX = 148;
	const int32 iScreenY = 263;
	
	m_Screen.pos.Set(iScreenX, iScreenY);

	m_Channel.pos.Set(iScreenX + 24, iScreenY + 6);
	m_Channel.scale_pivot.Set(CRT_SCREEN_WIDTH * 0.5f, CRT_SCREEN_HEIGHT * 0.5f);
	m_Channel.scale.Set(CRT_SHRINK_AMT, CRT_SHRINK_AMT);
	m_Channel.SetVisible(false);
	m_Channel.SetStencil(&m_Stencil);
	
	m_Nob.pos.Set(1120.0f, 730.0f);
	m_VcrTimeHrs.pos.Set(614, 1040);
	m_VcrTimeHrs.SetTextAlignment(HYALIGN_Right);
	m_VcrTimeMins.pos.Set(634, 1040);

	m_Stencil.AddMask(m_Screen);

	m_iChannelIndex = 0;
	for(uint32 i = 0; i < CRT_MAX_CHANNELS; ++i)
	{
		Channel *pNewChannel = nullptr;
		if(i == 0)
			pNewChannel = HY_NEW Music(vgMusicRef, &m_Channel);
		//else
		//	pNewChannel = HY_NEW Channel(&m_Channel);
		pNewChannel->SetVisible(i == m_iChannelIndex);
		//pNewChannel->
		m_ChannelList.push_back(pNewChannel);
	}

	pos.Set(548.0f, 0.0f);
}

/*virtual*/ Crt::~Crt()
{
}

HyEntity2d &Crt::GetChannelEntity()
{
	return m_Channel;
}

void Crt::TogglePower()
{
	if(m_Channel.IsVisible() == false)
	{
		m_Channel.SetVisible(true);
		m_Channel.scale.Tween(1.0f, CRT_SHRINK_AMT, 0.2f, HyTween::Linear, 0.0f, [](IHyNode *pThis) { static_cast<HyEntity2d *>(pThis)->scale.Tween(1.0f, 1.0f, 0.4f, HyTween::BounceOut); });
	}
	else
		m_Channel.scale.Tween(CRT_SHRINK_AMT, CRT_SHRINK_AMT, 0.2f, HyTween::BounceIn, 0.0f, [](IHyNode *pThis)
			{
				pThis->SetVisible(false);
			});
}

void Crt::SetChannel(int32 iChannelIndex)
{
	iChannelIndex %= CRT_MAX_CHANNELS;
	iChannelIndex = fabs(iChannelIndex);
	if(m_iChannelIndex == iChannelIndex)
		return;

	m_ChannelList[m_iChannelIndex]->SetVisible(false);
	m_iChannelIndex = iChannelIndex;
	m_ChannelList[m_iChannelIndex]->SetVisible(true);

	m_Nob.rot.Tween((360.0f / CRT_MAX_CHANNELS) * m_iChannelIndex, 0.2f, HyTween::QuadIn);
}

void Crt::NudgeChannel(int32 iIndexOffset)
{
	SetChannel(m_iChannelIndex + iIndexOffset);
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

	m_VcrTimeHrs.SetText(std::to_string(newtime.tm_hour));
	if(newtime.tm_min < 10)
		m_VcrTimeMins.SetText("0" + std::to_string(newtime.tm_min));
	else
		m_VcrTimeMins.SetText(std::to_string(newtime.tm_min));

	if(HyEngine::Input().IsActionReleased(INPUT_CrtPowerToggle))
		TogglePower();

	if(HyEngine::Input().IsActionReleased(INPUT_CrtChannelUp))
		NudgeChannel(1);
	if(HyEngine::Input().IsActionReleased(INPUT_CrtChannelDown))
		NudgeChannel(-1);
}
