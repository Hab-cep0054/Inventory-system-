#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>



using namespace std;
void intro();
class Inventory
{
  private:
	struct StockItem
	{
		string itemName;
		double price;
		int quantity;
	};
	struct Stock
	{
		string stockId;
		time_t dateTime;
		StockItem item;
	};
public:
	void fillProduct()
	{system ("cls");
		displayInventory();
		string itemName;
		cout << "Enter product name: ";
		cin >> itemName;

		Stock item = findItem(itemName);

		if (item.stockId.empty())
		{
			cout << "Product not found in inventory." << endl;
			return;
		}

		cout << "Enter quantity to add: ";
		int quantity;
		cin >> quantity;
		item.item.quantity += quantity;

		cout << "Enter price: ";
		cin >> item.item.price;

		updateItem(item);

		cout << "Product filled successfully." << endl;
		displayInventory();
		
			cout << "\n\n\n Enter any key\n: ";
		cin.ignore();
  cin.get();
  system("cls");
  intro();
	}

	Stock findItem(const string &itemName)
	{
		ifstream file("inventory.csv");

		Stock item;
		item.stockId = "";
		string stockId, dateTime, name, quantity, price;

		while (getline(file, stockId, ','))
		{
			getline(file, dateTime, ',');
			getline(file, name, ',');
			getline(file, quantity, ',');
			getline(file, price);

			if (name == itemName)
			{
				item.stockId = stockId;
				item.dateTime = parseDateTime(dateTime);
				item.item.itemName = name;
				item.item.quantity = stoi(quantity);
				item.item.price = stod(price);
				break;
			}
		}

		file.close();
		return item;
	}

	void updateItem(const Stock &item)
	{
		ifstream file("inventory.csv");
		ofstream tempFile("temp.csv");

		string stockId, dateTime, name, quantity, price;

		while (getline(file, stockId, ','))
		{
			getline(file, dateTime, ',');
			getline(file, name, ',');
			getline(file, quantity, ',');
			getline(file, price);

			if (stockId != item.stockId)
			{
				tempFile << stockId << "," << dateTime << "," << name << "," << quantity << "," << price << endl;
			}
			else
			{
				tempFile << item.stockId << "," << ctime(&item.dateTime) << "," << item.item.itemName << ","
						 << item.item.quantity << "," << item.item.price << endl;
			}
		}

		file.close();
		tempFile.close();

		remove("inventory.csv");
		rename("temp.csv", "inventory.csv");
	}



	void displayInventory()
	{
		availableStockList();
	}
void availableStockList()
	{
		ifstream file("inventory.csv");
		if (!file)
		{
			cout << "Inventory file not found." << endl;
			return;
		}

		cout << "Stock ID\tDate & Time\t\tItem Name\tQuantity\tPrice" << endl;
		cout << "--------------------------------------------------------" << endl;

		string stockId, dateTime, itemName, quantity, price;

		while (getline(file, stockId, ','))
		{
			getline(file, dateTime, ',');
			getline(file, itemName, ',');
			getline(file, quantity, ',');
			getline(file, price);

			cout << stockId << "\t\t" << dateTime << "\t" << itemName << "\t\t" << quantity << "\t\t" << price << endl;
		}

		file.close();
	}
	

	time_t parseDateTime(const string &dateTimeStr)
	{
		struct tm tm;
		strptime(dateTimeStr.c_str(), "%a %b %d %H:%M:%S %Y", &tm);
		return mktime(&tm);
	}
}inventory;

int main()
{
	
	cout << "WELCOME TO INVENTORY SYSTEM\n\n\n";

cout << "Enter any key: ";
  cin.get();
intro();

	return 0;
}


void intro(){
	cout << "1. Fill Product" << endl;
	cout << "2. Available Stock List" << endl;
	cout << "3. Exit " << endl;
	cout << "Enter your choice: ";
int choice;
	cin >> choice;

	switch (choice)
	{
	case 1:
		inventory.fillProduct();
		break;
	case 2:
		inventory.availableStockList();
		break;
			case 3:
		exit(1);
		break;
	default:
		cout << "Invalid choice." << endl;
	}}