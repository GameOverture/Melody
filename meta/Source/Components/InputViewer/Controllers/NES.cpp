#include "pch.h"
#include "NES.h"
#include "CtrlPanel.h"

NESController::NESController(HyEntity2d *pParent /*= nullptr*/) :
	IController(pParent),
	m_pArduino(nullptr),
	m_uiDpadFlags(0),
	m_uiBtnFlags(0),
	m_Dpad(HyNodePath("InputViewer/NES/DpadGate"), HyNodePath("InputViewer/NES/DpadBall"), HyNodePath("InputViewer/NES/Buttons"), 38.0f, 32.0f, HyColor::LightGray, 8.0f, this),
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
		m_Btns[i].SetTint(HyColor(128, 128, 128));
		m_Btns[i].SetDisplayOrder(DISPLAYORDER_Buttons);
	}
}

/*virtual*/ NESController::~NESController()
{
	delete m_pArduino;
}

/*virtual*/ void NESController::ApplyInputs() /*override*/
{
	m_Dpad.ApplyInput(m_uiDpadFlags);

	uint32 uiCurBtnFlags = 0;
	for(int i = 0; i < NUM_NES_BUTTONS; ++i)
		uiCurBtnFlags |= (m_Btns[i].GetTag() << i);

	if(uiCurBtnFlags != m_uiBtnFlags)
	{
		const float fButtonPressScale = 1.15f;

		if(!(uiCurBtnFlags & BTNFLAG_B) && m_uiBtnFlags & BTNFLAG_B)
		{
			m_Btns[BUTTON_B].SetTint(HyColor::White);
			m_Btns[BUTTON_B].scale.Tween(fButtonPressScale, fButtonPressScale, 0.1f, HyTween::QuadOut);
			m_Btns[BUTTON_B].SetTag(1);
			m_Dpad.SetButtonPress(BUTTON_B, true);
		}
		else if(uiCurBtnFlags & BTNFLAG_B && !(m_uiBtnFlags & BTNFLAG_B))
		{
			m_Btns[BUTTON_B].SetTint(HyColor(128, 128, 128));
			m_Btns[BUTTON_B].scale.Tween(1.0f, 1.0f, 0.1f, HyTween::QuadOut);
			m_Btns[BUTTON_B].SetTag(0);
			m_Dpad.SetButtonPress(BUTTON_B, false);
		}

		if(!(uiCurBtnFlags & BTNFLAG_A) && m_uiBtnFlags & BTNFLAG_A)
		{
			m_Btns[BUTTON_A].SetTint(HyColor::White);
			m_Btns[BUTTON_A].scale.Tween(fButtonPressScale, fButtonPressScale, 0.1f, HyTween::QuadOut);
			m_Btns[BUTTON_A].SetTag(1);
			m_Dpad.SetButtonPress(BUTTON_A, true);
		}
		else if(uiCurBtnFlags & BTNFLAG_A && !(m_uiBtnFlags & BTNFLAG_A))
		{
			m_Btns[BUTTON_A].SetTint(HyColor(128, 128, 128));
			m_Btns[BUTTON_A].scale.Tween(1.0f, 1.0f, 0.1f, HyTween::QuadOut);
			m_Btns[BUTTON_A].SetTag(0);
			m_Dpad.SetButtonPress(BUTTON_A, false);
		}

		if(!(uiCurBtnFlags & BTNFLAG_START) && m_uiBtnFlags & BTNFLAG_START)
		{
			m_Btns[BUTTON_START].SetTint(HyColor::White);
			m_Btns[BUTTON_START].scale.Tween(fButtonPressScale, fButtonPressScale, 0.1f, HyTween::QuadOut);
			m_Btns[BUTTON_START].SetTag(1);
			m_Dpad.SetButtonPress(BUTTON_START, true);
		}
		else if(uiCurBtnFlags & BTNFLAG_START && !(m_uiBtnFlags & BTNFLAG_START))
		{
			m_Btns[BUTTON_START].SetTint(HyColor(128, 128, 128));
			m_Btns[BUTTON_START].scale.Tween(1.0f, 1.0f, 0.1f, HyTween::QuadOut);
			m_Btns[BUTTON_START].SetTag(0);
			m_Dpad.SetButtonPress(BUTTON_START, false);
		}

		if(!(uiCurBtnFlags & BTNFLAG_SELECT) && m_uiBtnFlags & BTNFLAG_SELECT)
		{
			m_Btns[BUTTON_SELECT].SetTint(HyColor::White);
			m_Btns[BUTTON_SELECT].scale.Tween(fButtonPressScale, fButtonPressScale, 0.1f, HyTween::QuadOut);
			m_Btns[BUTTON_SELECT].SetTag(1);
			m_Dpad.SetButtonPress(BUTTON_SELECT, true);
		}
		else if(uiCurBtnFlags & BTNFLAG_SELECT && !(m_uiBtnFlags & BTNFLAG_SELECT))
		{
			m_Btns[BUTTON_SELECT].SetTint(HyColor(128, 128, 128));
			m_Btns[BUTTON_SELECT].scale.Tween(1.0f, 1.0f, 0.1f, HyTween::QuadOut);
			m_Btns[BUTTON_SELECT].SetTag(0);
			m_Dpad.SetButtonPress(BUTTON_SELECT, false);
		}
	}
}

void NESController::Connect()
{
	if(m_pArduino && m_pArduino->isConnected())
		return;

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
	if(packet.size() < 8)
		return;

	m_uiBtnFlags = 0;
	for(int i = 0; i < 8; ++i)
	{
		if(packet[i] != 0x00)
			m_uiBtnFlags |= (1 << i);
	}

	m_uiDpadFlags = (m_uiBtnFlags & BTNFLAG_DPAD_MASK) >> 4;

	//// Write out the packet in binary
	//std::string sBinary;
	//for(uint8_t byte : packet)
	//{
	//	for(int i = 7; i >= 0; --i)
	//		sBinary += (byte & (1 << i)) ? "1" : "0";
	//	sBinary += " ";
	//}
	//HyLog(sBinary);

	//m_uiDpadFlags = packet[1];
	// m_uiBtnFlags
	//m_uiBtnFlags[BUTTON_B] = (packet[0] & 0x01) ? true : false;
	//m_bBtnStates[BUTTON_A] = (packet[0] & 0x02) ? true : false;
	//m_bBtnStates[BUTTON_SELECT] = (packet[0] & 0x04) ? true : false;
	//m_bBtnStates[BUTTON_START] = (packet[0] & 0x08) ? true : false;
}
