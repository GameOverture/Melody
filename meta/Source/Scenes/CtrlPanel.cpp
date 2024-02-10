#include "pch.h"
#include "CtrlPanel.h"
#include "IComponent.h"

CtrlPanel::CtrlPanel(HyEntity2d *pParent /*= nullptr*/) :
	HyUiContainer(HYORIEN_Vertical, HyPanelInit(), pParent),
	m_MessagesLineEdit(HyPanelInit(350, 50, 2, HyColor::Blue), HyNodePath("", "CtrlPanel"), this),
	m_AddMessageBtn(HyPanelInit(50, 50, 2, HyColor::Green), HyNodePath("", "CtrlPanel"), this)
{
	m_AddMessageBtn.SetText("ADD");
	m_AddMessageBtn.SetButtonClickedCallback([this](HyButton *pThis, void *pData)
	{
		Message *pMessage = HY_NEW Message(*this, m_MessagesLineEdit.GetUtf8String());
		m_MessageList.push_back(pMessage);
	});
	SetSize(420, 1000);
}

/*virtual*/ CtrlPanel::~CtrlPanel()
{
	for(auto pMessage : m_MessageList)
		delete pMessage;
}

void CtrlPanel::AddComponent(IComponent &componentRef)
{
	HyLayoutHandle hRow = InsertLayout(HYORIEN_Horizontal);
	InsertWidget(componentRef.GetCtrlPanelCheckBox(), hRow);
	InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);
}

void CtrlPanel::FinishComponents()
{
	HyLayoutHandle hRow = InsertLayout(HYORIEN_Horizontal);
	InsertWidget(m_MessagesLineEdit, hRow);
	InsertWidget(m_AddMessageBtn, hRow);
	InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);

	InsertSpacer();
}

void CtrlPanel::OnRemoveMessage(Message *pMessage)
{
	m_MessageListToRemove.push_back(pMessage);
}

/*virtual*/ void CtrlPanel::OnContainerUpdate() /*override*/
{
	for(auto pMessage : m_MessageListToRemove)
	{
		for(auto iter = m_MessageList.begin(); iter != m_MessageList.end(); ++iter)
		{
			if(*iter == pMessage)
			{
				RemoveLayout(pMessage->m_hLayout);
				m_MessageList.erase(iter);
				delete pMessage;
				break;
			}
		}
	}
	m_MessageListToRemove.clear();
}
