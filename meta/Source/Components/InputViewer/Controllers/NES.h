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
		BUTTON_B = 0,
		BUTTON_A,
		BUTTON_START,
		BUTTON_SELECT,

		NUM_NES_BUTTONS
	};
	enum ButtonFlag
	{
		BTNFLAG_B = 0x01,
		BTNFLAG_A = 0x02,
		BTNFLAG_START = 0x04,
		BTNFLAG_SELECT = 0x08,
	};

	// USB Serial
	const char *			m_szPORTNAME = "\\\\.\\COM5";
	SerialPort *			m_pArduino;
	char					m_TempReadBuffer[NESCONTROLLER_BUFFER_LENGTH];
	std::vector<uint8>		m_ReadBuffer;

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
