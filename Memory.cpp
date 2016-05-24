#include "Memory.h"

Memory::Memory(int size)
{
	memory = new char[size];
	memorySize = size;
	bitMap = new int[size]; 

	for(int i = 0; i < size; i++)
	{
		bitMap[i] = 0;
	}

	pthread_mutex_init(&mutex, NULL);
}

Memory::~Memory()
{
	delete memory;
	delete bitMap;
	pthread_mutex_destroy(&mutex);
}


void Memory::addInstructions(char * buffer, int bufferSize, int BR)
{
	pthread_mutex_lock(&mutex);
	for (int i = 0; i < bufferSize; i++)
	{	
		memory[i + BR] = buffer[i]; //BR eklemeden emin deðilim?!!?!
		bitMap[i + BR] = 1;
	}
	pthread_mutex_unlock(&mutex);
}

char * Memory::getInstruction(int PC, int BR) 
{
	char* instruction = new char[4];

	pthread_mutex_lock(&mutex);
	for (int i = 0; i < 4; i++)
	{
		instruction[i] = memory[BR + PC + i];
	}
	pthread_mutex_unlock(&mutex);

	return instruction;
}

void Memory::removeInstructions(int bufferSize, int BR)
{
	pthread_mutex_lock(&mutex);
	for(int i = 0; i < bufferSize; i++)
	{
		bitMap[i + BR] = 0;
	}
	pthread_mutex_unlock(&mutex);
}

int Memory::hasFreeSpace(int size)
{
	int start_index;

	pthread_mutex_lock(&mutex);
	for(int i = 0; i < memorySize; i++)
	{
		for(int j = 0; bitMap[i+j] == 0 && j < size; j++)
		{
			if(j == size-1)
			{
				start_index = i+j - size +1;
				pthread_mutex_unlock(&mutex);
				return start_index;
			}
		}

	}
	pthread_mutex_unlock(&mutex);
	return -1; //boþ yer bulamadýysa
}




