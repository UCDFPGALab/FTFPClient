// AlgorithmDemonstrationClient.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "COMio.h" //restructure the COMio

const int TOWRITE = 16;
const int ROWS = 4;
const int COLUMNS = 4;
const int matrices = 3;

using namespace std;

void read(vector<unsigned char> &vec, char* fname)
{
		fstream fs;
		fs.open(fname, fstream::in);
		int i;
	
		while(fs >> i)
		{
			vec.push_back((unsigned char)i);
		}
}

void matrix(unsigned char (&mat)[ROWS][COLUMNS], vector<unsigned char> &vec, int matrix)
{
	for(int i = 0; i < ROWS; i++)
	{
		for(int j = 0; j < COLUMNS; j++)
		{
			mat[i][j] = vec.at((matrix-1)*(ROWS*COLUMNS) + (i*COLUMNS + j));
		}
	}
}

void alg(vector<unsigned char> &vec)
{
	fstream fs;
	fs.open("correct_output.txt", fstream::out);

	bool nw, n, ne, w, e, sw, s, se;
	int sum;

	unsigned char mat[ROWS][COLUMNS];
	
	for(int j = 0; j < matrices; j++)
	{
		matrix(mat, vec, j+1);
	
		for(int row = 0; row < ROWS; row++)
		{
			for(int column = 0; column < COLUMNS; column++)
			{
				nw = (column != 0 && row != 0);
				n = (row != 0);
				ne = (column != COLUMNS-1 && row != 0);
				w = (column != 0);
				e = (column != COLUMNS-1);
				sw = (row != ROWS-1 && column != 0);
				s = (row != ROWS-1);
				se = (row != ROWS-1 && column != COLUMNS-1);

				sum = mat[row][column];
				
				if(nw)	sum += mat[row-1][column-1];
				if(n)	sum += mat[row-1][column];
				if(ne)	sum += mat[row-1][column+1];
				if(w)	sum += mat[row][column-1];
				if(e)	sum += mat[row][column+1];
				if(sw)	sum += mat[row+1][column-1];
				if(s)	sum += mat[row+1][column];
				if(se)	sum += mat[row+1][column+1];

				fs << sum;
				fs << ' ';
			}
			fs << '\n';
		}
		fs << '\n';
	}
}

int main(int argc, char* argv[])
{
	if(argc != 4)
	{
		cout << "\nProgram usage:\nAlgorithmDemonstrationClient.exe [Port Name] [Input File] [Timeout (msec)]" << endl;
	}
	else //main body of program, only for testing the FPGA side of things right now
	{
		COMio PORT(argv[1], 115200, ONESTOPBIT, 8, NOPARITY);

		vector<unsigned char> buff;
		vector<unsigned char> input;

		read(input, argv[2]); //read the input file
		alg(input);

		PORT.writeSerialPort(input);
		PORT.readSerialPort(atoi(argv[3]));

		PORT.getBuff(buff);

		int row = 0, column = 0;
		fstream fs;
		fs.open("fpga_output.txt", fstream::out);

		for(vector<unsigned char>::iterator it = buff.begin(); it < buff.end(); it++)
		{
			fs << (unsigned int)*it << ' ';
			column++;
			if (column == COLUMNS)
			{
				column = 0;
				row++;
				fs << endl;
			}
			if (row == ROWS)
			{
				fs << endl << endl;
				row = 0;
				column = 0;
			}
		}
	}

	//cout << "Press a key to continue..." << endl;
	//cin.get(); //pause until input
	return 0;
}