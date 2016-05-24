#include "LinkedList.h"
#include <iostream>
LinkedList::LinkedList()
{
	head = NULL;
	pthread_mutex_init(&mutex, NULL);
}

LinkedList::~LinkedList()
{
	pthread_mutex_destroy(&mutex);
	delete head;
}


void LinkedList::insertItemToEnd(ProcessImage item)
{
	Node * ptr = head;

	pthread_mutex_lock(&mutex); //bu mutexlerin yerleri tamamen sýkýþ

	if(head == NULL)
	{
		head = new Node(item, NULL);
	}

	else
	{
		while(ptr->next != NULL)
		{
			ptr = ptr->next;
		}

		ptr->next = new Node(item, NULL);
	}
	pthread_mutex_unlock(&mutex);
}

Node LinkedList::removeFirstItem()
{
	Node* ptr = head;

	pthread_mutex_lock(&mutex);
	head = head->next;
	pthread_mutex_unlock(&mutex);
	return *ptr;
}

bool LinkedList::isEmpty()
{
	if(head == NULL)
		return true;
	else
		return false;
}
