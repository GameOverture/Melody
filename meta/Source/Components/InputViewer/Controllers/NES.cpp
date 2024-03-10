#include "pch.h"
#include "NES.h"
#include "CtrlPanel.h"

NESController::NESController(HyEntity2d *pParent /*= nullptr*/) :
	IController(pParent),
	m_pArduino(nullptr),
	m_Dpad(HyNodePath("InputViewer/NES/DpadGate"), HyNodePath("InputViewer/DpadBall"), HyNodePath("InputViewer/NES/Buttons"), this),
	m_Btns{ HySprite2d(HyNodePath("InputViewer/NES/Buttons"), this), HySprite2d(HyNodePath("InputViewer/NES/Buttons"), this), HySprite2d(HyNodePath("InputViewer/NES/Buttons"), this), HySprite2d(HyNodePath("InputViewer/NES/Buttons"), this) }
{
	memset((void *)m_TempReadBuffer, 0, NESCONTROLLER_BUFFER_LENGTH);

	m_Btns[BUTTON_B].pos.Set(198.0f, -12.0f);
	m_Btns[BUTTON_A].pos.Set(264.0f, -12.0f);
	m_Btns[BUTTON_SELECT].pos.Set(78.0f, -25.0f);
	m_Btns[BUTTON_START].pos.Set(138.0f, -25.0f);

	for(int i = 0; i < NUM_NES_BUTTONS; ++i)
	{
		m_Btns[i].SetState(i);
	}

	ResetDisplayOrder();
}

/*virtual*/ NESController::~NESController()
{
	delete m_pArduino;
}

/*virtual*/ void NESController::ApplyInputs() /*override*/
{
}

void NESController::Connect()
{
	delete m_pArduino;
	m_pArduino = new SerialPort(m_szPORTNAME);
}

/*virtual*/ void NESController::OnUpdate() /*override*/
{
	if(m_pArduino == nullptr)
		return;

	if(m_pArduino->isConnected() == false)
	{
		HyLog("NESController - Arduino connection lost...");
		
		Connect();
		//Checking if pArduino is connected or not
		if(m_pArduino->isConnected())
			HyLog("Connection established at port " << m_szPORTNAME);
	}
	else // Is connected
	{
		int iReadBytes = m_pArduino->readSerialPort(m_TempReadBuffer, NESCONTROLLER_BUFFER_LENGTH);
		if(iReadBytes != 0)
		{
			for(int i = 0; i < iReadBytes; ++i)
				m_ReadBuffer.push_back(m_TempReadBuffer[i]);

			auto start = m_ReadBuffer.begin();
			auto end = m_ReadBuffer.begin();

			while(end != m_ReadBuffer.end())
			{
				end = std::find(start, m_ReadBuffer.end(), 0x0A); // Find delimiter

				if(end != m_ReadBuffer.end())
				{
					PacketRecv(std::vector<uint8_t>(start, end)); // Process packet
					start = end + 1; // Move start to next byte after delimiter
				}
			}

			// Remove extracted bytes from m_ReadBuffer
			m_ReadBuffer.erase(m_ReadBuffer.begin(), start);
		}
	}
}

void NESController::PacketRecv(std::vector<uint8_t> packet)
{
	std::string sPacket;
	for(uint8_t byte : packet)
		sPacket += std::to_string(byte);

	HyLog("PACKET: " << sPacket);
}
