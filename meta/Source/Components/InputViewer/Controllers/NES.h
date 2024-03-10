#ifndef NESController_h__
#define NESController_h__

#include "pch.h"
#include "IController.h"
#include "Dpad.h"

#include "../../lib/NESController/pch.h"

#define NESCONTROLLER_BUFFER_LENGTH 255

class NESController : public IController
{	
	const char *			m_szPORTNAME = "\\\\.\\COM5";
	SerialPort *			m_pArduino;
	char					m_TempReadBuffer[NESCONTROLLER_BUFFER_LENGTH];
	std::vector<uint8>		m_ReadBuffer;

	Dpad					m_Dpad;
	enum NES_BUTTONS
	{
		BUTTON_B = 0,
		BUTTON_A,
		BUTTON_START,
		BUTTON_SELECT,

		NUM_NES_BUTTONS
	};
	HySprite2d				m_Btns[NUM_NES_BUTTONS];

public:
	NESController(HyEntity2d *pParent = nullptr);
	virtual ~NESController();

	virtual void ApplyInputs() override;

	void Connect();

protected:
	virtual void OnUpdate() override;

	void PacketRecv(std::vector<uint8_t> packet);
};

#endif // NESController_h__
