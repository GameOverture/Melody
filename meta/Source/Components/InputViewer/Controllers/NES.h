#ifndef NESController_h__
#define NESController_h__

#include "pch.h"
#include "IController.h"
#include "Dpad.h"

#include "../../lib/NESController/pch.h"

#define NESCONTROLLER_BUFFER_LENGTH 255

class NESController : public IController
{
	enum ButtonState
	{
		BUTTON_A = 0,
		BUTTON_B,
		BUTTON_SELECT,
		BUTTON_START,

		NUM_NES_BUTTONS
	};
	enum ButtonFlag
	{
		BTNFLAG_A = 1 << 0,
		BTNFLAG_B = 1 << 1,
		BTNFLAG_SELECT = 1 << 2,
		BTNFLAG_START = 1 << 3,
		BTNFLAG_UP = 1 << 4,
		BTNFLAG_DOWN = 1 << 5,
		BTNFLAG_LEFT = 1 << 6,
		BTNFLAG_RIGHT = 1 << 7,

		BTNFLAG_DPAD_MASK = (BTNFLAG_UP | BTNFLAG_DOWN | BTNFLAG_LEFT | BTNFLAG_RIGHT)
	};

	// USB Serial
	const char *			m_szPORTNAME = "\\\\.\\COM5";
	SerialPort *			m_pArduino;
	char					m_TempReadBuffer[NESCONTROLLER_BUFFER_LENGTH];
	std::vector<uint8>		m_ReadBuffer;
	float					m_fElapsedTime;

	// Cached input state
	uint32					m_uiDpadFlags;
	uint32					m_uiBtnFlags;

	// Display
	Dpad					m_Dpad;
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
