#ifndef Melody_h__
#define Melody_h__

#include "pch.h"
#include "CtrlPanel.h"
#include "FightStick.h"
#include "VgMusic.h"
#include "Wheel.h"
#include "Crt.h"
#include "Brb.h"
#include "HeartBeat.h"
#include "MessageCycle.h"

class IScene;

class Melody : public HyEngine
{
	HyCamera2d *	m_pCamera;
	HyCamera2d *	m_pCameraCtrlPanel;

	HyPrimitive2d	m_ColorKeyBg;

	HyPrimitive2d	m_AboveLiveSplit;

	FightStick		m_FightStick;
	VgMusic			m_VgMusic;
	//Wheel			m_Wheel;
	Crt				m_Crt;
	Brb				m_Brb;
	HeartBeat		m_HeartBeat;
	MessageCycle	m_MessageCycle;

	CtrlPanel		m_CtrlPanel;

public:
	Melody(HarmonyInit &initStruct);
	virtual ~Melody();

	virtual bool OnUpdate() override;
};

#endif // Melody_h__
