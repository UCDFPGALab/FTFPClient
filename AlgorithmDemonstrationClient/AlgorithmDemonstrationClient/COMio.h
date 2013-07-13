// COMio.h : include file for custom COM io initialization and parameter setting

#pragma once

#define SIZEOFPORT 20

using namespace std;

class COMio
{
public:
	COMio(char port[], int baud, int stop, int bytes, int parity); //try to use given types, like NOPARITY, for the arguments
	~COMio();
	int writeSerialPort(string write);
	int readSerialPort(int timeout); //timeout in seconds
	void getBuff(vector<char> &inBuff);

private:
	HANDLE hSerial;
	vector<char> buff;
	COMMTIMEOUTS timeouts;
};