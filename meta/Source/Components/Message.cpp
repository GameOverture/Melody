#include "pch.h"
#include "Message.h"

Message::Message(HyEntity2d *pParent /*= nullptr*/) :
	HyEntity2d(pParent),
	m_MainText("", "MainText", this),
	m_fMainDuration(0.0f),
	m_fSubDuration(0.0f)
{
	m_MainText.SetTextAlignment(HYALIGN_Center);
}

/*virtual*/ Message::~Message()
{
}

void Message::SetMsgs(std::string sMain, std::string sSub, float fMainDuration, float fSubDuration)
{
	m_sMain = sMain;
	m_sSub = sSub;
	m_fMainDuration = fMainDuration;
	m_fSubDuration = fSubDuration;

	m_MainText.SetText(m_sMain);
	m_Timer.InitStart(m_fMainDuration);
}

/*virtual*/ void Message::OnUpdate() /*override*/
{
	if(m_MainText.pos.IsAnimating() == false)
	{
		if(m_MainText.pos.Y() >= 0.0f)
			m_MainText.pos.Tween(0.0f, -6.0f, 6.0f, HyTween::QuadInOut);
		else
			m_MainText.pos.Tween(0.0f, 6.0f, 6.0f, HyTween::QuadInOut);
	}

	if(m_Timer.IsExpired() && m_sSub.empty() == false)
	{
		if(m_sMain == m_MainText.GetUtf8String())
		{
			if(m_MainText.alpha.IsAnimating() == false && m_MainText.alpha.Get() == 1.0f)
			{
				m_MainText.alpha.Tween(0.0f, 1.0f, HyTween::Linear, 0.0f, [&](IHyNode *pThis) {
					static_cast<HyText2d *>(pThis)->SetText(m_sSub);
					static_cast<HyText2d *>(pThis)->alpha.Tween(1.0f, 1.0f);
					static_cast<HyText2d *>(pThis)->SetState(1);
					m_Timer.InitStart(m_fSubDuration);
					});
				
			}
		}
		else // sub text
		{
			if(m_MainText.alpha.IsAnimating() == false && m_MainText.alpha.Get() == 1.0f)
			{
				m_MainText.alpha.Tween(0.0f, 1.0f, HyTween::Linear, 0.0f, [&](IHyNode *pThis) {
					static_cast<HyText2d *>(pThis)->SetText(m_sMain);
					static_cast<HyText2d *>(pThis)->alpha.Tween(1.0f, 1.0f);
					static_cast<HyText2d *>(pThis)->SetState(0);
					m_Timer.InitStart(m_fMainDuration);
					});
			}
			
		}
	}
}
