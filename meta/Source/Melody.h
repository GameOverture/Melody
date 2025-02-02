#ifndef Melody_h__
#define Melody_h__

#include "pch.h"
#include "CtrlPanel.h"
#include "Compositorium.h"
#include "InputViewer.h"
#include "VgMusic.h"
#include "GameBrowser.h"
#include "Monitor.h"
#include "LiveSplit.h"
#include "Wheel.h"
#include "Crt.h"
#include "NowPlaying.h"
#include "Docket.h"
#include "HeartBeat.h"
#include "MessageCycle.h"

class IScene;

class Melody : public HyEngine
{
	static Melody *	sm_pThis;

	HyCamera2d *	m_pCamera;
	HyCamera2d *	m_pCameraCtrlPanel;

	Compositorium	m_Compositorium;

	HyPrimitive2d	m_ColorKeyBg;

	CtrlPanel		m_CtrlPanel;

	// Components
	VgMusic			m_VgMusic;
	GameBrowser		m_GameBrowser;
	Monitor			m_Monitor;
	LiveSplit		m_LiveSplit;
	MessageCycle	m_MessageCycle;
	InputViewer		m_InputViewer;
	Crt				m_Crt;
	NowPlaying		m_NowPlaying;
	Docket			m_Docket;
	HeartBeat		m_HeartBeat;

	// Presets
	HyButton		m_PresetStartingBtn;
	HyButton		m_PresetLiveBtn;
	HyButton		m_PresetBrbOnBtn;
	HyButton		m_PresetBrbOffBtn;
	HyButton		m_PresetEndingBtn;

	// Debug
	//HyPrimitive2d	m_DebugRetroCaptureArea;

public:
	Melody(HarmonyInit &initStruct);
	virtual ~Melody();

	virtual bool OnUpdate() override;

	static void RefreshCamera();
};

void TransformTexture(HyTexturedQuad2d &quadRef, glm::ivec2 vMaxSize, glm::vec2 ptCenter);

#endif // Melody_h__
