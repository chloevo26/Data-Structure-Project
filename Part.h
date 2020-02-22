// Part Class
// Item class for the database
// Developer: Quan
// 

#ifndef PART_H_
#define PART_H_

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class Part
{
private:
	string partId;
	string partName;
	string partCategory;
	string partProducer;
	double partPrice;
	int partQuantity;
	string importMonth;

public:

	//Constructor
	Part();
	Part(string id, string name, string cat, string pro, double price, int quan, string month);
	Part(istream&);

	//Setters
	void setId(string partId) { this->partId = partId; }
	void setName(string partName) { this->partName = partName; }
	void setCat(string partCategory) { this->partCategory = partCategory; }
	void setProducer(string partProducer) { this->partProducer = partProducer; }
	void setPrice(double partPrice) { this->partPrice = partPrice; }
	void setQuantity(int partQuantity) { this->partQuantity = partQuantity; }
	void setMonth(string importMonth) { this->importMonth = importMonth; }

	//Getters
	string getId() const { return partId; }
	string getName() const { return partName; }
	string getCat() const { return partCategory; }
	string getPro() const { return partProducer; }
	double getPrice() const { return partPrice; }
	int getQuan() const { return partQuantity; }
	string getMonth() const { return importMonth; }

	// Operators

	// Overload < operator
	friend bool operator < (const Part& obj1, const Part& obj2);

	// Overload > operator
	friend bool operator > (const Part& obj1, const Part& obj2);

	// Overload == operator
	friend bool operator == (const Part& obj1, const Part& obj2);

	// Overload << operator
	friend ostream& operator << (ostream& out, const Part& part);

	// Overload >> operator
	friend istream& operator >> (istream& strm, Part& obj);

	//Destructor
	~Part() {};
};


Part::Part()
{
	partId = "";
	partName = "";
	partCategory = "";
	partProducer = "";
	partPrice = 0;
	partQuantity = 0;
	importMonth = "";
}

Part::Part(string id, string name, string cat, string pro, double price, int quan, string month)
{
	partId = id;
	partName = name;
	partCategory = cat;
	partProducer = pro;
	partPrice = price;
	partQuantity = quan;
	importMonth = month;
}

Part::Part(istream& in)
{
	getline(in, partId, ' ');
	getline(in, partName, ';');
	in.ignore();
	getline(in, partCategory, ';');
	in.ignore();
	getline(in, partProducer, ' ');
	if (!(in >> partPrice))
	{
		partPrice = 0.0;
		cout << "ERROR:Invalid Input!\n";
	}
	if (!(in >> partQuantity))
	{
		partPrice = 0.0;
		cout << "ERROR:Invalid Input!\n";
	}
	in.ignore();
	getline(in, importMonth);
}

ostream& operator << (ostream& out, const Part& part)
{
	out << part.partId << ' ' << part.partName << "; " << part.partCategory << "; "
		<< part.partProducer << ' ' << part.partPrice << ' ' << part.partQuantity
		<< ' ' << part.importMonth << endl;

	return out;
}

istream& operator >> (istream& strm, Part& obj)
{
	cout << "Name: ";
	getline(strm, obj.partName);
	cout << "Category: ";
	getline(strm, obj.partCategory);
	cout << "Producer: ";
	getline(strm, obj.partProducer);
	cout << "Price: ";
	double price;
	strm >> price;
	while (strm.fail() || price <= 0)
	{
		strm.clear();
		strm.ignore(1000, '\n');
		cout << "ERROR:Invalid Input! Try again: ";
		strm >> price;
	}
	obj.setPrice(price);
	cout << "Quantity: ";
	int quantity;
	strm >> quantity;
	while (strm.fail() || quantity < 0)
	{
		strm.clear();
		strm.ignore(1000, '\n');
		cout << "ERROR:Invalid Input! Try again: ";
		strm >> quantity;
	}
	obj.setQuantity(quantity);
	cout << "Month: ";
	strm >> obj.importMonth;
	strm.ignore();

	return strm;

}

bool operator == (const Part& obj1, const Part& obj2)
{
	return (obj1.partId == obj2.partId);
}

bool operator < (const Part& obj1, const Part& obj2)
{
	return (obj1.partId < obj2.partId);
}

bool operator > (const Part& obj1, const Part& obj2)
{
	return (obj1.partId > obj2.partId);
}


#endif