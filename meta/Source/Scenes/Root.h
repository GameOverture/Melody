#ifndef Root_h__
#define Root_h__

#include "pch.h"
#include "Crt.h"
#include "Message.h"

#include "Intro.h"
#include "LowerThird.h"
#include "Break.h"
#include "Wheel.h"
#include "Outro.h"

class Root : public HyEntity2d
{
	Crt				m_Crt;
	Message			m_Message;

	IScene *		m_pCurrentScene;
	Intro			m_Intro;
	LowerThird		m_LowerThird;
	Break			m_Break;
	Wheel			m_Wheel;
	Outro			m_Outro;

public:
	Root(HyEntity2d *pParent = nullptr);
	virtual ~Root();

	Crt &GetCrt();
	Message &GetMessage();

	Intro GetIntro();
	LowerThird GetLowerThird();
	Break GetBreak();
	Wheel GetWheel();
	Outro GetOutro();

protected:
	virtual void OnUpdate() override;
};

#endif // Root_h__
