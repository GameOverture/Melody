#include "pch.h"
#include "Crt.h"
#include "VgMusic.h"
#include "Music.h"

#define CRT_SHRINK_AMT 0.01f
#define CRT_SHUTOFF_DUR 0.2f

Crt::Crt(VgMusic &vgMusicRef, HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_Crt, pParent),
	m_iChannelIndex(0),
	m_Screen("CRT", "Screen", this),
	m_ChannelStack(this),
	m_ScreenOverlay("CRT", "ScreenOverlay", this),
	m_Frame("CRT", "Frame", this),
	m_Nob("CRT", "Nob", this),
	m_VcrTimeHrs("CRT", "VCR", this),
	m_VcrTimeMins("CRT", "VCR", this)
{
	const int32 iScreenX = 148;
	const int32 iScreenY = 263;
	
	m_Screen.pos.Set(iScreenX, iScreenY);
	m_ScreenOverlay.pos.Set(iScreenX, iScreenY);

	m_ChannelStack.pos.Set(iScreenX + 24, iScreenY + 6);
	m_ChannelStack.scale_pivot.Set(CRT_SCREEN_WIDTH * 0.5f, CRT_SCREEN_HEIGHT * 0.5f);
	m_ChannelStack.scale.Set(CRT_SHRINK_AMT, CRT_SHRINK_AMT);
	m_ChannelStack.SetVisible(false);
	m_ChannelStack.SetStencil(&m_Stencil);
	//m_ChannelStack.SetPostProcessing(
	
	m_Nob.pos.Set(1120.0f, 730.0f);
	m_VcrTimeHrs.pos.Set(620, 1050);
	m_VcrTimeHrs.SetTextAlignment(HYALIGN_Right);
	m_VcrTimeMins.pos.Set(640, 1050);

	m_VolumeText.Init("CRT", "Volume", &m_ChannelStack);
	m_VolumeText.SetText("VOLUME");
	m_VolumeText.pos.Set(95, 160);
	m_VolumeText.SetDisplayOrder(DISPLAYORDER_CrtVolume);
	m_VolumeText.SetVisible(false);
	for(int i = 0; i < NUM_VOLUME_BARS; ++i)
	{
		m_VolumeBar[i].SetVisible(false);
		m_VolumeBar[i].SetTint(HyColor::Green);
		m_VolumeBar[i].SetAsBox(24.0f, 48.0f);
		m_VolumeBar[i].pos.Set(100 + (i * 32), 100);
		m_VolumeBar[i].SetDisplayOrder(DISPLAYORDER_CrtVolume);
		
		m_ChannelStack.ChildAppend(m_VolumeBar[i]);
	}

	m_Stencil.AddMask(m_Screen);

	HY_NEW Channel(CHANNELTYPE_Static, &m_ChannelStack);
	HY_NEW Music(vgMusicRef, &m_ChannelStack);

	pos.Set(548.0f, 0.0f);
}

/*virtual*/ Crt::~Crt()
{
	for(int i = 0; i < m_ChannelStack.ChildCount(); ++i)
		delete m_ChannelStack.ChildGet(i);
}

/*virtual*/ void Crt::Show(float fDuration) /*override*/
{
	alpha.Set(0.0f);
	alpha.Tween(1.0f, fDuration, HyTween::Linear, 0.0f, [this](IHyNode *pThis) { TogglePower(true); });
	SetVisible(true);
}

/*virtual*/ void Crt::Hide(float fDuration) /*override*/
{
	TogglePower(false);
	alpha.Tween(0.0f, fDuration, HyTween::Linear, CRT_SHUTOFF_DUR + 1.0f, [this](IHyNode *pThis) { pThis->SetVisible(false); });
}

bool Crt::IsPowerOn() const
{
	return m_ChannelStack.IsVisible();
}

void Crt::TogglePower(bool bPowerOn)
{
	if(bPowerOn)
	{
		if(IsPowerOn() == false)
		{
			m_ChannelStack.SetVisible(true);
			m_ChannelStack.scale.Tween(1.0f, CRT_SHRINK_AMT, 0.2f, HyTween::Linear, 0.0f, [](IHyNode *pThis) { static_cast<HyEntity2d *>(pThis)->scale.Tween(1.0f, 1.0f, 0.4f, HyTween::BounceOut); });
		}
	}
	else
	{
		if(IsPowerOn())
			m_ChannelStack.scale.Tween(CRT_SHRINK_AMT, CRT_SHRINK_AMT, CRT_SHUTOFF_DUR, HyTween::BounceIn, 0.0f, [](IHyNode *pThis) {  pThis->SetVisible(false); });
	}
}

void Crt::SetChannel(int32 iChannelIndex)
{
	iChannelIndex %= NUM_CHANNELTYPE;
	iChannelIndex = fabs(iChannelIndex);
	if(m_iChannelIndex == iChannelIndex)
		return;

	//m_ChannelList[m_iChannelIndex]->SetVisible(false);
	m_iChannelIndex = iChannelIndex;
	//m_ChannelList[m_iChannelIndex]->SetVisible(true);

	m_Nob.rot.Tween((360.0f / NUM_CHANNELTYPE) * m_iChannelIndex, 0.2f, HyTween::QuadIn);
}

void Crt::NudgeChannel(int32 iIndexOffset)
{
	SetChannel(m_iChannelIndex + iIndexOffset);
}

void Crt::SetVolume(float fVolume)
{
	m_VolumeText.SetVisible(true);
	for(int i = 0; i < NUM_VOLUME_BARS; ++i)
	{
		if(fVolume >= 0.05f)
		{
			m_VolumeBar[i].SetVisible(true);
			fVolume -= 0.05f;
		}
		else
			m_VolumeBar[i].SetVisible(false);
	}

	m_fVolumeShowTime = 2.0f;
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
		TogglePower(!IsPowerOn());

	if(HyEngine::Input().IsActionReleased(INPUT_CrtChannelUp))
		NudgeChannel(1);
	if(HyEngine::Input().IsActionReleased(INPUT_CrtChannelDown))
		NudgeChannel(-1);

	if(m_fVolumeShowTime > 0.0f)
	{
		m_fVolumeShowTime -= HyEngine::DeltaTime();
		if(m_fVolumeShowTime <= 0.0f)
		{
			m_fVolumeShowTime = 0.0f;
			m_VolumeText.SetVisible(false);
			for(int i = 0; i < NUM_VOLUME_BARS; ++i)
				m_VolumeBar[i].SetVisible(false);
		}
	}
}
