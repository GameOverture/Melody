#ifndef MessageCycle_h__
#define MessageCycle_h__

#include "pch.h"
#include "IComponent.h"

class MessageCycle : public IComponent
{
	CtrlPanel *					m_pCtrlPanel;

	HyCheckBox					m_CtrlPanel_CheckBox;
	HyLineEdit					m_CtrlPanel_LineEdit;
	HyButton					m_CtrlPanel_AddBtn;

	std::vector<std::string>	m_sMsgList;
	int32						m_iCurrMsgIndex;
	HyTimer						m_Timer;
	
	HyText2d					m_Text;


	struct Message
	{
		HyLabel			m_Message;
		HyButton		m_Remove;
		HyLayoutHandle	m_hLayout;

		Message() :
			m_Message(HyPanelInit(225, 50, 2, HyColor::DarkGray), HyNodePath("", "CtrlPanel"), nullptr),
			m_Remove(HyPanelInit(50, 50, 2, HyColor::Red), HyNodePath("", "MainText"), nullptr),
			m_hLayout(HY_UNUSED_HANDLE)
		{
		}
	};
	std::vector<Message *> m_MessageList;
	std::vector<Message *> m_MessageListToRemove;

public:
	MessageCycle(HyEntity2d *pParent = nullptr);
	virtual ~MessageCycle();

	virtual void PopulateCtrlPanel(CtrlPanel &ctrlPanel) override;

	virtual void Show(float fDuration) override;
	virtual void Hide(float fDuration) override;

	void SetMsgs(std::vector<std::string> &sMsgList, float fCycleDuration);
	void OnRemoveMessage(Message *pMessage);

protected:
	virtual void OnUpdate() override;

	void OnNextMsg();
};

#endif // MessageCycle_h__
