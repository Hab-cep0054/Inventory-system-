#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>

#ifdef _WIN32
    #include <conio.h> // For Windows
#elif defined(__unix__) || defined(__APPLE__)
    #include <termios.h> // For Unix-like systems
    #include <unistd.h>
#else
    #error Unsupported platform
#endif

char getInput() {
    char key;
#ifdef _WIN32
    key = _getch();
#elif defined(__unix__) || defined(__APPLE__)
    struct termios oldTermios, newTermios;
    tcgetattr(STDIN_FILENO, &oldTermios);
    newTermios = oldTermios;
    newTermios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newTermios);

    key = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldTermios);
#else
    // Error: Unsupported platform
    throw std::runtime_error("Unsupported platform");
#endif
    return key;
}


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
  getInput();
  system("cls");
  intro();
	}

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
  getInput();
  system("cls");
  intro();
	}

	void removeProduct()
	{
			system("cls");
			displayInventory();
		string itemName;
		cout << "Enter product name you want to remove: ";
		cin >> itemName;

		Stock item = findItem(itemName);

		if (item.stockId.empty())
		{
			cout << "Product not found in inventory." << endl;
			return;
		}

		removeItem(item);

		cout << "Product removed successfully." << endl;
		displayInventory();
		cout << "\n\n\n Enter any key\n: ";
		cin.ignore();
  getInput();
  system("cls");
  intro();
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

  private:
	string generateStockId()
	{
		srand(time(NULL));
		string stockId = "A" + to_string(rand() % 900 + 100);
		return stockId;
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

	void removeItem(const Stock &item)
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
		}

		file.close();
		tempFile.close();

		remove("inventory.csv");
		rename("temp.csv", "inventory.csv");
	}

	time_t parseDateTime(const string &dateTimeStr)
	{
		struct tm tm;
		strptime(dateTimeStr.c_str(), "%a %b %d %H:%M:%S %Y", &tm);
		return mktime(&tm);
	}

	void displayInventory()
	{
		availableStockList();
	}
}inventory;

int main()
{
	
	cout << "WELCOME TO INVENTORY SYSTEM\n\n\n";

cout << "Enter any key: ";
  getInput();
intro();

	return 0;
}
void intro(){
	system ("cls");
	cout << "1. Add New Product" << endl;
	cout << "2. Fill Product" << endl;
	cout << "3. Remove Product" << endl;
	cout << "4. Available Stock List" << endl;
	cout << "5. Exit " << endl;
	cout << "Enter your choice: ";

	int choice;
	cin >> choice;

	switch (choice)
	{
	case 1:
		inventory.addNewProduct();
		break;
	case 2:
		inventory.fillProduct();
		break;
	case 3:
		inventory.removeProduct();
		break;
	case 4:
	system("cls");
		inventory.availableStockList();
		break;
	case 5:
		exit(1);
		break;
	default:
		cout << "Invalid choice." << endl;
	}}