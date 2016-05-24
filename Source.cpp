#include <iostream>
#include <string>
#include <pthread.h>
#include <semaphore.h>
#include <Windows.h>
#include <fstream>
#include "Memory.h"
#include "CPU.h"
#include "Assembler.cpp"
#include "ProcessImage.h"
#include "ProcessInfo.h"
#include "BoundedBuffer.h"
#include "LinkedList.h"

using namespace std;

const int quantumConstant = 5;
Memory * mainMemory;
CPU * myCPU;
BoundedBuffer<ProcessInfo> * fileInputQueue;
BoundedBuffer<unsigned int> * consoleInputQueue;
LinkedList * readyQueue;
LinkedList * blockedQueue;

void * producerFileInputFunction(void *)
{
	ifstream input;
	input.open("inputSequence.txt");

	string filePath, binaryFile;
	int time, size;

	while(!input.eof())
	{
		input >> filePath >> time;
		printf("File path read: %s \n", filePath.c_str());
		size = createBinaryFile(filePath);
		binaryFile = filePath.substr(0, filePath.find(".")) + ".bin";
		ProcessInfo proInfo(binaryFile, size);

		printf("Assembly file opened, converted it to binary and written it to the binary file: %s \n", binaryFile.c_str());

		fileInputQueue->insert_item(proInfo);

		printf("Process info created and stored in file input queue. \n");
		printf("Now thread will sleep for %i ms. \n", time);
		Sleep(time);
	}

	return NULL;
}

void * consumerFileInputFunction(void *)
{
	char* buffer;
	int size;
	string fileName;

	while(true) 
	{
		ProcessInfo proInfo = fileInputQueue->remove_item();
		
		size = proInfo.getInstructionSize();
		fileName = proInfo.getFileName();
		printf("File %s removed from the file input queue. \n", fileName.c_str());
		printf("Check for available space in memory for %i \n", size);
		

		while(mainMemory->hasFreeSpace(size) == -1) 
		{
			Sleep(2000);
		}

		int BR = mainMemory->hasFreeSpace(size); 
		
		printf("Available space found, binary file %s is opened. \n", fileName.c_str());
		buffer = readBinaryFile(size, fileName);
		mainMemory->addInstructions(buffer, size, BR);
		printf("Binary version of instructions is sloaded into the memory. \n");

		ProcessImage proImage(BR, BR+size);
		readyQueue->insertItemToEnd(proImage);
		printf("New process instance is created and added to the end of ready queue. \n");
	}

	return NULL;
}

void * osThreadFunction(void *)
{
	int usedQuantum;
	bool isBlocked;
	while(true)
	{
		if(readyQueue->isEmpty()) 
		{
			Sleep(2000);
		}

		else
		{
			Node nodeToBeExec = readyQueue->removeFirstItem();
			printf("First process of the ready queue is removed.\n");
			ProcessImage processToBeExec = nodeToBeExec.item;
			int size = processToBeExec.LR - processToBeExec.BR +1;
			myCPU->transferFromImage(processToBeExec);
			printf("Process image is transferred to CPU. Fetch-Decode-Execute cycle will begin. \n");
			usedQuantum = 0;
			isBlocked = false;

			while(usedQuantum < quantumConstant && myCPU->getPC() != (myCPU->getLR() - myCPU->getBR()) && !isBlocked) 
			{
				myCPU->fetch();
				printf("Fetch is done \n");
				printf("IR: %i \n", myCPU->getIR());
				printf("BR: %i \n", myCPU->getBR());
				printf("PC: %i \n", myCPU->getPC()+myCPU->getBR());
				printf("LR: %i \n", myCPU->getLR());
				myCPU->decodeExecute();
				printf("Decoded and executed \n");

				if(myCPU->getIR() == 0) //I/O operation
				{
					if(myCPU->getV() == 0) //read
					{
						printf("I/O operation. Process should receive input. So it is sent to the end of Blocked Queue.\n");
						myCPU->transferToImage(processToBeExec);
						blockedQueue->insertItemToEnd(processToBeExec);
						isBlocked = true;
					}

					else if(myCPU->getV() != 0) //write
					{
						printf("I/O operatioon. Process is giving output: ");	
						printf("%i \n", myCPU->getV());
					}
				}

				usedQuantum++;
				printf("Used quantum is: %i \n", usedQuantum);
			}
		
			if(!isBlocked)
			{
				if(usedQuantum < quantumConstant) //If a process finishes executing before its quantum if over, 
												//then the thread should empty the memory allocated by the process.

				{
					printf("Process finished executing before its quantum is over, the thread will empty the memory allocated by the process. \n");
					myCPU->transferToImage(processToBeExec);
					mainMemory->removeInstructions(size, processToBeExec.BR);
				}

				else if(usedQuantum == quantumConstant) //process bitmedi ama quantum bitti VE block'a da girmedi
				{
					printf("Process finished its quantum time and it is not finished or blocked, it is sent back to the end of the Ready Queue. \n");
					myCPU->transferToImage(processToBeExec);
					readyQueue->insertItemToEnd(processToBeExec);
				}
			}
			processToBeExec.dumpToDisk();
		}
	}

	return NULL;
}

void * producerConsoleInputFunction(void *)
{
	unsigned int input_int;

	while(true)
	{
		cin >> input_int;
		printf("Input '%i' is received and added to Console Input Queue. \n", input_int);
		consoleInputQueue->insert_item(input_int);
	}


	return NULL;
}

void * consumerConsoleInputFunction(void *)
{
	unsigned int input_int;
	Node process_node;

	while(true)
	{
		input_int = consoleInputQueue->remove_item();
		printf("Removed '%i' from the Console Input Queue. \n", input_int);

		while(blockedQueue->isEmpty()) //there are no processes in Blocked Queue
		{
			Sleep(2000);
		}

		process_node = blockedQueue->removeFirstItem();
		printf("First process of the Blocked Queue removed. \n");
		process_node.item.V = input_int;
		printf("Value of the register V is set to '%i'. \n", input_int);
		readyQueue->insertItemToEnd(process_node.item);
		printf("Process is put to the end of the Ready Queue. \n");
	}

	return NULL;
}
int main()
{
	pthread_t producerFileInput, consumerFileInput, osThread, producerConsoleInput, consumerConsoleInput;
	mainMemory = new Memory(80);
	myCPU = new CPU(mainMemory);
	fileInputQueue = new BoundedBuffer<ProcessInfo>(5);
	consoleInputQueue = new BoundedBuffer<unsigned int>(5);
	readyQueue = new LinkedList();
	blockedQueue = new LinkedList();

	pthread_create(&producerFileInput, NULL, producerFileInputFunction, NULL);
	pthread_create(&consumerFileInput, NULL, consumerFileInputFunction, NULL);
	pthread_create(&osThread, NULL, osThreadFunction, NULL);
	pthread_create(&producerConsoleInput, NULL, producerConsoleInputFunction, NULL);
	pthread_create(&consumerConsoleInput, NULL, consumerConsoleInputFunction, NULL);

	pthread_join(producerFileInput, NULL);
	pthread_join(consumerFileInput, NULL);
	pthread_join(osThread, NULL);
	pthread_join(producerConsoleInput, NULL);
	pthread_join(consumerConsoleInput, NULL);

	return 0;
}