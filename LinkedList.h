#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H
#include <pthread.h>
#include "ProcessImage.h"


struct Node {
	ProcessImage item;
	Node * next;

	Node()
	{}

	Node(ProcessImage i, Node * n) : item(i), next(n)
	{}
};

class LinkedList
{
public:
	LinkedList();
	~LinkedList();
	void insertItemToEnd(ProcessImage item);
	Node removeFirstItem();
	bool isEmpty();
private:
	Node * head;
	pthread_mutex_t mutex;
};
#endif