// COMio.h : include file for custom COM io initialization and parameter setting

#pragma once

#define SIZEOFPORT 20

using namespace std;

/* TO-DO 
restructure the COMio class so that there is a COMio-incl.h that contains the implementations of the functions, and add it as an include to the bottom of this file
*/

class COMio
{
public:
	COMio(char port[], int baud, int stop, int bytes, int parity); //try to use given types, like NOPARITY, for the arguments
	~COMio();
	int writeSerialPort(vector<unsigned char> write);
	int readSerialPort(int timeout); //timeout in seconds
	void getBuff(vector<unsigned char> &inBuff);

private:
	HANDLE hSerial;
	vector<unsigned char> buff;
	COMMTIMEOUTS timeouts;
};