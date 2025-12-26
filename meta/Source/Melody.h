#ifndef Melody_h__
#define Melody_h__

#include "pch.h"
#include "Compositorium.h"
#include "CtrlPanel.h"

class IComponent;

class Melody : public HyEngine
{
	static Melody *	sm_pThis;

	HyCamera2d *	m_pCamera;
	HyCamera2d *	m_pCameraCtrlPanel;

	Compositorium	m_Compositorium;
	HyPrimitive2d	m_ColorKeyBg;

	CtrlPanel		m_CtrlPanel;
	IComponent *	m_pComponents[NUM_COMPONENTS];

	// Presets
	HyButton		m_PresetStartingBtn;
	HyButton		m_PresetLiveBtn;
	HyButton		m_PresetBrb1Btn;
	HyButton		m_PresetBrb5Btn;
	HyButton		m_PresetBrb10Btn;
	HyButton		m_PresetBrbOffBtn;
	HyButton		m_PresetEndingBtn;

	// Debug
	//HyPrimitive2d	m_DebugRetroCaptureArea;

public:
	Melody(HyInit &initStruct);
	virtual ~Melody();

	virtual bool OnUpdate() override;

	static IComponent *GetComponent(ComponentType eType);
	static void RefreshCamera();

protected:
	void StartBrb(int iBrbTime);
	void ClearBrb();
};

void TransformTexture(HyTexturedQuad2d &quadRef, glm::ivec2 vMaxSize, glm::vec2 ptCenter);

#endif // Melody_h__
