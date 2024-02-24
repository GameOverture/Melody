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

	HyPrimitive2d	m_RetroLeftSide;
	HyStencil		m_RetroLeftSideStencil;
	//HyPrimitive2d	m_RetroCaptureArea;

	CtrlPanel		m_CtrlPanel;

	VgMusic			m_VgMusic;
	Brb				m_Brb;
	MessageCycle	m_MessageCycle;
	Crt				m_Crt;
	FightStick		m_FightStick;
	HeartBeat		m_HeartBeat;

public:
	Melody(HarmonyInit &initStruct);
	virtual ~Melody();

	virtual bool OnUpdate() override;
};

#endif // Melody_h__
