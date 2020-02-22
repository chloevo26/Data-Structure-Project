/******************************************************************************
 HashTable template
 Developer: Linh Tran (Cynthia), Jack Ou
 ******************************************************************************/

#ifndef _HASH_TABLE
#define _HASH_TABLE

#include "LinkedList.h"

template<class ItemType, class KeyType>
class HashTable
{
private:
	LinkedList<ItemType, KeyType>* hashAry;
	int arySize;
	int count;

	int collision;

	int nextPrime(int current);
	void _reHash(int hashFun(KeyType), int compare(ItemType&, ItemType&));

public:
	// Constructor -- Create a new hash table
	HashTable() { arySize = 10; count = 0; collision = 0; hashAry = new LinkedList<ItemType, KeyType>[arySize]; }
	HashTable(int size) { arySize = size; count = 0; collision = 0; hashAry = new LinkedList<ItemType, KeyType>[arySize]; }
	~HashTable() { delete[] hashAry; }

	// Common functions for all hash table
	bool isEmpty() const { return count == 0; }
	int getCount() const { return arySize; }
	int getCollision() const { return collision; }
	double loadFactor();
	bool insertHash(ItemType itemIn, int hashFun(KeyType), KeyType key, int compare(ItemType&, ItemType&));
	bool deleteHash(ItemType itemOut, KeyType key, int hashFun(KeyType), int compare(ItemType&, ItemType&));
	bool searchHash(ItemType target, KeyType key, int hashFun(KeyType), ItemType& dataOut, int compare(ItemType&, ItemType&));

	// Other functions
	void printHash(void visit(ItemType&));
};


/******************************************************************************
 inserHash function
 **/
template<class ItemType, class KeyType>
bool HashTable<ItemType, KeyType>::insertHash(ItemType itemIn, int hashFun(KeyType), KeyType key, int compare(ItemType&, ItemType&))
{
	bool isRehashed = false;

	int location = hashFun(key) % arySize;

	if (location >= arySize || location < 0)
		return false;

	if (loadFactor() >= 0.75)
	{
		isRehashed = true;
		_reHash(hashFun, compare);
	}

	if (hashAry[location].isEmpty())
		count++;
	else
		collision++;

	hashAry[location].insertNode(itemIn, key, compare);

	return !isRehashed;
}


/******************************************************************************
 deleteHash function
 **/
template<class ItemType, class KeyType>
bool HashTable<ItemType, KeyType>::deleteHash(ItemType itemOut, KeyType key, int hashFun(KeyType), int compare(ItemType&, ItemType&))
{
	int location = hashFun(key) % arySize;
	if (location >= arySize || location < 0)
		return false;
	count--;
	return hashAry[location].deleteNode(itemOut, compare);
}


/******************************************************************************
 searchHash function
 **/
template<class ItemType, class KeyType>
bool HashTable<ItemType, KeyType>::searchHash(ItemType target, KeyType key, int hashFun(KeyType), ItemType& dataOut, int compare(ItemType&, ItemType&))
{
	int location = hashFun(key) % arySize;
	if (location >= arySize || location < 0)
		return false;
	return hashAry[location].searchList(target, dataOut, compare);
}

// Calculate the load factor
template<class ItemType, class KeyType>
double HashTable<ItemType, KeyType>::loadFactor()
{
	double count = 0;
	for (int i = 0; i < arySize; i++)
	{
		if (!(hashAry[i].isEmpty()))
			count++;
	}

	return count / static_cast<double>(arySize);
}

// Find the next prime number
template<class ItemType, class KeyType>
int HashTable<ItemType, KeyType>::nextPrime(int current)
{
	for (int i = 2; i <= sqrt(current); i++)
	{
		if (current % i == 0)
			return nextPrime(current + 1);
	}
	return current;
}

/******************************************************************************
 _reHash function
 **/
template<class ItemType, class KeyType>
void HashTable<ItemType, KeyType>::_reHash(int hashFun(KeyType), int compare(ItemType&, ItemType&))
{
	LinkedList<ItemType, KeyType>* temp = hashAry;
	int n = arySize;
	collision = 0;
	int newArySize = arySize * 2;
	arySize = nextPrime(newArySize);

	hashAry = new LinkedList<ItemType, KeyType>[arySize];

	for (int i = 0; i < n; i++)
	{
		ListNode<ItemType, KeyType>* moveData = temp[i].getHead();
		ListNode<ItemType, KeyType>* head = moveData;
		moveData = moveData->next;

		while (moveData != head)
		{
			ItemType moveItem = moveData->data.item;
			KeyType itemKey = moveData->data.key;

			insertHash(moveItem, hashFun, itemKey, compare);
			moveData = moveData->next;
		}
	}
	//arySize = newArySize;
	delete[] temp;
}

template<class ItemType, class KeyType>
void HashTable<ItemType, KeyType>::printHash(void visit(ItemType&))
{
	for (int i = 0; i < arySize; i++) {
		hashAry[i].traverseForward(visit);
	}
}
#endif //_HASHTABLE_H
