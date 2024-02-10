#ifndef MessageCycle_h__
#define MessageCycle_h__

#include "pch.h"
#include "IComponent.h"

class MessageCycle : public IComponent
{
	std::vector<std::string>	m_sMsgList;
	int32						m_iCurrMsgIndex;
	HyTimer						m_Timer;
	
	HyText2d					m_Text;

public:
	MessageCycle(HyEntity2d *pParent = nullptr);
	virtual ~MessageCycle();

	virtual void Show(float fDuration) override;
	virtual void Hide(float fDuration) override;

	void SetMsgs(std::vector<std::string> &sMsgList, float fCycleDuration);

protected:
	virtual void OnUpdate() override;

	void OnNextMsg();
};

#endif // MessageCycle_h__
