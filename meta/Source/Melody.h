#ifndef Melody_h__
#define Melody_h__

#include "pch.h"
#include "Root.h"
#include "FightStick.h"

class IScene;

class Melody : public HyEngine
{
	HyCamera2d *	m_pCamera;

	HyPrimitive2d	m_ColorKeyBg;
	FightStick		m_FightStick;

public:
	Melody(HarmonyInit &initStruct);
	virtual ~Melody();

	virtual bool OnUpdate() override;
};

#endif // Melody_h__
