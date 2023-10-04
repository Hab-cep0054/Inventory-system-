#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
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
	void addNewProduct()
	{system("cls");
	
	cout<<"enter "<<quoted("done")<<" to stop\n\n\n";
	while(true){
		Stock newItem;
		cout << "Enter stock item name: ";
		cin >> newItem.item.itemName;
			if(newItem.item.itemName=="done"){
	break;		}
		cout << "Enter quantity: ";
		cin >> newItem.item.quantity;
		cout << "Enter price: ";
		cin >> newItem.item.price;

		time(&newItem.dateTime);
		newItem.stockId = generateStockId();

		saveToSpreadsheet(newItem);

		cout << "You have successfully added "<<newItem.item.itemName<<" items to inventory." << endl;
	}
	system("cls");
		displayInventory();
		cout << "\n\n\n Enter any key\n: ";
		cin.ignore();
  cin.get();
  system("cls");
  intro();
	}
		void displayInventory()
	{
		availableStockList();
	}
		void saveToSpreadsheet(const Stock &item)
	{
		ofstream file("inventory.csv", ios::app);

		if (!file)
		{
			cout << "Error opening inventory file." << endl;
			return;
		}

		file << item.stockId << "," << ctime(&item.dateTime) << "," << item.item.itemName << ","
			 << item.item.quantity << "," << item.item.price << endl;

		file.close();
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
		string generateStockId()
	{
		srand(time(NULL));
		string stockId = "A" + to_string(rand() % 900 + 100);
		return stockId;
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
	system ("cls");
	cout << "1. Add New Product" << endl;
	cout << "2. Available Stock List" << endl;
	cout << "Enter your choice: ";

	int choice;
	cin >> choice;

	switch (choice)
	{
	case 1:
		inventory.addNewProduct();
		break;
	case 2:
	inventory.availableStockList();
	break;
	default:
		cout << "Invalid choice." << endl;
	}}
