#ifndef Melody_h__
#define Melody_h__

#include "pch.h"
#include "Root.h"
#include "ControlPanel.h"

class IScene;

class Melody : public HyEngine
{
	HyCamera2d *	m_pCamera;

	ControlPanel	m_ControlPanel;
	Root			m_Root;

public:
	Melody(HarmonyInit &initStruct);
	virtual ~Melody();

	virtual bool OnUpdate() override;
};

#endif // Melody_h__
