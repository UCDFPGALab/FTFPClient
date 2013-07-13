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

		string test = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890$";

		int error = 0;

		while(1)
		{
			vector<char> buff;

			PORT.writeSerialPort(test);
	
			PORT.readSerialPort(atoi(argv[3]));
			PORT.getBuff(buff);

			string str(buff.begin(),buff.end());

			for(int i = 0; i < str.size() || i < test.size(); i++)
			{
				if(str[i] != test[i])
					error++;
			}

			error += strcmp(str.c_str(), test.c_str());

			cout << str << endl;
			cout << "The current error count is: " << error << endl;
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