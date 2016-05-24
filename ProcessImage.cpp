#include "ProcessImage.h"

ProcessImage::ProcessImage()
{
	S0 = 0;
	S1 = 0;
	S2 = 0;
	S3 = 0;
	S4 = 0;
	S5 = 0;
	S6 = 0;
	S7 = 0;
	$0 = 0;
	PC = 0;
	V = 0;
	IR = 0;
	BR = 0;
	LR = 0;
}

ProcessImage::ProcessImage(int baseRegister, int limitRegister)
{
	S0 = 0;
	S1 = 0;
	S2 = 0;
	S3 = 0;
	S4 = 0;
	S5 = 0;
	S6 = 0;
	S7 = 0;
	$0 = 0;
	PC = 0;
	V = 0;
	IR = 0;
	BR = baseRegister;
	LR = limitRegister;
}

void ProcessImage::dumpToDisk()
{
	ofstream output;
	output.open("processRegisterDump.bin");

	output << "S0 " << S0 << endl
		<< "S1 " << S1 << endl
		<< "S2 " << S2 << endl
		<< "S3 " << S3 << endl
		<< "S4 " << S4 << endl
		<< "S5 " << S5 << endl
		<< "S6 " << S6 << endl
		<< "S7 " << S7 << endl
		<< "$0 " << $0 << endl
		<< "PC " << PC << endl
		<< "V " << V << endl
		<< "IR " << IR << endl
		<< "BR " << BR << endl
		<< "LR " << LR << endl;
}
