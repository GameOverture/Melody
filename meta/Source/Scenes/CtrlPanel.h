#ifndef CtrlPanel_h__
#define CtrlPanel_h__

#include "pch.h"

class IComponent;

class CtrlPanel : public HyUiContainer
{
	HyLineEdit			m_MessagesLineEdit;
	HyButton			m_AddMessageBtn;

	struct Message
	{
		CtrlPanel &		m_CtrlPanelRef;
		HyLabel			m_Message;
		HyButton		m_Remove;
		HyLayoutHandle	m_hLayout;

		Message(CtrlPanel &ctrlPanelRef, std::string sText) :
			m_CtrlPanelRef(ctrlPanelRef),
			m_Message(HyPanelInit(350, 50, 2, HyColor::DarkGray), HyNodePath("", "CtrlPanel"), nullptr),
			m_Remove(HyPanelInit(50, 50, 2, HyColor::Red), HyNodePath("", "CtrlPanel"), nullptr)
		{
			m_Message.SetText(sText);
			m_Remove.SetText("X");
			m_Remove.SetButtonClickedCallback([this](HyButton *pThis, void *pData)
			{
				m_CtrlPanelRef.OnRemoveMessage(this);
			});

			m_hLayout = m_CtrlPanelRef.InsertLayout(HYORIEN_Horizontal);
			m_CtrlPanelRef.InsertWidget(m_Message, m_hLayout);
			m_CtrlPanelRef.InsertWidget(m_Remove, m_hLayout);
			m_CtrlPanelRef.InsertSpacer(HYSIZEPOLICY_Expanding, 0, m_hLayout);

			m_Message.Load();
			m_Remove.Load();
		}
	};
	std::vector<Message *> m_MessageList;
	std::vector<Message *> m_MessageListToRemove;

public:
	CtrlPanel(HyEntity2d *pParent = nullptr);
	virtual ~CtrlPanel();

	void AddComponent(IComponent &componentRef);
	void FinishComponents();

	void OnRemoveMessage(Message *pMessage);

protected:
	virtual void OnContainerUpdate() override;
};

#endif // CtrlPanel_h__
