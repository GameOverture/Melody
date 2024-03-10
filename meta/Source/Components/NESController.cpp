#include "pch.h"
#include "NESController.h"
#include "CtrlPanel.h"

NESController::NESController(HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_NESController, pParent),
	m_CtrlPanel_CheckBox(HyPanelInit(32, 32, 2), HyNodePath("", "CtrlPanel")),
	m_pArduino(nullptr)
{
	m_CtrlPanel_CheckBox.SetText("NES Controller");
	m_CtrlPanel_CheckBox.SetCheckedChangedCallback(
		[this](HyCheckBox *pCheckBox, void *pData)
		{
			if(pCheckBox->IsChecked())
				reinterpret_cast<IComponent *>(pData)->Show(0.5f);
			else
				reinterpret_cast<IComponent *>(pData)->Hide(0.5f);
		}, this);

	memset((void *)m_TempReadBuffer, 0, NESCONTROLLER_BUFFER_LENGTH);
}

/*virtual*/ NESController::~NESController()
{
	delete m_pArduino;
}

/*virtual*/ void NESController::PopulateCtrlPanel(CtrlPanel &ctrlPanel) /*override*/
{
	HyLayoutHandle hRow = ctrlPanel.InsertLayout(HYORIEN_Horizontal);
	ctrlPanel.InsertWidget(m_CtrlPanel_CheckBox, hRow);
	ctrlPanel.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);
}

/*virtual*/ void NESController::Show(float fDuration)
{
	IComponent::Show(fDuration);
	Connect();
}

/*virtual*/ void NESController::Hide(float fDuration)
{
	IComponent::Hide(fDuration);
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
