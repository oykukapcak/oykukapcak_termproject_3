#ifndef _PROCESSINFO_H
#define _PROCESSINFO_H

#include <string>
using namespace std;

class ProcessInfo
{
private:
	string fileName;
	int instructionSize;
public:
	ProcessInfo();
	ProcessInfo(string fName, int insSize);
	string getFileName();
	int getInstructionSize();
};
#endif