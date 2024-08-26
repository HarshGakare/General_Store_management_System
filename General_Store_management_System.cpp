#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>  // For exit function
using namespace std;

const string DATA_FILE_NAME = "data.txt";
const string STORE_FILE_NAME = "store.txt";

class Store {
    string item_ID, item_Name;
    int item_Quantity, item_Price, total_Amount = 0, Quantity, item_rate;
    fstream file, outFile;  // Declare outFile here
    string search;
    char choices;

public:
    void add_Product();
    void buy_product();
    void view_Product();
};

int main() {
    char choice;
    Store S;
    cout << "-----------Welcome to General Store------------\n";

    while (true) {
        cout << "Press A --> Admin Panel\n";
        cout << "Press S --> Start Shopping\n";
        cout << "Press E --> Exit\n";
        cin >> choice;

        switch (choice) {
            case 'A':
                S.add_Product(); 
                break;
            case 'S':
                S.view_Product();
                S.buy_product();
                break;
            case 'E':
                return 0;
            default:
                cout << "Invalid selection. Please try again." << endl;
                break;
        }
    }
}

void Store::add_Product() {
    char choose = 'Y';
    while (choose == 'Y') {
        cout << "Enter Product ID : ";
        cin >> item_ID;
        cout << "Enter Product Name : ";
        cin >> item_Name;
        cout << "Enter Product Quantity : ";
        cin >> item_Quantity;
        cout << "Enter Product Price : ";
        cin >> item_Price;

        file.open(DATA_FILE_NAME, ios::out | ios::app);
        if (!file.is_open()) {
            cerr << "Error opening file: " << DATA_FILE_NAME << endl;
            exit(EXIT_FAILURE);
        }
        file << item_ID << "\t" << item_Name << "\t" << item_Quantity << "\t" << item_Price << endl;
        file.close();
        
        cout << "\nContinue adding products?\nPress Y --> Yes\nPress N --> No" << endl;
        cin >> choose;
    }

    char view;
    cout << "\nPress V --> View the Product List\nPress E --> Exit\n";
    cin >> view;
    if (view == 'V') {
        view_Product();
    }
}

void Store::view_Product() {
    file.open(DATA_FILE_NAME, ios::in);
    if (!file.is_open()) {
        cerr << "Error opening file: " << DATA_FILE_NAME << endl;
        exit(EXIT_FAILURE);
    }

    // Print column headers
    cout << "Product ID\tProduct Name\tQuantity\tPrice" << endl;

    while (file >> item_ID >> item_Name >> item_Quantity >> item_Price) {
        // Display product information
        cout << item_ID << "\t\t" << item_Name << "\t\t" << item_Quantity << "\t\t" << item_Price << endl;
    }

    file.close();
}

void Store::buy_product() {
    choices = 'Y';
    while (choices == 'Y') {
        file.open(DATA_FILE_NAME, ios::in);
        if (!file.is_open()) {
            cerr << "Error opening file: " << DATA_FILE_NAME << endl;
            exit(EXIT_FAILURE);
        }

        outFile.open( STORE_FILE_NAME, ios::out | ios::app);  // Use outFile for writing
        if (!outFile.is_open()) {
            cerr << "Error opening file: " << STORE_FILE_NAME << endl;
            exit(EXIT_FAILURE);
        }

        cout << "Enter Product ID to purchase: ";
        cin >> search;

        bool productFound = false;
        while (file >> item_ID >> item_Name >> item_Quantity >> item_Price) {
            if (item_ID == search) {
                productFound = true;
                cout << "Enter quantity to purchase: ";
                cin >> Quantity;

                if (Quantity > item_Quantity) {
                    cout << "Not enough quantity in stock!" << endl;
                    // Update inventory with the original quantity if the purchase fails
                    outFile << item_ID << "\t" << item_Name << "\t" << item_Quantity << "\t" << item_Price << endl;
                } else {
                    item_Quantity -= Quantity;
                    item_rate = item_Price * Quantity;
                    total_Amount += item_rate;

                    outFile << item_ID << "\t" << item_Name << "\t" << item_Quantity << "\t" << item_Price << endl;

                    cout << "-----------Payment Bill----------" << endl;
                    cout << "Total Purchase Amount: Rs." << total_Amount << endl;
                }
            } else {
                outFile << item_ID << "\t" << item_Name << "\t" << item_Quantity << "\t" << item_Price << endl;
            }
        }

        if (!productFound) {
            cout << "Item ID not found." << endl;
        }

        file.close();
        outFile.close();

        // Replace the old data file with the updated store file
        if (remove(DATA_FILE_NAME.c_str()) != 0) {
            cerr << "Error deleting file: " << DATA_FILE_NAME << endl;
            exit(EXIT_FAILURE);
        }

        if (rename(STORE_FILE_NAME.c_str(), DATA_FILE_NAME.c_str()) != 0) {
            cerr << "Error renaming file: " << STORE_FILE_NAME << endl;
            exit(EXIT_FAILURE);
        }

        cout << "\nContinue shopping?\nPress Y --> Yes\nPress N --> No" << endl;
        cin >> choices;
    }
}
