#ifndef MessageCycle_h__
#define MessageCycle_h__

#include "pch.h"
#include "IComponent.h"

#define MSGCYCLE_HIDE_YPOS		-100.0f
#define MSGCYCLE_SHOW_YPOS		50.0f

#define MSGCYCLE_DURATION_LONG	20.0f
#define MSGCYCLE_DURATION_SHORT	5.0f

class Monitor;

class MessageCycle : public IComponent
{
	CtrlPanel *					m_pCtrlPanel;

	float						m_fXPos;

	HyTextField					m_CtrlPanel_LineEdit;
	HyButton					m_CtrlPanel_AddBtn;

	HyRadioButton				m_CtrlPanel_radLong;
	HyRadioButton				m_CtrlPanel_radShort;
	HyCheckBox					m_CtrlPanel_GrayBg;

	HyPrimitive2d				m_GrayBg;

	int32						m_iCurrMsgIndex;
	HyTimer						m_Timer;
	
	HyText2d					m_Text;

	struct Message
	{
		float			m_fDuration;
		HyLabel			m_Message;
		HyButton		m_Remove;
		HyLayoutHandle	m_hLayout;

		Message(bool bLongDur) :
			m_fDuration(bLongDur ? MSGCYCLE_DURATION_LONG : MSGCYCLE_DURATION_SHORT),
			m_Message(HyUiPanelInit(225, 50, 2, bLongDur ? HyColor::DarkGray : HyColor::DarkGreen), HyNodePath("", "CtrlPanel"), nullptr),
			m_Remove(HyUiPanelInit(50, 50, 2, HyColor::Red), HyNodePath("", "MainText"), nullptr),
			m_hLayout(HY_UNUSED_HANDLE)
		{
		}

		~Message()
		{
			m_Message.DisableMouseInput();
			m_Remove.DisableMouseInput();
		}

		std::string GetMsg() const { return m_Message.GetUtf8String(); }
		float GetDuration() const { return m_fDuration; }
	};
	std::vector<Message *> m_MessageList;
	std::vector<Message *> m_DeleteList;

public:
	MessageCycle(HyEntity2d *pParent = nullptr);
	virtual ~MessageCycle();

	void SetXPosOffset(float fXPos);

	void AddMessage(const std::string &sMessage, bool bLongDur);
	void RemoveMessage(const std::string &sMessage);
	void ClearMessages();

	virtual void PopulateCtrlPanel(CtrlPanel &ctrlPanel) override;

	virtual void Show(float fDuration) override;
	virtual void Hide(float fDuration) override;

	void OnRemoveMessage(Message *pMessage);

protected:
	virtual void OnUpdate() override;

	void OnNextMsg();
};

#endif // MessageCycle_h__
