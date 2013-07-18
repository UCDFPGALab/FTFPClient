// AlgorithmDemonstrationClient.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "COMio.h"

using namespace std;

int main(int argc, char* argv[])
{
	if(argc != 4)
	{
		cout << "\nProgram usage:\nAlgorithmDemonstrationClient.exe [Port Name] [Input File] [Timeout (msec)]" << endl;
	}
	else //main body of program
	{
		COMio PORT(argv[1], 115200, ONESTOPBIT, 8, NOPARITY);

		char c[17];
		for(int i = 0; i < 16; i++)
		{
			c[i] = 5+i;
		}
		c[16] = NULL;

		cout << "The string test is this long: " << strlen(c) << endl;
		

		int error = 0;

		while(1)
		{
			vector<int> buff;

			PORT.writeSerialPort(c);
	
			PORT.readSerialPort(atoi(argv[3]));
			PORT.getBuff(buff);

			cout << endl;

			cout << "The input is:  ";
			for (int i = 0; i < strlen(c); i++)
				cout << (unsigned)c[i] << " ";
			cout << endl;
			cout << "The return is: ";
			for (vector<int>::iterator it = buff.begin(); it < buff.end(); it++)
				cout << *it << " ";

			cout << endl;
		}

/*		fstream input;
		input.open(argv[2], fstream::in);
		string c;

		while(input >> skipws >> c)
		{
			PORT.writeSerialPort(c);
		}

		PORT.readSerialPort(atoi(argv[3]));
		PORT.getBuff(buff);

		fstream output;
		output.open("output.txt", fstream::out);
		int i = 0;

		for( vector<char>::const_iterator it = buff.begin(); it != buff.end(); ++it)
		{
			output << *it;
		}

		*/
		
	}

	//cout << "Press a key to continue..." << endl;
	//cin.get(); //pause until input
	return 0;
}