#include "pch.h"
#include "MessageCycle.h"
#include "Monitor.h"
#include "CtrlPanel.h"

MessageCycle::MessageCycle(Monitor &monitorRef, HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_MessageCycle, pParent),
	m_MonitorRef(monitorRef),
	m_pCtrlPanel(nullptr),
	m_fXPos(0.0f),
	m_CtrlPanel_GrayBg(HyPanelInit(50, 50, 2), this),
	m_CtrlPanel_LineEdit(HyPanelInit(225, 50, 2, HyColor::Blue), HyNodePath("", "CtrlPanel"), this),
	m_CtrlPanel_AddBtn(HyPanelInit(50, 50, 2, HyColor::Green), HyNodePath("", "CtrlPanel"), this),
	m_CtrlPanel_radLong(HyPanelInit(24, 24, 2), HyNodePath("", "CtrlPanel"), this),
	m_CtrlPanel_radShort(HyPanelInit(24, 24, 2), HyNodePath("", "CtrlPanel"), this),
	m_iCurrMsgIndex(0),
	m_Text("", "MainText", this)
{
	m_CtrlPanel_CheckBox.SetText("Messages");
	m_CtrlPanel_CheckBox.SetCheckedChangedCallback(
		[this](HyCheckBox *pCheckBox)
		{
			if(pCheckBox->IsChecked())
				Show(0.5f);
			else
				Hide(0.5f);
		});

	m_CtrlPanel_GrayBg.SetText("Gray Bar");
	m_CtrlPanel_GrayBg.SetCheckedChangedCallback(
		[this](HyCheckBox *pCheckBox)
		{
			if(pCheckBox->IsChecked())
				m_GrayBg.SetVisible(true);
			else
				m_GrayBg.SetVisible(false);
		});

	m_CtrlPanel_LineEdit.SetOnSubmit([this](HyLineEdit *pThis)
		{
			m_CtrlPanel_AddBtn.InvokeButtonClicked();
		});

	m_CtrlPanel_AddBtn.SetText("ADD");
	m_CtrlPanel_AddBtn.SetButtonClickedCallback([this](HyButton *pThis)
		{
			AddMessage(m_CtrlPanel_LineEdit.GetUtf8String(), m_CtrlPanel_radLong.IsChecked());
		});


	m_CtrlPanel_radShort.SetText("Short");
	m_CtrlPanel_radLong.SetText("Long");
	m_CtrlPanel_radLong.SetChecked(true);

	m_GrayBg.UseWindowCoordinates();
	m_GrayBg.SetAsBox(1920, 208);
	m_GrayBg.SetTint(HyColor(0x1F1F1F));
	m_GrayBg.SetVisible(false);

	m_Text.SetAlignment(HYALIGN_Center);
	
	SetXPosOffset(0.0f);
	pos.Set(m_fXPos, MSGCYCLE_HIDE_YPOS);
}

/*virtual*/ MessageCycle::~MessageCycle()
{
	for(auto pMessage : m_MessageList)
		delete pMessage;
}

void MessageCycle::SetXPosOffset(float fXPosOffset)
{
	m_fXPos = HyEngine::Window(0).GetWidthF(0.5f) + fXPosOffset;
	pos.Tween(m_fXPos, pos.GetAnimFloat(1).GetAnimDestination(), 2.5f, HyTween::QuadInOut);
}

void MessageCycle::AddMessage(const std::string &sMessage, bool bLongDur)
{
	Message *pMessage = HY_NEW Message(bLongDur);

	pMessage->m_Message.SetText(sMessage);
	m_CtrlPanel_LineEdit.SetText("");

	pMessage->m_Remove.SetText("*");
	pMessage->m_Remove.SetButtonClickedCallback(
		[this, pMessage](HyButton *pThis)
		{
			OnRemoveMessage(pMessage);
		});

	pMessage->m_hLayout = m_pCtrlPanel->InsertLayout(HYORIENT_Horizontal);
	m_pCtrlPanel->InsertWidget(pMessage->m_Message, pMessage->m_hLayout);
	m_pCtrlPanel->InsertWidget(pMessage->m_Remove, pMessage->m_hLayout);
	m_pCtrlPanel->InsertSpacer(HYSIZEPOLICY_Expanding, 0, pMessage->m_hLayout);

	m_pCtrlPanel->Load();

	m_MessageList.push_back(pMessage);
	OnNextMsg();
}

void MessageCycle::RemoveMessage(const std::string &sMessage)
{
	Message *pMessage = nullptr;
	for(auto pMsg : m_MessageList)
	{
		if(pMsg->m_Message.GetUtf8String() == sMessage)
		{
			pMessage = pMsg;
			break;
		}
	}
	if(pMessage)
		OnRemoveMessage(pMessage);
}

void MessageCycle::ClearMessages()
{
	for(auto pMessage : m_MessageList)
		m_DeleteList.push_back(pMessage);
	m_MessageList.clear();
}

/*virtual*/ void MessageCycle::PopulateCtrlPanel(CtrlPanel &ctrlPanel) /*override*/
{
	m_pCtrlPanel = &ctrlPanel;

	HyLayoutHandle hRow = ctrlPanel.InsertLayout(HYORIENT_Horizontal);
	ctrlPanel.InsertWidget(m_CtrlPanel_CheckBox, hRow);
	ctrlPanel.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);

	ctrlPanel.InsertWidget(m_CtrlPanel_GrayBg);

	hRow = ctrlPanel.InsertLayout(HYORIENT_Horizontal);
	ctrlPanel.InsertWidget(m_CtrlPanel_LineEdit, hRow);
	ctrlPanel.InsertWidget(m_CtrlPanel_AddBtn, hRow);
	ctrlPanel.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);

	hRow = ctrlPanel.InsertLayout(HYORIENT_Horizontal);
	ctrlPanel.InsertWidget(m_CtrlPanel_radLong, hRow);
	ctrlPanel.InsertWidget(m_CtrlPanel_radShort, hRow);
	ctrlPanel.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);

	ctrlPanel.InsertSpacer();
}

/*virtual*/ void MessageCycle::Show(float fDuration) /*override*/
{
	SetVisible(true);
	pos.Tween(m_fXPos, MSGCYCLE_SHOW_YPOS, 1.25f, HyTween::QuadOut);
}

/*virtual*/ void MessageCycle::Hide(float fDuration) /*override*/
{
	pos.Tween(m_fXPos, MSGCYCLE_HIDE_YPOS, fDuration, HyTween::QuadIn, 0.0f, [this](IHyNode *pThis) { pThis->SetVisible(false); });
}

void MessageCycle::OnRemoveMessage(Message *pMessage)
{
	auto iter = std::find(m_MessageList.begin(), m_MessageList.end(), pMessage);
	if(iter != m_MessageList.end())
	{
		m_MessageList.erase(iter);
		m_DeleteList.push_back(pMessage);
	}

	OnNextMsg();
}

/*virtual*/ void MessageCycle::OnUpdate() /*override*/
{
	if(m_DeleteList.empty() == false)
	{
		for(auto pMessage : m_DeleteList)
		{
			m_pCtrlPanel->RemoveWidget(pMessage->m_Message);
			m_pCtrlPanel->RemoveWidget(pMessage->m_Remove);
			m_pCtrlPanel->RemoveLayout(pMessage->m_hLayout);
			delete pMessage;
		}
		m_DeleteList.clear();
	}

	if(m_MessageList.empty())
		return;

	if(m_Text.pos.IsAnimating() == false)
	{
		if(m_Text.pos.Y() >= 0.0f)
			m_Text.pos.Tween(0.0f, -6.0f, 6.0f, HyTween::QuadInOut);
		else
			m_Text.pos.Tween(0.0f, 6.0f, 6.0f, HyTween::QuadInOut);
	}

	if(m_Timer.IsExpired() && m_MessageList.size() > 1)
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
	if(m_MessageList.empty())
	{
		m_Text.SetText("");
		return;
	}

	m_iCurrMsgIndex = (m_iCurrMsgIndex + 1) % m_MessageList.size();
	m_Text.SetText(m_MessageList[m_iCurrMsgIndex]->GetMsg());

	if(m_MonitorRef.IsBrb())
		m_Timer.InitStart(MSGCYCLE_DURATION_SHORT);
	else
		m_Timer.InitStart(m_MessageList[m_iCurrMsgIndex]->GetDuration());
}
