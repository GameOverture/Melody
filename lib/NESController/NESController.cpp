// NESController.cpp : Defines the functions for the static library.
//

#include "pch.h"

char *portName = "\\\\.\\COM20";

#define MAX_DATA_LENGTH 255

char incomingData[MAX_DATA_LENGTH];

//Control signals for turning on and turning off the led
//Check arduino code
char ledON[] = "ON\n";
char ledOFF[] = "OFF\n";

//Arduino SerialPort object
SerialPort *pArduino = nullptr;

//Blinking Delay
const unsigned int BLINKING_DELAY = 1000;

//If you want to send data then define "SEND" else comment it out
//#define SEND

void exampleReceiveData(void)
{
	int readResult = pArduino->readSerialPort(incomingData, MAX_DATA_LENGTH);
	printf("%s", incomingData);
	Sleep(10);
}

void exampleWriteData(unsigned int delayTime)
{
	pArduino->writeSerialPort(ledON, MAX_DATA_LENGTH);
	Sleep(delayTime);
	pArduino->writeSerialPort(ledOFF, MAX_DATA_LENGTH);
	Sleep(delayTime);
}

void autoConnect(void)
{
	//better than recusion
	//avoid stack overflows
	while(1) {
		// ui - searching
		std::cout << "Searching in progress";
		// wait connection
		while(!pArduino->isConnected()) {
			Sleep(100);
			std::cout << ".";
			pArduino = new SerialPort(portName);
		}

		//Checking if pArduino is connected or not
		if(pArduino->isConnected()) {
			std::cout << std::endl << "Connection established at port " << portName << std::endl;
		}

#ifdef SEND
		while(pArduino->isConnected()) exampleWriteData(BLINKING_DELAY);
#else // SEND
		while(pArduino->isConnected()) exampleReceiveData();
#endif // SEND
	}
}

void StartNESController()
{
	pArduino = new SerialPort(portName);

	autoConnect();

	delete pArduino;
}
