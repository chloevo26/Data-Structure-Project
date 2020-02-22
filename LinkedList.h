/******************************************************************************
 LinkedList template
 Developer: Linh Tran (Cynthia)
 **/

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include <string>
using namespace std;

template <class ItemType, class KeyType>
struct DataNode
{
	ItemType item;
	KeyType key;
};


template <class ItemType, class KeyType>
struct ListNode
{
	DataNode<ItemType, KeyType> data;
	ListNode* prev;
	ListNode* next;
};

template <class ItemType, class KeyType>
class LinkedList
{
private:
	ListNode<ItemType, KeyType>* head;
	int count;

public:
	// Constructor
	LinkedList() { head = new ListNode<ItemType, KeyType>; head->next = head; head->prev = head; count = 0; }

	// Getters
	ListNode<ItemType, KeyType>* getHead() { return head; }
	ItemType getData() { return data.item; }
	KeyType getKey() { return data.key; }
	ListNode<ItemType, KeyType>* getNext() { return next; }
	ListNode<ItemType, KeyType>* getPrev() { return prev; }

	// Linked list operations
	void insertNode(ItemType, KeyType, int compare(ItemType&, ItemType&));
	bool deleteNode(ItemType, int compare(ItemType&, ItemType&));
	bool searchList(ItemType, ItemType&, int compare(ItemType&, ItemType&));
	bool isEmpty() { return count == 0; };
	int getCount() { return count; };
	void traverseForward(void visit(ItemType&)) const;
	void traverseBackward(void visit(ItemType&)) const;

	//Destructor
	~LinkedList();
};


/******************************************************************************
 inserNode function
 **/
template <class ItemType, class KeyType>
void LinkedList<ItemType, KeyType>::insertNode(ItemType dataIn, KeyType keyData, int compare(ItemType&, ItemType&))
{
	// Allocate the nodes to store data
	ListNode<ItemType, KeyType>* newNode;
	newNode = new ListNode<ItemType, KeyType>;
	newNode->data.item = dataIn;
	newNode->data.key = keyData;

	// Allocate the nodes to traverse the list
	ListNode<ItemType, KeyType>* pCur;
	ListNode<ItemType, KeyType>* pPre;
	// Initialize the pointers
	pPre = head;
	pCur = head->next;

	// Find location
	while (pCur != head && compare(pCur->data.item, dataIn) == -1)
	{
		pPre = pCur;
		pCur = pCur->next;
	}

	// Insert the new node between pPre and pCur
	if (pCur != NULL)
	{
		newNode->next = pCur;
		newNode->prev = pPre;
		pPre->next = newNode;
		pCur->prev = newNode;
	}
	else
	{
		newNode->next = head->next;
		newNode->prev = pPre;
		pPre->next = newNode;
		head->next->prev = newNode;
	}
	//Update the counter
	count++;
}


/******************************************************************************
 deleteNode function
 **/
template <class ItemType, class KeyType>
bool LinkedList<ItemType, KeyType>::deleteNode(ItemType dataOut, int compare(ItemType&, ItemType&))
{
	// Check if deleteNode works
	bool deleted = false;

	// Allocate the nodes to traverse the list
	ListNode<ItemType, KeyType>* pCur;
	ListNode<ItemType, KeyType>* pPre;
	// Initialize pointers
	pPre = head;
	pCur = head->next;

	// Find node containing the target
	while (pCur != head && compare(pCur->data.item, dataOut) == -1)
	{
		pPre = pCur;
		pCur = pCur->next;
	}

	// If found, delete the node
	if (pCur != head && compare(pCur->data.item, dataOut) == 0)
	{
		pPre->next = pCur->next;
		pCur->next->prev = pPre;
		delete pCur;
		deleted = true;
		//Update counter
		count--;
	}
	return deleted;
}


/******************************************************************************
 searchList function
 **/
template <class ItemType, class KeyType>
bool LinkedList<ItemType, KeyType>::searchList(ItemType target, ItemType& dataOut, int compare(ItemType&, ItemType&))
{
	ListNode<ItemType, KeyType>* pCur;
	pCur = head->next;
	bool found = false;

	while (pCur != head && compare(pCur->data.item, target) == -1)
		pCur = pCur->next;

	if (pCur != head && compare(pCur->data.item, target) == 0)
	{
		dataOut = pCur->data.item;
		found = true;
	}
	return found;
}


/******************************************************************************
 traverseForward function
 **/
template <class ItemType, class KeyType>
void LinkedList<ItemType, KeyType>::traverseForward(void visit(ItemType&)) const
{
	// Allocate the first node
	ListNode<ItemType, KeyType>* pCur;
	pCur = head->next;

	while (pCur != head)
	{
		visit(pCur->data.item);
		pCur = pCur->next;
	}
}


/******************************************************************************
 traverseBackward function
 **/
template <class ItemType, class KeyType>
void LinkedList<ItemType, KeyType>::traverseBackward(void visit(ItemType&)) const
{
	// Allocate the last node
	ListNode* pCur;
	pCur = head->prev;

	while (pCur != head)
	{
		visit(pCur->data);
		pCur = pCur->prev;
	}
}


// Destructor
template <class ItemType, class KeyType>
LinkedList<ItemType, KeyType>::~LinkedList()
{
	// Allocate nodes to traverse and delete the list
	ListNode<ItemType, KeyType>* pCur;
	ListNode<ItemType, KeyType>* pNext;
	// Initialize the pointer
	pCur = head->next;

	// Delete the data inside the list
	while (pCur != head)
	{
		pNext = pCur->next;	// Save the location of the next node
		delete pCur;	// Delete the current node.
		pCur = pNext;	// Move to the next node
	}
	delete head; // Delete the sentinel node
}

#endif // LINKEDLIST_H_