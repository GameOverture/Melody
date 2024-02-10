#include "pch.h"
#include "MessageCycle.h"

#define MSGCYCLE_HIDE_YPOS		-100.0f
#define MSGCYCLE_SHOW_YPOS		100.0f

MessageCycle::MessageCycle(HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_MessageCycle, pParent),
	m_iCurrMsgIndex(0),
	m_Text("", "MainText", this)
{
	m_Text.SetTextAlignment(HYALIGN_Center);
	
	pos.Set(HyEngine::Window().GetWidthF(0.5f), MSGCYCLE_HIDE_YPOS);
}

/*virtual*/ MessageCycle::~MessageCycle()
{
}

/*virtual*/ void MessageCycle::Show(float fDuration) /*override*/
{
	SetVisible(true);
	pos.Tween(HyEngine::Window().GetWidthF(0.5f), MSGCYCLE_SHOW_YPOS, 1.25f, HyTween::QuadOut);
}

/*virtual*/ void MessageCycle::Hide(float fDuration) /*override*/
{
	pos.Tween(HyEngine::Window().GetWidthF(0.5f), MSGCYCLE_HIDE_YPOS, fDuration, HyTween::QuadIn, 0.0f, [this](IHyNode *pThis) { pThis->SetVisible(false); });
}

void MessageCycle::SetMsgs(std::vector<std::string> &sMsgList, float fCycleDuration)
{
	m_sMsgList = sMsgList;
	m_Timer.Init(fCycleDuration);

	if(IsVisible())
		m_Timer.Resume();
}

/*virtual*/ void MessageCycle::OnUpdate() /*override*/
{
	if(IsVisible() == false || m_sMsgList.empty())
		return;

	if(m_Text.pos.IsAnimating() == false)
	{
		if(m_Text.pos.Y() >= 0.0f)
			m_Text.pos.Tween(0.0f, -6.0f, 6.0f, HyTween::QuadInOut);
		else
			m_Text.pos.Tween(0.0f, 6.0f, 6.0f, HyTween::QuadInOut);
	}

	if(m_Timer.IsExpired() && m_sMsgList.size() > 1)
	{
		if(m_Text.alpha.IsAnimating() == false && m_Text.alpha.Get() == 1.0f)
		{
			m_Text.alpha.Tween(0.0f, 1.0f, HyTween::Linear, 0.0f, 
				[this](IHyNode *pThis)
				{
					OnNextMsg();
					static_cast<HyText2d *>(pThis)->alpha.Tween(1.0f, 1.0f);
				});
		}
	}
}

void MessageCycle::OnNextMsg()
{
	m_iCurrMsgIndex = (m_iCurrMsgIndex + 1) % m_sMsgList.size();
	m_Text.SetText(m_sMsgList[m_iCurrMsgIndex]);

	m_Timer.Reset();
	m_Timer.Start();
}
