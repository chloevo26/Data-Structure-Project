// Binary Search Tree ADT
// Modified by: Thu Vo

#ifndef _BINARY_SEARCH_TREE
#define _BINARY_SEARCH_TREE

#include "BinaryTree.h"

template <class ItemType>
class BinarySearchTree : public BinaryTree<ItemType>
{

private:
	// internal insert node: insert newNode in nodePtr subtree
	BinaryNode<ItemType>* _insert(BinaryNode<ItemType>* nodePtr, BinaryNode<ItemType>* newNode, int compare(ItemType&, ItemType&));
	// internal _findDuplicate: traverse the tree and find duplicated node
	void _findDuplicate(BinaryNode<ItemType>* nodePtr, const ItemType& target, int compare(ItemType&, ItemType&), bool& success, Queue <ItemType>*& q) const;

	// internal remove node: locate and delete target node under nodePtr subtree
	BinaryNode<ItemType>* _remove(BinaryNode<ItemType>* nodePtr, const ItemType target, bool& success, int compare(ItemType&, ItemType&));

	// delete target node from tree, called by internal remove node
	BinaryNode<ItemType>* deleteNode(BinaryNode<ItemType>* targetNodePtr);

	// remove the leftmost node in the left subtree of nodePtr
	BinaryNode<ItemType>* removeLeftmostNode(BinaryNode<ItemType>* nodePtr, ItemType& successor);

	// search for target node
	BinaryNode<ItemType>* findNode(BinaryNode<ItemType>* treePtr, const ItemType& target, int compare(ItemType&, ItemType&)) const;


	// find smallest item
	BinaryNode<ItemType>* _findSmallest(BinaryNode<ItemType>* nodePtr) const;

	// find largest item
	BinaryNode<ItemType>* _findLargest(BinaryNode<ItemType>* nodePtr) const;


public:
	// insert a node at the correct location by ID
	bool insert(const ItemType& newData, int compare(ItemType&, ItemType&));
	// remove a node if found
	bool remove(const ItemType& anEntry, int compare(ItemType&, ItemType&));
	// find a target node
	//bool getEntry(const ItemType & target, ItemType & returnedItem, int compare(ItemType &, ItemType &)) const;
	bool getEntry(const ItemType& target, ItemType& returnedItem, int compare(ItemType&, ItemType&), void display(ItemType&)) const;
	// find a smallest node
	void getSmallest(ItemType& returnedSmallest) const;
	// find a largest node
	void getLargest(ItemType& returnedSmallest) const;

	virtual ~BinarySearchTree() {}

};


///////////////////////// public function definitions ///////////////////////////
//Inserting items within a tree
template<class ItemType>
bool BinarySearchTree<ItemType>::insert(const ItemType& newEntry, int compare(ItemType&, ItemType&))
{
	BinaryNode<ItemType>* newNodePtr = new BinaryNode<ItemType>(newEntry);
	this->rootPtr = _insert(this->rootPtr, newNodePtr, compare);
	this->count++;
	return true;
}


//Removing items within a tree with duplicate key
template<class ItemType>
bool BinarySearchTree<ItemType>::remove(const ItemType& target, int compare(ItemType&, ItemType&))
{
	bool isSuccessful = false;
	this->rootPtr = _remove(this->rootPtr, target, isSuccessful, compare);
	return isSuccessful;
}



// Finding the entries within a tree with duplicate 
template <class ItemType>
bool BinarySearchTree<ItemType>::getEntry(const ItemType& anEntry, ItemType& returnedItem, int compare(ItemType&, ItemType&), void display(ItemType&)) const {
	bool success = false;
	Queue <ItemType>* q = new Queue<ItemType>;	  // Pointer to a queue that holds address of duplicate node
	_findDuplicate(this->rootPtr, anEntry, compare, success, q);	// find duplicate of the key 
	ItemType temp;
	while (!q->isEmpty()) {
		q->dequeue(temp);
		display(temp);
	}
	delete q;
	return success;
}


// Finding smallest element
template<class ItemType>
void BinarySearchTree<ItemType>::getSmallest(ItemType& returnedSmallest) const {
	BinaryNode<ItemType>* smallestPtr;
	smallestPtr = _findSmallest(this->rootPtr);
	returnedSmallest = smallestPtr->getItem();
}

template<class ItemType>
void BinarySearchTree<ItemType>::getLargest(ItemType& returnedLargest) const {
	BinaryNode<ItemType>* largestPtr;
	largestPtr = _findLargest(this->rootPtr);
	returnedLargest = largestPtr->getItem();
}




//////////////////////////// private functions ////////////////////////////////////////////
//Implementation of the insert operation
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::_insert(BinaryNode<ItemType>* nodePtr,
	BinaryNode<ItemType>* newNodePtr, int compare(ItemType&, ItemType&))
{
	if (nodePtr == 0) { // if tree is empty
		nodePtr = newNodePtr;
		return newNodePtr;
	}
	ItemType newItem = newNodePtr->getItem();
	ItemType item = nodePtr->getItem();
	// case 1 - data in is less than the data that is already in the tree
	if (compare(newItem, item) == -1) {
		if (nodePtr->getLeftPtr() != NULL)
			nodePtr->setLeftPtr(_insert(nodePtr->getLeftPtr(), newNodePtr, compare));
		else {
			nodePtr->setLeftPtr(newNodePtr);
			return nodePtr;
		}

	}
	// case 2 - data in is greater than the data athat is already in the tree
	else {
		if (nodePtr->getRightPtr() != NULL)
			nodePtr->setRightPtr(_insert(nodePtr->getRightPtr(), newNodePtr, compare));
		else {
			nodePtr->setRightPtr(newNodePtr);
			return nodePtr;
		}
	}

}


//Implementation of the remove operation
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::_remove(BinaryNode<ItemType>* nodePtr,
	const ItemType target,
	bool& success, int compare(ItemType&, ItemType&))

{
	if (nodePtr == 0)
	{
		success = false;
		return 0;
	}
	ItemType currentItem = nodePtr->getItem();
	ItemType targetItem = target;
	if (compare(currentItem, targetItem) == 1)
		nodePtr->setLeftPtr(_remove(nodePtr->getLeftPtr(), target, success, compare));
	else if (compare(currentItem, targetItem) == -1)
		nodePtr->setRightPtr(_remove(nodePtr->getRightPtr(), target, success, compare));
	else
	{
		nodePtr = deleteNode(nodePtr);
		success = true;
	}
	return nodePtr;
}

//Implementation of the delete operation
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::deleteNode(BinaryNode<ItemType>* nodePtr)
{
	if (nodePtr->isLeaf())
	{
		delete nodePtr;
		nodePtr = 0;
		return nodePtr;
	}
	else if (nodePtr->getLeftPtr() == 0)
	{
		BinaryNode<ItemType>* nodeToConnectPtr = nodePtr->getRightPtr();
		delete nodePtr;
		nodePtr = 0;
		return nodeToConnectPtr;
	}
	else if (nodePtr->getRightPtr() == 0)
	{
		BinaryNode<ItemType>* nodeToConnectPtr = nodePtr->getLeftPtr();
		delete nodePtr;
		nodePtr = 0;
		return nodeToConnectPtr;
	}
	else
	{
		ItemType newNodeValue;
		nodePtr->setRightPtr(removeLeftmostNode(nodePtr->getRightPtr(), newNodeValue));
		nodePtr->setItem(newNodeValue);
		return nodePtr;
	}
}
//Implementation to remove the left leaf
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::removeLeftmostNode(BinaryNode<ItemType>* nodePtr,
	ItemType& successor)
{
	if (nodePtr->getLeftPtr() == 0)
	{
		successor = nodePtr->getItem();
		return deleteNode(nodePtr);
	}
	else
	{
		nodePtr->setLeftPtr(removeLeftmostNode(nodePtr->getLeftPtr(), successor));
		return nodePtr;
	}
}


//Implementation for the search operation
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::findNode(BinaryNode<ItemType>* nodePtr,
	const ItemType& target, int compare(ItemType&, ItemType&)) const
{

	BinaryNode<ItemType>* found = NULL;
	BinaryNode<ItemType>* pWalk = nodePtr;
	while (pWalk != NULL && found == NULL) {
		ItemType currItem = pWalk->getItem();
		ItemType targetItem = target;
		if (compare(currItem, targetItem) == -1) {		// if target < the value in the current node ==> go left
			pWalk = pWalk->getLeftPtr();
		}
		else {
			if (compare(currItem, targetItem) == 1) {	// if target > the value in the current node => go right
				pWalk = pWalk->getRightPtr();
			}
			else {								// if target == the value in the current node 
				found = pWalk;
			}
		}
	}

	return found;

}

// Finding duplicate node 
template <class ItemType>
void BinarySearchTree<ItemType>::_findDuplicate(BinaryNode<ItemType>* nodePtr, const ItemType& target, int compare(ItemType&, ItemType&), bool& success, Queue <ItemType>*& q) const {
	if (nodePtr != NULL) {
		_findDuplicate(nodePtr->getLeftPtr(), target, compare, success, q);
		ItemType currItem = nodePtr->getItem();
		ItemType targetItem = target;
		if (compare(currItem, targetItem) == 0) {
			success = true;
			q->enqueue(nodePtr->getItem());
		}
		_findDuplicate(nodePtr->getRightPtr(), target, compare, success, q);
	}
}



// Implement for the findSmallest operation
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::_findSmallest(BinaryNode<ItemType>* nodePtr) const
{
	if (nodePtr->getLeftPtr() == 0)
	{
		return nodePtr;
	}
	else {
		return _findSmallest(nodePtr->getLeftPtr());
	}
}


// Implement for the findLargest operation
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::_findLargest(BinaryNode<ItemType>* nodePtr) const
{
	if (nodePtr->getRightPtr() == 0)
	{
		return nodePtr;
	}
	else {
		return _findLargest(nodePtr->getRightPtr());
	}
}



#endif