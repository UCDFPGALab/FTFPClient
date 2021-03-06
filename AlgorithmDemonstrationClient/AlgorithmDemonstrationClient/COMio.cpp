// COMio.cpp, implementation file for the COM functions declared in COMio.h
// Some functionality, especially for parity and more complicated stuff, is missing
// TODO: Rewrite the opening and operation of serial ports with try/catch blocks instead of silly error codes

#include "stdafx.h"
#include "COMio.h"

using namespace std;

COMio::COMio(char port[], int baud, int stop, int bytes, int parity)
{
	wchar_t wtext[SIZEOFPORT];
	mbstowcs_s(NULL, wtext, size_t(SIZEOFPORT), port, strlen(port)+1 );//Plus null

	hSerial_ = CreateFile(wtext,
						  GENERIC_READ | GENERIC_WRITE,
						  0,
						  0,
						  OPEN_EXISTING,
						  FILE_ATTRIBUTE_NORMAL,
						  0);

	if(hSerial_==INVALID_HANDLE_VALUE) {
		if(GetLastError()==ERROR_FILE_NOT_FOUND)
			cout << "Serial port does not exist: " << port << endl; //serial port does not exist. Inform user.
		else
			cout << "Serial port \"other\" error: " << port << endl; //some other error occurred. Inform user.
		return;
	}

	//set the parameters now
	DCB dcbSerialParams = {0};
	dcbSerialParams.DCBlength=sizeof(dcbSerialParams);

	if (!GetCommState(hSerial_, &dcbSerialParams)) {
		//error getting state
		cout << "Serial port unreachable, state could not be read: " << port << endl;
		return;
	}
	
	switch (baud) {
		case 110 : 
			dcbSerialParams.BaudRate=CBR_110;
			break;
		case 300 : 
			dcbSerialParams.BaudRate=CBR_300;
			break;
		case 600 : 
			dcbSerialParams.BaudRate=CBR_600;
			break;
		case 1200 : 
			dcbSerialParams.BaudRate=CBR_1200;
			break;
		case 2400 : 
			dcbSerialParams.BaudRate=CBR_2400;
			break;
		case 4800 : 
			dcbSerialParams.BaudRate=CBR_4800;
			break;
		case 9600 : 
			dcbSerialParams.BaudRate=CBR_9600;
			break;
		case 14400 : 
			dcbSerialParams.BaudRate=CBR_14400;
			break;
		case 19200 :
			dcbSerialParams.BaudRate=CBR_19200;
			break;
		case 38400 :
			dcbSerialParams.BaudRate=CBR_38400;
			break;
		case 57600 :
			dcbSerialParams.BaudRate=CBR_57600;
			break;
		case 115200 : 
			dcbSerialParams.BaudRate=CBR_115200;
			break;
		case 128000 : 
			dcbSerialParams.BaudRate=CBR_128000;
			break;
		case 256000 : 
			dcbSerialParams.BaudRate=CBR_256000;
			break;
		default:
			cout << "Incorrect baud rate when initializing port, no such baud rate exists: " << baud << endl;
			return;
	}
	
	if(bytes != 7 && bytes != 8)
		cout << "WARNING: byte payload is not 7 or 8 bytes, what are you doing? (Payload : " << bytes << " )" << endl;

	dcbSerialParams.ByteSize=bytes;

	switch (stop) {
		case 0:
			dcbSerialParams.StopBits=ONESTOPBIT;
			break;
		case 1:
			dcbSerialParams.StopBits=ONE5STOPBITS;
			break;
		case 2:
			dcbSerialParams.StopBits=TWOSTOPBITS;
			break;
		default:
			cout << "No such mode for stop bits allowed: " << stop << endl;
			return;
	}

	switch (parity) {
		case NOPARITY:
			dcbSerialParams.Parity=NOPARITY;
			break;
		default:
			cout << "No such mode for pairty allowed: " << parity << endl;
			return;
	}

	if (!SetCommState(hSerial_, &dcbSerialParams)) {
		//error setting serial port state
		cout << "Serial port unreachable, state could not be set: " << port << endl;
		return;
	}

	//set the timeouts_ now to default values
	timeouts_.ReadIntervalTimeout=50;
	timeouts_.ReadTotalTimeoutConstant=0; //500 msecond wait default
	timeouts_.ReadTotalTimeoutMultiplier=50;
	timeouts_.WriteTotalTimeoutConstant=500;
	timeouts_.WriteTotalTimeoutMultiplier=100;

	if(!SetCommTimeouts(hSerial_, &timeouts_))
		cout << "Serial port timeouts cannot be set" << endl; //error occured. Inform user
}



COMio::~COMio() {
	CloseHandle(hSerial_);
}



int COMio::writeSerialPort(vector<unsigned char> input) {
	//writes the vector to the board bit by bit
	DWORD dwBytesWritten = 0;
	int total = 0;
	int size = sizeof(input[0]);
	string arr;

	for (vector<unsigned char>::iterator it = input.begin(); it < input.end(); it++) {
			arr.push_back(*it);
	}

	if (!WriteFile(hSerial_, arr.c_str(), arr.length(), &dwBytesWritten, NULL)) {
		//error occurred. Report to user.
		cout << "Serial port write error, " << dwBytesWritten << " bytes written" << endl;
		return -1;
	}
	cout << dwBytesWritten << " bytes written to board" << endl;
	return 0;
}

int COMio::readSerialPort(int timeout)
{

	int count = 0;

	timeouts_.ReadTotalTimeoutConstant=timeout; //500 msecond wait default

	if (!SetCommTimeouts(hSerial_, &timeouts_)) {
		//error occured. Inform user
		cout << "Serial port timeouts_ cannot be set (in readSerialPort)" << endl;
		return -1;
	}

    unsigned char cbuf;
    DWORD dwIncomingReadSize;
	count = 0;

	do {
		if (ReadFile(hSerial_, &cbuf, 1, &dwIncomingReadSize, NULL) != 0) {
			if (dwIncomingReadSize > 0) {
				count++;
				buff_.push_back(cbuf);
            }
		}
		else {
			cout << "Serial port unreachable, ReadFile failed (in readSerialPort)" << endl;
		    return -1;
		}
	} while (dwIncomingReadSize > 0);
	cout << count << " bytes read from board" << endl;

	return 0;
}



void COMio::getBuff(vector<unsigned char> &inbuff) {
	buff_.swap(inbuff);
}