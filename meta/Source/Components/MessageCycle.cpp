#include "pch.h"
#include "MessageCycle.h"
#include "CtrlPanel.h"

#define MSGCYCLE_HIDE_YPOS		-100.0f
#define MSGCYCLE_SHOW_YPOS		100.0f

MessageCycle::MessageCycle(HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_MessageCycle, pParent),
	m_pCtrlPanel(nullptr),
	m_CtrlPanel_CheckBox(HyPanelInit(32, 32, 2), HyNodePath("", "CtrlPanel")),
	m_CtrlPanel_LineEdit(HyPanelInit(225, 50, 2, HyColor::Blue), HyNodePath("", "CtrlPanel"), this),
	m_CtrlPanel_AddBtn(HyPanelInit(50, 50, 2, HyColor::Green), HyNodePath("", "CtrlPanel"), this),
	m_iCurrMsgIndex(0),
	m_Text("", "MainText", this)
{
	m_CtrlPanel_CheckBox.SetText("Messages");
	m_CtrlPanel_CheckBox.SetCheckedChangedCallback(
		[this](HyCheckBox *pCheckBox, void *pData)
		{
			if(pCheckBox->IsChecked())
				reinterpret_cast<IComponent *>(pData)->Show(0.5f);
			else
				reinterpret_cast<IComponent *>(pData)->Hide(0.5f);
		}, this);

	m_CtrlPanel_AddBtn.SetText("ADD");
	m_CtrlPanel_AddBtn.SetButtonClickedCallback([this](HyButton *pThis, void *pData)
		{
			Message *pMessage = HY_NEW Message();
			pMessage->m_Message.SetText(m_CtrlPanel_LineEdit.GetUtf8String());
			pMessage->m_Remove.SetText("*");
			pMessage->m_Remove.SetButtonClickedCallback(
				[this](HyButton *pThis, void *pData)
				{
					Message *pMessage = reinterpret_cast<Message *>(pData);
					OnRemoveMessage(pMessage);
				}, pData);

			HyLayoutHandle hLayout = m_pCtrlPanel->InsertLayout(HYORIEN_Horizontal);
			m_pCtrlPanel->InsertWidget(pMessage->m_Message, hLayout);
			m_pCtrlPanel->InsertWidget(pMessage->m_Remove, hLayout);
			m_pCtrlPanel->InsertSpacer(HYSIZEPOLICY_Expanding, 0, hLayout);

			m_pCtrlPanel->Load();

			m_MessageList.push_back(pMessage);

			std::vector<std::string> msgList;
			for(auto msg : m_MessageList)
				msgList.push_back(msg->m_Message.GetUtf8String());

			SetMsgs(msgList, 5.0f);
		});

	m_Text.SetTextAlignment(HYALIGN_Center);
	
	pos.Set(pos.GetX(), MSGCYCLE_HIDE_YPOS);
}

/*virtual*/ MessageCycle::~MessageCycle()
{
	for(auto pMessage : m_MessageList)
		delete pMessage;
}

/*virtual*/ void MessageCycle::PopulateCtrlPanel(CtrlPanel &ctrlPanel) /*override*/
{
	m_pCtrlPanel = &ctrlPanel;
	ctrlPanel.InsertSpacer();

	HyLayoutHandle hRow = ctrlPanel.InsertLayout(HYORIEN_Horizontal);
	ctrlPanel.InsertWidget(m_CtrlPanel_CheckBox, hRow);
	ctrlPanel.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);

	hRow = ctrlPanel.InsertLayout(HYORIEN_Horizontal);
	ctrlPanel.InsertWidget(m_CtrlPanel_LineEdit, hRow);
	ctrlPanel.InsertWidget(m_CtrlPanel_AddBtn, hRow);
	ctrlPanel.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);
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
	m_Timer.InitStart(fCycleDuration);

	if(IsVisible())
		m_Timer.Resume();
}

void MessageCycle::OnRemoveMessage(Message *pMessage)
{
	auto iter = std::find(m_MessageList.begin(), m_MessageList.end(), pMessage);
	if(iter != m_MessageList.end())
	{
		m_MessageList.erase(iter);
		delete pMessage;
	}

	SetMsgs(m_sMsgList, m_Timer.GetDuration());
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
