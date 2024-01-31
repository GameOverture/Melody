#ifndef MessageCycle_h__
#define MessageCycle_h__

#include "pch.h"

class MessageCycle : public HyEntity2d
{
	std::string		m_sMain;
	std::string		m_sSub;
	float			m_fMainDuration;
	float			m_fSubDuration;

	HyText2d		m_MainText;

	HyTimer			m_Timer;

public:
	MessageCycle(HyEntity2d *pParent = nullptr);
	virtual ~MessageCycle();

	void SetMsgs(std::string sMain, std::string sSub, float fMainDuration, float fSubDuration);

protected:
	virtual void OnUpdate() override;
};

#endif // MessageCycle_h__
