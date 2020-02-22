//  main.cpp
//  22C_TeaMProject
//
//

// Adjust hashing functions here
// hashing_LC = low collision
// hashing HC = high collision
#define HASHING_ALGORITHM hashing_LC

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cctype>
#include <Windows.h>
#include <cmath>

#include "BinarySearchTree.h"
#include "Part.h"
#include "HashTable.h"
#include "StackADT.h"

using namespace std;

const string inputFile = "input.txt";
const string outputFile = "output.txt";

// Prototypes
void testDriver(BinarySearchTree<Part*>* idTree, BinarySearchTree<Part*>* producerTree, HashTable<Part*, string>* hashTab, Stack <Part*>* stack);
void buildDatabase(string filename,
	BinarySearchTree<Part*>* obj1, BinarySearchTree<Part*>* obj2, HashTable<Part*, string>* obj3);
void insertOperation(BinarySearchTree<Part*>* obj1, BinarySearchTree<Part*>* obj2, HashTable<Part*, string>* obj3);
void deleteOperation(BinarySearchTree<Part*>* obj1, BinarySearchTree<Part*>* obj2, HashTable<Part*, string>* obj3, Stack <Part*>*& stack);
void unDeleteOperation(BinarySearchTree<Part*>* obj1, BinarySearchTree<Part*>* obj2, HashTable<Part*, string>* obj3, Stack <Part*>* stack);
void listOperation(BinarySearchTree<Part*>* obj1, BinarySearchTree<Part*>* obj2, HashTable<Part*, string>* obj3);
void searchOperation(BinarySearchTree<Part*>* obj2, HashTable<Part*, string>* obj3);
//void saveOutput(string filename, BinarySearchTree<Part*>* obj);
//void outputDisplay(ofstream outFile, Part*& part);
void outputDisplay(Part*& part);
void saveToFile(BinarySearchTree<Part*>* tree);

int compareID(Part*& obj1, Part*& obj2);
int compareProducer(Part*& obj1, Part*& obj2);
void display(Part*& part);
void displayID(Part*& part);
void displayProducer(Part*& part);
int hashing_LC(string key);
int hashing_HC(string key);
void printTitle(string subtitle);
void showMenu();
void listMenu();
void hold();
void printTag();
bool confirm();
void searchMenu();
void deleteMenu();
void printStat(HashTable < Part*, string>* obj);

int main()
{
	int ArySize = 15;
	BinarySearchTree<Part*>* idTree = new BinarySearchTree<Part*>;;
	BinarySearchTree<Part*>* producerTree = new BinarySearchTree<Part*>;
	HashTable<Part*, string>* hashTab = new HashTable < Part*, string>(ArySize);
	Stack<Part*>* undoStack = new Stack<Part*>;

	buildDatabase(inputFile, idTree, producerTree, hashTab);

	testDriver(idTree, producerTree, hashTab, undoStack);

	delete idTree;
	delete producerTree;
	delete hashTab;
	delete undoStack;

	system("pause");
	return 0;
}



/*****************************************************************************
 Build Database:
 *****************************************************************************/
void buildDatabase(string filename, BinarySearchTree<Part*>* IdTree,
	BinarySearchTree<Part*>* ProducerTree,
	HashTable<Part*, string>* hash)
{
	ifstream inputFile;
	inputFile.open(filename);
	if (!inputFile)
	{
		cout << "ERROR:Fail to open the file\n";
		exit(1);
	}
	cout << "Open file successfully\n";
	Sleep(100);

	while (!inputFile.eof())
	{
		printTitle("Loading...");
		Part* newPart = new Part(inputFile);
		cout << *newPart;
		IdTree->insert(newPart, compareID);
		ProducerTree->insert(newPart, compareProducer);
		if (!(hash->insertHash(newPart, HASHING_ALGORITHM, newPart->getId(), compareID)))
		{
			cout << "\nLoad factor is too high\n";
			cout << "The hash table is automatically rehashed\n" << endl;
			printStat(hash);
			hold();
		}
		Sleep(20);
	}

	cout << "Database Initialized...\n";
	hold();
	inputFile.close();

}


/*********************************************************************
 Test Driver

 **********************************************************************/
void testDriver(BinarySearchTree<Part*>* idTree, BinarySearchTree<Part*>* producerTree, HashTable<Part*, string>* hashTab, Stack <Part*>* stack)
{
	string cache;
	char choice;

	showMenu();
	getline(cin, cache);
	if (cache.length() > 1)
		return testDriver(idTree, producerTree, hashTab, stack);
	choice = cache[0];

	switch (toupper(choice))
	{
	case '1':
		insertOperation(idTree, producerTree, hashTab);
		return testDriver(idTree, producerTree, hashTab, stack);
		break;

	case '2':
		deleteOperation(idTree, producerTree, hashTab, stack);
		return testDriver(idTree, producerTree, hashTab, stack);
		break;
	case '3':
		unDeleteOperation(idTree, producerTree, hashTab, stack);
		return testDriver(idTree, producerTree, hashTab, stack);
		break;

	case '4':
		searchOperation(producerTree, hashTab);
		return testDriver(idTree, producerTree, hashTab, stack);
		break;

	case '5':
		listOperation(idTree, producerTree, hashTab);
		return testDriver(idTree, producerTree, hashTab, stack);
		break;

	case '6':
		saveToFile(idTree);
		hold();
		return testDriver(idTree, producerTree, hashTab, stack);
		break;

	case '7':
		printStat(hashTab);
		hold();
		return testDriver(idTree, producerTree, hashTab, stack);
		break;
	case '8':
		saveToFile(idTree);
		return;
		break;

	default:
		return testDriver(idTree, producerTree, hashTab, stack);
		break;

	}

}

/*********************************************************************
 Insert Operation

 **********************************************************************/
void insertOperation(BinarySearchTree<Part*>* obj1, BinarySearchTree<Part*>* obj2, HashTable<Part*, string>* obj3)
{
	string id = "";
	Part* returnItem = new Part;

	printTitle("Add New Data");

	Part* newItem = new Part;
	cout << "ID: ";
	getline(cin, id);
	newItem->setId(id);
	if (obj3->searchHash(newItem, newItem->getId(), HASHING_ALGORITHM, returnItem, compareID))
	{
		cout << "ERROR: Duplicates Found. Failed to add the item.\n" << endl;
		hold();
		return;
	}
	else {
		cin >> *newItem;
	}

	cout << endl;

	printTag();
	display(newItem);

	cout << endl;
	if (confirm())
	{
		obj1->insert(newItem, compareID);
		obj2->insert(newItem, compareProducer);
		obj3->insertHash(newItem, HASHING_ALGORITHM, newItem->getId(), compareID);
		cout << endl << newItem->getId() << " Added\n";
	}

	hold();
}

/*********************************************************************
 Search Operation

 **********************************************************************/
void searchOperation(BinarySearchTree<Part*>* obj2, HashTable<Part*, string>* obj3)
{
	string cache;
	char choice;

	searchMenu();
	getline(cin, cache);
	if (cache.length() > 1)
		return searchOperation(obj2, obj3);
	choice = cache[0];

	string target = "";
	Part* newItem = new Part;
	Part* returnItem = new Part;

	cout << endl;
	switch (toupper(choice))
	{
	case '1':
		cout << "Target ID: ";
		getline(cin, target);
		newItem->setId(target);
		printTag();
		if (obj3->searchHash(newItem, newItem->getId(), HASHING_ALGORITHM, returnItem, compareID))
			display(returnItem);
		else
		{
			cout << "\nItem not found" << endl;
		}
		cout << endl;
		hold();
		return searchOperation(obj2, obj3);
		break;

	case '2':
		cout << "Target Producer: ";
		getline(cin, target);
		newItem->setProducer(target);
		printTag();
		if (obj2->getEntry(newItem, returnItem, compareProducer, display))
			;
		else
		{
			cout << "\nItem not found" << endl;
		}
		cout << endl;
		hold();
		return searchOperation(obj2, obj3);
		break;

	case '3':
		return;
		break;

	default:
		return searchOperation(obj2, obj3);
		break;
	}
}

/*********************************************************************
 Delete Operation

 **********************************************************************/
void deleteOperation(BinarySearchTree<Part*>* obj1, BinarySearchTree<Part*>* obj2, HashTable<Part*, string>* obj3, Stack <Part*>*& stack)
{
	string cache;
	char choice;

	deleteMenu();
	getline(cin, cache);
	if (cache.length() > 1)
		return deleteOperation(obj1, obj2, obj3, stack);
	choice = cache[0];

	string target = "";
	Part* tartItem = new Part;
	Part* returnItem = new Part;

	cout << endl;
	switch (toupper(choice))
	{
	case '1':
		cout << "Target ID: ";
		getline(cin, target);
		tartItem->setId(target);

		if (obj3->searchHash(tartItem, tartItem->getId(), HASHING_ALGORITHM, returnItem, compareID))
		{
			printTag();
			display(returnItem);

			cout << endl;
			if (confirm())
			{
				stack->push(returnItem);
				obj1->remove(tartItem, compareID);
				obj2->remove(tartItem, compareID);
				obj3->deleteHash(tartItem, tartItem->getId(), HASHING_ALGORITHM, compareID);
				cout << endl << tartItem->getId() << " Deleted\n";
			}
		}
		else
		{
			cout << "Item not found\n\n";
		}

		hold();

		return deleteOperation(obj1, obj2, obj3, stack);
		break;
	case '2':
		return;
		break;

	default:
		return deleteOperation(obj1, obj2, obj3, stack);
		break;
	}
}

/*********************************************************************
 List Operation

 **********************************************************************/
void listOperation(BinarySearchTree<Part*>* obj1, BinarySearchTree<Part*>* obj2, HashTable<Part*, string>* obj3)
{
	string cache;
	char choice;

	listMenu();
	getline(cin, cache);
	if (cache.length() > 1)
		return listOperation(obj1, obj2, obj3);
	choice = cache[0];
	cout << endl;

	switch (choice)
	{
	case '1':
		printTag();
		obj3->printHash(display);
		cout << endl;
		hold();
		return listOperation(obj1, obj2, obj3);
		break;

	case '2':
		printTag();
		obj1->inOrder(display);
		cout << endl;
		hold();
		return listOperation(obj1, obj2, obj3);
		break;

	case '3':
		printTag();
		obj2->inOrder(display);
		cout << endl;
		hold();
		return listOperation(obj1, obj2, obj3);
		break;

	case '4':

		cout << "\nID Tree: \n\n";
		obj1->printTree(displayID);
		cout << "\nProducer Tree: \n\n";
		obj2->printTree(displayProducer);
		hold();
		return listOperation(obj1, obj2, obj3);
		break;

	case '5':
		return;
		break;

	default:
		return listOperation(obj1, obj2, obj3);
		break;
	}
}

/*********************************************************************
 Undelete Operation

 **********************************************************************/
void unDeleteOperation(BinarySearchTree<Part*>* obj1, BinarySearchTree<Part*>* obj2, HashTable<Part*, string>* obj3, Stack <Part*>* stack) {
	printTitle("Undelete");
	Part* returnItem = new Part();
	if (!stack->isEmpty())
	{
		stack->pop(returnItem);
		printTag();
		display(returnItem);

		cout << endl;
		if (confirm())
		{
			obj1->insert(returnItem, compareID);
			obj2->insert(returnItem, compareProducer);
			obj3->insertHash(returnItem, HASHING_ALGORITHM, returnItem->getId(), compareID);
			cout << endl << returnItem->getId() << " Restored\n";
		}
		else
			stack->push(returnItem);
	}
	else
	{
		cout << "Nothing to restore\n\n";
	}

	hold();
}



/*********************************************************************
 Function pointers
 **********************************************************************/
int compareID(Part*& obj1, Part*& obj2)
{
	if (obj1->getId() < obj2->getId())
		return -1;
	else if (obj1->getId() > obj2->getId())
		return 1;
	else
		return 0;
}

int compareProducer(Part*& obj1, Part*& obj2)
{
	if (obj1->getPro() < obj2->getPro())
		return -1;
	else if (obj1->getPro() > obj2->getPro())
		return 1;
	else
		return 0;
}

void display(Part*& part)
{
	cout << left << setw(10) << part->getId()
		<< left << setw(43) << part->getName()
		<< left << setw(23) << part->getCat()
		<< left << setw(10) << part->getPro()
		<< left << setw(10) << part->getPrice()
		<< left << setw(10) << part->getQuan() << part->getMonth() << endl;
}


void outputDisplay(Part*& part) 
{
	ofstream outFile(outputFile, ios::app);
	cout << part->getId() << ' ' << part->getName() << "; " << part->getCat() << "; "
		<< part->getPrice() << ' ' << part->getPrice() << ' ' << part->getQuan()
		<< ' ' << part->getMonth() << endl;

	outFile << part->getId() << ' ' << part->getName() << "; " << part->getCat() << "; "
		<< part->getPrice() << ' ' << part->getPrice() << ' ' << part->getQuan()
		<< ' ' << part->getMonth() << endl;
	outFile.close();
}

void displayID(Part*& part)
{
	cout << part->getId() << endl;
}

void displayProducer(Part*& part)
{
	cout << part->getPro() << endl;
}

int hashing_HC(string key)
{
	int index = 0;
	for (int i = 2; i < key.length(); i++)
	{
		index += key[i];
		index -= 48;
		index *= 10;
	}
	return index;
}

int hashing_LC(string key)
{
	int index = 0;
	for (int i = 0; i < key.length(); i++)
		index += key[i];

	pow(index, 11);
	index << 3;
	return index;
}

/*********************************************************************
 Other Functions
 **********************************************************************/

void listMenu()
{
	printTitle("    List");
	cout << "1. Unsorted Data\n"
		<< "2. Sorted by ID\n"
		<< "3. Sorted by Producer\n"
		<< "4. Print as Indented Tree\n"
		<< "5. Back\n";
	cout << "Please enter your option: ";
}

void printTitle(string subtitle)
{
	system("CLS");
	cout << "===========================================================\n"
		<< "\t\tLaptop Manufacture Database\n"
		<< "\t\t\t" << subtitle << endl
		<< "===========================================================\n";
}

void showMenu()
{
	printTitle("Main Menu");
	cout << "1. Add New Data\n"
		<< "2. Delete Data\n"
		<< "3. Undelete\n"
		<< "4. Search\n"
		<< "5. Display\n"
		<< "6. Save to file\n"
		<< "7. Statistics\n"
		<< "8. Quit\n";
	cout << "Please enter your option: ";
}

void hold()
{
	cout << "Press <ENTER> to continue...\n";
	cin.ignore(10000, '\n');
}

void printTag()
{
	cout << "======================================================================================================================\n";
	cout << left << setw(10) << "ID"
		<< left << setw(43) << "NAME"
		<< left << setw(23) << "CATEGORY"
		<< left << setw(10) << "PRODUCER"
		<< left << setw(10) << "PRICE"
		<< left << setw(10) << "QUANTITY" << "MONTH" << endl;
	cout << "======================================================================================================================\n";
}

bool confirm()
{
	cout << "Confirm? <Y/N> ";
	string choice;
	getline(cin, choice);
	if (choice.length() > 1)
		return confirm();
	if (toupper(choice[0]) == 'Y')
		return true;
	if (toupper(choice[0]) == 'N')
		return false;
	else
		return confirm();
}

void searchMenu()
{
	printTitle("   Search");
	cout << "1. Search by ID\n"
		<< "2. Search by Producer\n"
		<< "3. Back\n";
	cout << "Please enter your option: ";
}

void deleteMenu()
{
	printTitle("   Delete");
	cout << "1. Delete by ID\n"
		<< "2. Back\n";
	cout << "Please enter your option: ";
}

void saveToFile(BinarySearchTree<Part*>* tree)
{
	ofstream outFile(outputFile, ios::trunc);
	outFile.close();
	cout << endl;
	tree->inOrder(outputDisplay);
	cout << "\nDatabase saved to file\n";
}

void printStat(HashTable<Part*, string>* obj)
{
	cout << "\n---------- Hash Table Statistics ----------" << endl;
	cout << "Size: " << obj->getCount() << endl;
	cout << "Collisions: " << obj->getCollision() << endl;
	cout << "Load Factor: " << obj->loadFactor() << endl;
	cout << "------------------------------------------" << endl;
}