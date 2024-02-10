#include "pch.h"
#include "CtrlPanel.h"
#include "IComponent.h"
#include "VgMusic.h"
#include "Crt.h"
#include "MessageCycle.h"

CtrlPanel::CtrlPanel(VgMusic &vgMusicRef, Crt &crtRef, MessageCycle &msgCycle, HyEntity2d *pParent /*= nullptr*/) :
	HyUiContainer(HYORIEN_Vertical, HyPanelInit(), pParent),
	m_VgMusicRef(vgMusicRef),
	m_CrtRef(crtRef),
	m_MsgCycle(msgCycle),
	m_btnMusic_Prev(HyPanelInit(120, 50, 2), HyNodePath("", "CtrlPanel"), this),
	m_btnMusic_Play(HyPanelInit(120, 50, 2), HyNodePath("", "CtrlPanel"), this),
	m_btnMusic_Stop(HyPanelInit(120, 50, 2), HyNodePath("", "CtrlPanel"), this),
	m_btnVolume_Down(HyPanelInit(120, 50, 2), HyNodePath("", "CtrlPanel"), this),
	m_btnVolume_Up(HyPanelInit(120, 50, 2), HyNodePath("", "CtrlPanel"), this),
	m_MessagesLineEdit(HyPanelInit(350, 50, 2, HyColor::Blue), HyNodePath("", "CtrlPanel"), this),
	m_AddMessageBtn(HyPanelInit(50, 50, 2, HyColor::Green), HyNodePath("", "CtrlPanel"), this)
{
	m_btnMusic_Prev.SetText("PREV");
	m_btnMusic_Prev.SetButtonClickedCallback([this](HyButton *pThis, void *pData)
	{
		m_VgMusicRef.Prev();
	});

	m_btnMusic_Play.SetText("PLAY");
	m_btnMusic_Play.SetButtonClickedCallback([this](HyButton *pThis, void *pData)
	{
		m_VgMusicRef.Play();
	});

	m_btnMusic_Stop.SetText("STOP");
	m_btnMusic_Stop.SetButtonClickedCallback([this](HyButton *pThis, void *pData)
	{
		m_VgMusicRef.Stop();
	});

	m_btnVolume_Down.SetText("VOL -");
	m_btnVolume_Down.SetButtonClickedCallback([this](HyButton *pThis, void *pData)
	{
		HyEngine::Audio().SetGlobalVolume(HyMath::Clamp(HyEngine::Audio().GetGlobalVolume() - 0.05f, 0.0f, 1.0f));
		m_CrtRef.SetVolume(HyEngine::Audio().GetGlobalVolume());
	});

	m_btnVolume_Up.SetText("VOL +");
	m_btnVolume_Up.SetButtonClickedCallback([this](HyButton *pThis, void *pData)
	{
		HyEngine::Audio().SetGlobalVolume(HyMath::Clamp(HyEngine::Audio().GetGlobalVolume() + 0.05f, 0.0f, 1.0f));
		m_CrtRef.SetVolume(HyEngine::Audio().GetGlobalVolume());
	});

	m_AddMessageBtn.SetText("ADD");
	m_AddMessageBtn.SetButtonClickedCallback([this](HyButton *pThis, void *pData)
	{
		Message *pMessage = HY_NEW Message(*this, m_MessagesLineEdit.GetUtf8String());
		m_MessageList.push_back(pMessage);

		std::vector<std::string> msgList;
		for(auto msg : m_MessageList)
			msgList.push_back(msg->m_Message.GetUtf8String());

		m_MsgCycle.SetMsgs(msgList, 5.0f);
	});
	SetSize(420, 1000);

	EnableScrollBars(true, false);
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
	HyLayoutHandle hCurRow = InsertLayout(HYORIEN_Horizontal);
	InsertWidget(m_btnMusic_Prev, hCurRow);
	InsertWidget(m_btnMusic_Play, hCurRow);
	InsertWidget(m_btnMusic_Stop, hCurRow);
	
	hCurRow = InsertLayout(HYORIEN_Horizontal);
	InsertWidget(m_btnVolume_Down, hCurRow);
	InsertWidget(m_btnVolume_Up, hCurRow);

	hCurRow = InsertLayout(HYORIEN_Horizontal);
	InsertWidget(m_MessagesLineEdit, hCurRow);
	InsertWidget(m_AddMessageBtn, hCurRow);
	InsertSpacer(HYSIZEPOLICY_Expanding, 0, hCurRow);

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
