#ifndef Melody_h__
#define Melody_h__

#include "pch.h"
#include "CtrlPanel.h"
#include "FightStick.h"
#include "VgMusic.h"

class IScene;

class Melody : public HyEngine
{
	HyCamera2d *	m_pCamera;
	HyCamera2d *	m_pCameraCtrlPanel;

	HyPrimitive2d	m_ColorKeyBg;

	FightStick		m_FightStick;
	VgMusic			m_VgMusic;

public:
	Melody(HarmonyInit &initStruct);
	virtual ~Melody();

	virtual bool OnUpdate() override;
};

#endif // Melody_h__
