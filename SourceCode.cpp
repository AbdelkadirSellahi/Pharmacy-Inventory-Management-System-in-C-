#include <iostream>
#include <string>
#include <fstream>
using namespace std;

// Structure for date
struct Date {
    int day;
    int month;
    int year;
};

// Structure for medication
struct Medication {
    string name;
    double price;
    double quantity;
    Date manufactureDate;
    Date expiryDate;
};

// Structure for list node
struct Node {
    Medication data;
    Node* next;
};
typedef Node* List; // Define type List

// Insert a new node into the list
void insertNode(List* L, Medication value) {
    List newNode;
    Node* temp;
    newNode = new Node;
    bool exists = false;
    newNode->data = value;
    newNode->next = NULL;
    if (*L == NULL) {
        *L = newNode;
    } else {
        for (temp = *L; temp != NULL; temp = temp->next) {
            if (temp->data.name == value.name) {
                temp->data.quantity += value.quantity;
                exists = true;
            }
        }
        if (!exists) {
            newNode->next = *L;
            *L = newNode;
        }
    }
}

// Display the list
void display(List L) {
    List current = L;
    while (current != NULL) {
        cout << "Name: " << current->data.name << endl;
        cout << "Quantity: " << current->data.quantity << endl;
        cout << "Price: " << current->data.price << endl;
        cout << "Manufacture Date: " << current->data.manufactureDate.day << "-"
             << current->data.manufactureDate.month << "-" << current->data.manufactureDate.year << endl;
        cout << "Expiry Date: " << current->data.expiryDate.day << "-"
             << current->data.expiryDate.month << "-" << current->data.expiryDate.year << endl;
        cout << "----------------" << endl;
        current = current->next;
    }
}

// Consult the price and quantity of a medication by name
void consultPriceAndQuantity(List L) {
    bool found = false;
    string name;
    cout << "Enter the medication name: ";
    cin >> name;
    for (List current = L; current != NULL; current = current->next) {
        if (current->data.name == name) {
            cout << "Price: " << current->data.price << endl;
            cout << "Quantity: " << current->data.quantity << endl;
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "This medication does not exist." << endl;
    }
}

// Compare two dates
bool compareDates(Date date1, Date date2) {
    if (date2.year > date1.year) {
        return true;
    } else if (date2.year < date1.year) {
        return false;
    } else { // Same year
        if (date2.month > date1.month) {
            return true;
        } else if (date2.month < date1.month) {
            return false;
        } else { // Same month
            return date2.day > date1.day;
        }
    }
}

// Display medications expiring before a given date
void displayExpiringBefore(List L) {
    Date givenDate;
    bool found = false;
    cout << "Enter the date (day month year): ";
    cin >> givenDate.day >> givenDate.month >> givenDate.year;
    for (List current = L; current != NULL; current = current->next) {
        if (!compareDates(givenDate, current->data.expiryDate)) {
            cout << "Medication: " << current->data.name << " is expiring before the given date." << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "There are no medications expiring before the given date." << endl;
    }
}

// Display medications with quantity below a certain threshold
void displayAtRiskMedications(List L) {
    double threshold;
    bool found = false;
    cout << "Enter the quantity threshold: ";
    cin >> threshold;
    for (List current = L; current != NULL; current = current->next) {
        if (current->data.quantity <= threshold) {
            cout << "Medication: " << current->data.name << " is at risk." << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "There are no medications at risk." << endl;
    }
}

// Delete a medication or reduce its quantity
void deleteOrUpdateNode(List* L, string name, double quantity) {
    Node** current = L;
    while (*current != NULL) {
        if ((*current)->data.name == name) {
            if ((*current)->data.quantity > quantity) {
                (*current)->data.quantity -= quantity;
            } else {
                Node* temp = *current;
                *current = (*current)->next;
                delete temp;
            }
            return;
        }
        current = &(*current)->next;
    }
    cout << "Medication not found." << endl;
}

// Sort the list alphabetically by medication name
void sortList(List* L) {
    if (*L == NULL || (*L)->next == NULL) {
        return; // List is empty or has one element
    }
    bool swapped;
    do {
        swapped = false;
        Node** current = L;
        while ((*current)->next != NULL) {
            if ((*current)->data.name > (*current)->next->data.name) {
                Medication temp = (*current)->data;
                (*current)->data = (*current)->next->data;
                (*current)->next->data = temp;
                swapped = true;
            }
            current = &(*current)->next;
        }
    } while (swapped);
}

// Main program
int main() {
    int choice;
    Medication value;
    List L = NULL;
    double totalStockValue = 0;

    do {
        cout << "----------Menu----------" << endl;
        cout << "1. Sell a medication" << endl;
        cout << "2. Restock a medication" << endl;
        cout << "3. Display all medications" << endl;
        cout << "4. Consult price and quantity of a medication" << endl;
        cout << "5. Display total stock value" << endl;
        cout << "6. Display medications expiring before a given date" << endl;
        cout << "7. Display at-risk medications" << endl;
        cout << "0. Exit the program" << endl;
        cout << "------------------------" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cout << "------------------------" << endl;

        // Implementation of switch cases
        switch (choice) {
            case 1: { // Code for selling a medication
                string medName;
                double quantity;
                cout << "Enter the name of the medication to sell: ";
                cin >> medName;
                cout << "Enter the quantity to sell: ";
                cin >> quantity;
                deleteOrUpdateNode(&L, medName, quantity);
                break;
            }
            case 2: { // Code for restocking a medication
                cout << "Enter medication details to restock:" << endl;
                cout << "Name: ";
                cin >> value.name;
                cout << "Quantity: ";
                cin >> value.quantity;
                cout << "Price: ";
                cin >> value.price;
                cout << "Manufacture Date (day month year): ";
                cin >> value.manufactureDate.day >> value.manufactureDate.month >> value.manufactureDate.year;
                cout << "Expiry Date (day month year): ";
                cin >> value.expiryDate.day >> value.expiryDate.month >> value.expiryDate.year;
                if (!compareDates(value.manufactureDate, value.expiryDate)) {
                    cout << "Error: Expiry date must be after manufacture date." << endl;
                    break;
                }
                insertNode(&L, value);
                totalStockValue += value.price * value.quantity;
                sortList(&L);
                cout << "Medication restocked and sorted alphabetically." << endl;
                break;
            }
            case 3: { // Code for displaying all medications
                if (L != NULL) {
                    cout << "Displaying all medications:" << endl;
                    display(L);
                } else {
                    cout << "The medication list is empty." << endl;
                }
                break;
            }
            case 4: { // Code for consulting price and quantity
                consultPriceAndQuantity(L);
                break;
            }
            case 5: { // Code for displaying total stock value
                cout << "Total stock value: " << totalStockValue << " DA" << endl;
                break;
            }
            case 6: { // Code for displaying expiring medications
                displayExpiringBefore(L);
                break;
            }
            case 7: { // Code for displaying at-risk medications
                displayAtRiskMedications(L);
                break;
            }
            case 0:
                cout << "Exiting the program." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 0);

    return 0;
}

