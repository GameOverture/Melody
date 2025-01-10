#ifndef Melody_h__
#define Melody_h__

#include "pch.h"
#include "CtrlPanel.h"
#include "InputViewer.h"
#include "VgMusic.h"
#include "Monitor.h"
#include "Wheel.h"
#include "Crt.h"
#include "NowPlaying.h"
#include "HeartBeat.h"
#include "MessageCycle.h"

class IScene;

class Melody : public HyEngine
{
	static Melody *	sm_pThis;

	HyCamera2d *	m_pCamera;
	HyCamera2d *	m_pCameraCtrlPanel;

	HyPrimitive2d	m_ColorKeyBg;

	CtrlPanel		m_CtrlPanel;

	// Components
	VgMusic			m_VgMusic;
	Monitor			m_Monitor;
	MessageCycle	m_MessageCycle;
	InputViewer		m_InputViewer;
	Crt				m_Crt;
	NowPlaying		m_NowPlaying;
	HeartBeat		m_HeartBeat;

	// Debug
	//HyPrimitive2d	m_DebugRetroCaptureArea;

public:
	Melody(HarmonyInit &initStruct);
	virtual ~Melody();

	virtual bool OnUpdate() override;

	static void RefreshCamera();
};

#endif // Melody_h__
