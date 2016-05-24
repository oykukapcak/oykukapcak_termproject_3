#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include <string>
#include <pthread.h>

using namespace std;

class Memory{

private:
	char* memory;
	int * bitMap;
	int memorySize;
	pthread_mutex_t mutex;
public:
	Memory(int size);
	~Memory();
	void addInstructions(char* buffer, int bufferSize, int BR); 
	void removeInstructions(int bufferSize, int BR);
	char* getInstruction(int PC, int BR); 
	int hasFreeSpace(int size);

};

#endif