#ifndef NESController_h__
#define NESController_h__

#include "pch.h"
#include "IController.h"

#include "../../lib/NESController/pch.h"

#define NESCONTROLLER_BUFFER_LENGTH 255

class NESController : public IController
{
	HyCheckBox				m_CtrlPanel_CheckBox;
	
	const char *			m_szPORTNAME = "\\\\.\\COM5";
	SerialPort *			m_pArduino;
	char					m_TempReadBuffer[NESCONTROLLER_BUFFER_LENGTH];
	std::vector<uint8>		m_ReadBuffer;

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
