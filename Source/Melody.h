#ifndef Melody_h__
#define Melody_h__

#include "pch.h"

class Melody : public HyEngine
{
	HyCamera2d *	m_pCamera;

	enum InputAction
	{
		INPUT_ExitGame = 0,
		
		INPUT_CameraUp,
		INPUT_CameraLeft,
		INPUT_CameraDown,
		INPUT_CameraRight
	};

public:
	Melody(HyInit &initStruct);
	virtual ~Melody();
	
	static Melody *Get();

	virtual bool OnUpdate() override;
};

#endif // Melody_h__
