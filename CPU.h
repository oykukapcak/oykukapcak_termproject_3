#ifndef CPU_H
#define CPU_H

#include "Memory.h"
#include "ProcessImage.h"

class CPU{

private:
	
	unsigned int S0;
	unsigned int S1;
	unsigned int S2;
	unsigned int S3;
	unsigned int S4;
	unsigned int S5;
	unsigned int S6;
	unsigned int S7;
	unsigned int $0;
	unsigned int PC;
	unsigned int V;
	unsigned int IR;
	unsigned int BR;
	unsigned int LR;

	Memory* memory;
	
	unsigned int* CPU::ReturnReg(int num);
public:
	CPU(Memory*);

	void fetch();
	void decodeExecute();
	int getLR();
	int getPC();
	int getIR();
	int getV();
	int getBR();

	
	
	void transferFromImage(ProcessImage);
	void transferToImage(ProcessImage &);
};


#endif