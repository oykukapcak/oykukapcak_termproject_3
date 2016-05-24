#include "ProcessInfo.h"

ProcessInfo::ProcessInfo()
{
	fileName = "";
	instructionSize = 0;
}


ProcessInfo::ProcessInfo(string fName, int insSize)
{
	fileName = fName;
	instructionSize = insSize;
}

string ProcessInfo::getFileName()
{
	return fileName;
}

int ProcessInfo::getInstructionSize()
{
	return instructionSize;
}


