// Binary tree abstract base class
// Modified by: Thu Vo
#ifndef _BINARY_TREE
#define _BINARY_TREE

#include "BinaryNode.h"
#include<iostream>
#include<string>
#include "Queue.h"
using namespace std;

template<class ItemType>
class BinaryTree
{
protected:
	BinaryNode<ItemType>* rootPtr;		// ptr to root node
	int count;							// number of nodes in tree

public:
	// "admin" functions
	BinaryTree() { rootPtr = 0; count = 0; }
	BinaryTree(const BinaryTree<ItemType>& tree) { }
	virtual ~BinaryTree() { }

	// common functions for all binary trees
	bool isEmpty() const { return count == 0; }
	int size() const { return count; }

	//void clear()			{destroyTree(rootPtr); rootPtr = 0; count = 0;}
	void preOrder(void visit(ItemType&)) const { _preorder(visit, rootPtr); }
	void inOrder(void visit(ItemType&)) const { _inorder(visit, rootPtr); }
	void postOrder(void visit(ItemType&)) const { _postorder(visit, rootPtr); }
	void levelOrder(void visit(ItemType&)) const;
	void printTree(void visit(ItemType&));


	// abstract functions to be implemented by derived class
	virtual bool insert(const ItemType& newData, int compare(ItemType&, ItemType&)) = 0;
	//virtual bool remove(const ItemType & data) = 0; 
	virtual bool getEntry(const ItemType& target, ItemType& returnedItem, int compare(ItemType&, ItemType&), void display(ItemType&)) const = 0;
	void clearTree();


private:
	// delete all nodes from the tree
	void destroyTree(BinaryNode<ItemType>* nodePtr);

	// internal traverse
	void _preorder(void visit(ItemType&), BinaryNode<ItemType>* nodePtr) const;
	void _inorder(void visit(ItemType&), BinaryNode<ItemType>* nodePtr) const;
	void _postorder(void visit(ItemType&), BinaryNode<ItemType>* nodePtr) const;
	void _printTree(void visit(ItemType&), BinaryNode<ItemType>* nodePtr, string indent, int level);

};





// Breath First Traversal
template<class ItemType>
void BinaryTree<ItemType>::levelOrder(void visit(ItemType&)) const {
	Queue <BinaryNode<ItemType>*> Q;
	BinaryNode<ItemType>* frontItem;
	if (rootPtr == NULL) {
		cout << "Emty tree\n";
		return;
	}
	Q.enqueue(rootPtr);
	while (!Q.isEmpty()) {
		Q.queueFront(frontItem);
		BinaryNode<ItemType>* curr = frontItem;
		ItemType item = curr->getItem();
		visit(item);
		if (curr->getLeftPtr() != NULL)
			Q.enqueue(curr->getLeftPtr());
		if (curr->getRightPtr() != NULL)
			Q.enqueue(curr->getRightPtr());
		Q.dequeue(frontItem);
	}



}




// Print tree
template<class ItemType>
void BinaryTree<ItemType>::printTree(void visit(ItemType&))
{
	int static level = 1;
	_printTree(visit, this->rootPtr, "", level);
}



//====================================== Private function =========================================


//Preorder Traversal
template<class ItemType>
void BinaryTree<ItemType>::_preorder(void visit(ItemType&), BinaryNode<ItemType>* nodePtr) const
{
	if (nodePtr != 0)
	{
		ItemType item = nodePtr->getItem();
		visit(item);
		_preorder(visit, nodePtr->getLeftPtr());
		_preorder(visit, nodePtr->getRightPtr());
	}
}


//Inorder Traversal
template<class ItemType>
void BinaryTree<ItemType>::_inorder(void visit(ItemType&), BinaryNode<ItemType>* nodePtr) const
{
	if (nodePtr != 0) {
		_inorder(visit, nodePtr->getLeftPtr());
		ItemType item = nodePtr->getItem();
		visit(item);
		_inorder(visit, nodePtr->getRightPtr());
	}

}

//Postorder Traversal
template<class ItemType>
void BinaryTree<ItemType>::_postorder(void visit(ItemType&), BinaryNode<ItemType>* nodePtr) const
{
	if (nodePtr != 0) {
		_postorder(visit, nodePtr->getLeftPtr());
		_postorder(visit, nodePtr->getRightPtr());
		ItemType item = nodePtr->getItem();
		visit(item);
	}

}



// Print tree
template<class ItemType>
void BinaryTree<ItemType>::_printTree(void visit(ItemType&), BinaryNode<ItemType>* nodePtr, string indent, int level) {
	if (nodePtr != NULL) {
		cout << indent << "Level " << level << ". ";
		ItemType item = nodePtr->getItem();
		visit(item);
		level++;
		_printTree(visit, nodePtr->getRightPtr(), indent + "\t\t", level);
		_printTree(visit, nodePtr->getLeftPtr(), indent + "\t\t", level);
	}
}



template<class ItemType>
void BinaryTree<ItemType>::clearTree()
{
	destroyTree(this->rootPtr);
}

//Destroy the entire tree
template<class ItemType>
void BinaryTree<ItemType>::destroyTree(BinaryNode<ItemType>* nodePtr)
{
	if (nodePtr != NULL) {
		destroyTree(nodePtr->getLeftPtr());
		destroyTree(nodePtr->getRightPtr());
		//ItemType item = nodePtr->getItem();
		delete(nodePtr);
	}
}

#endif
