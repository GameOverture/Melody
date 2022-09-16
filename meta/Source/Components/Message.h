#ifndef Message_h__
#define Message_h__

#include "pch.h"

class Message : public HyEntity2d
{
	std::string		m_sMain;
	std::string		m_sSub;
	float			m_fMainDuration;
	float			m_fSubDuration;

	HyText2d		m_MainText;

	HyTimer			m_Timer;

public:
	Message(HyEntity2d *pParent = nullptr);
	virtual ~Message();

	void SetMsgs(std::string sMain, std::string sSub, float fMainDuration, float fSubDuration);

protected:
	virtual void OnUpdate() override;
};

#endif // Message_h__
