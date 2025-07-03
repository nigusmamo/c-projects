#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;
using namespace chrono;

struct Contact {
    string firstName;
    string lastName;
    string phoneNumber;
    string email;
    Contact* next;
};

Contact* head = nullptr;

void loadFromFile();
void saveToFile();
bool isDuplicate(string phone);
void addContact();
void addContactAtBeginning();
void addContactAtEnd();
void addContactAtPosition();
void deleteContact();
void viewContacts();
void searchContact();
void sortContactsMenu();
void bubbleSort();
void insertionSort();
void selectionSort();
int menu();
int compareContacts(Contact* a, Contact* b);
void manualSwapStrings(string& s1, string& s2);
void swapContactData(Contact* a, Contact* b);

int main() {
    loadFromFile();

    int choice;
    while ((choice = menu()) != 6) {
        switch (choice) {
            case 1: 
            addContact(); 
            break;
            case 2: 
            deleteContact(); 
            break;
            case 3: 
            searchContact(); 
            break;
            case 4: 
            sortContactsMenu(); 
            break;
            case 5: 
            viewContacts(); 
            break;
            default: 
                cout << "Invalid choice!\n";
        }
    }

    saveToFile();

    Contact* current = head;
    while (current) {
        Contact* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
    return 0;
}

int menu() {
    cout << "\n==== Contact Management System ====" << endl;
    cout << "1. Add Contact" << endl;
    cout << "2. Delete Contact" << endl;
    cout << "3. Search Contact" << endl;
    cout << "4. Sort Contacts" << endl;
    cout << "5. View All Contacts" << endl;
    cout << "6. Exit" << endl;
    cout << "Enter choice: ";
    int ch;
    cin >> ch;
    cin.ignore();
    return ch;
}

void loadFromFile() {
    ifstream fin("contacts.txt");
    if (!fin.is_open()) {
        cout << "contacts.txt not found. Starting with an empty list.\n";
        return;
    }

    Contact* current = head;
    while (current) {
        Contact* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;

    string fname, lname, phone, email;
    Contact* tail = nullptr;

    while (getline(fin, fname, ',') && getline(fin, lname, ',') && getline(fin, phone, ',') && getline(fin, email)) {
        Contact* newContact = new Contact{fname, lname, phone, email, nullptr};

        if (head == nullptr) {
            head = newContact;
            tail = newContact;
        } else {
            tail->next = newContact;
            tail = newContact;
        }
    }
    fin.close();
    cout << "Contacts loaded from file.\n";
}

void saveToFile() {
    ofstream fout("contacts.txt");
    if (!fout.is_open()) {
        cout << "Error: Could not open 'contacts.txt' for saving.\n";
        return;
    }
    Contact* temp = head;
    while (temp) {
        fout << temp->firstName << "," << temp->lastName << "," << temp->phoneNumber << "," << temp->email << endl;
        temp = temp->next;
    }
    fout.close();
}

bool isDuplicate(string phone) {
    Contact* temp = head;
    while (temp) {
        if (temp->phoneNumber == phone) return true;
        temp = temp->next;
    }
    return false;
}

Contact* getContactDataFromUser() {
    Contact* newContact = new Contact;
    cout << "Enter First Name: "; getline(cin, newContact->firstName);
    cout << "Enter Last Name (optional): "; getline(cin, newContact->lastName);
    cout << "Enter Phone Number: "; getline(cin, newContact->phoneNumber);

    if (isDuplicate(newContact->phoneNumber)) {
        cout << "The contact with this phone number already exists. Please use a unique phone number.\n";
        delete newContact;
        return nullptr;
    }
    cout << "Enter Email (optional): "; getline(cin, newContact->email);
    return newContact;
}

void addContactAtBeginning() {
    Contact* newContact = getContactDataFromUser();
    if (!newContact) return;

    newContact->next = head;
    head = newContact;
    cout << "Contact added at the beginning successfully!\n";
    saveToFile();
}

void addContactAtEnd() {
    Contact* newContact = getContactDataFromUser();
    if (!newContact) return;

    newContact->next = nullptr;
    if (head == nullptr) {
        head = newContact;
    } else {
        Contact* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newContact;
    }
    cout << "Contact added at the end successfully!\n";
    saveToFile();
}

void addContactAtPosition() {
    if (head == nullptr) {
        cout << "List is empty. Adding at the beginning.\n";
        addContactAtBeginning();
        return;
    }

    int position;
    int count = 0;
    Contact* temp = head;
    while(temp != nullptr) {
        count++;
        temp = temp->next;
    }

    cout << "Enter position to add (1 to " << count + 1 << ", 1 for beginning, " << count + 1 << " for end): ";
    cin >> position;
    cin.ignore();

    if (position < 1 || position > count + 1) {
        cout << "Invalid position. Please try again.\n";
        return;
    }
    if (position == 1) {
        addContactAtBeginning();
        return;
    }
    if (position == count + 1) {
        addContactAtEnd();
        return;
    }

    Contact* newContact = getContactDataFromUser();
    if (!newContact) return;

    temp = head;
    for (int i = 1; i < position - 1; ++i) {
        if (temp == nullptr) {
            cout << "Error: Position out of bounds during traversal.\n";
            delete newContact;
            return;
        }
        temp = temp->next;
    }

    newContact->next = temp->next;
    temp->next = newContact;
    
    cout << "Contact added at position " << position << " successfully!\n";
    saveToFile();
}

void addContact() {
    int choice;
    cout << "\nWhere do you want to add the contact?\n";
    cout << "1. At the Beginning\n";
    cout << "2. At the End\n";
    cout << "3. At a Specific Position\n";
    cout << "Enter choice: ";
    cin >> choice;
    cin.ignore();

    switch (choice) {
        case 1: 
        addContactAtBeginning(); 
        break;
        case 2: 
        addContactAtEnd(); 
        break;
        case 3: 
        addContactAtPosition(); 
        break;
        default: cout << "Invalid choice for adding contact. No contact added.\n";
    }
}

void deleteContact() {
    string phone;
    cout << "Enter phone number to delete: ";
    getline(cin, phone);
    Contact *temp = head, *prev = nullptr;

    while (temp && temp->phoneNumber != phone) {
        prev = temp;
        temp = temp->next;
    }

    if (!temp) {
        cout << "Contact not found!\n";
        return;
    }

    if (!prev) {
        head = temp->next;
    } else {
        prev->next = temp->next;
    }

    delete temp;
    cout << "Contact deleted.\n";
    saveToFile();
}

void viewContacts() {
    Contact* temp = head;
    if (!temp) {
        cout << "\n--- No Contacts Available ---\n";
        return;
    }
    cout << "\n--- All Contacts ---\n";
    while (temp) {
        cout << "Name: " << temp->firstName << " " << temp->lastName << endl;
        cout << "Phone: " << temp->phoneNumber << endl;
        cout << "Email: " << temp->email << endl;
        cout << "----------------------\n";
        temp = temp->next;
    }
}

void searchContact() {
    string keyword;
    cout << "Enter name or phone number to search: ";
    getline(cin, keyword);
    Contact* temp = head;
    bool found = false;
    while (temp) {
        if (temp->firstName == keyword || temp->lastName == keyword || temp->phoneNumber == keyword) {
            cout << "\n--- Contact Found ---\n";
            cout << "Name: " << temp->firstName << " " << temp->lastName << endl;
            cout << "Phone: " << temp->phoneNumber << endl;
            cout << "Email: " << temp->email << endl;
            found = true;
            return;
        }
        temp = temp->next;
    }
    if (!found) {
        cout << "Contact not found.\n";
    }
}

int compareContacts(Contact* a, Contact* b) {
    string nameA = a->firstName + a->lastName;
    string nameB = b->firstName + b->lastName;
    return nameA.compare(nameB);
}

void manualSwapStrings(string& s1, string& s2) {
    string temp = s1;
    s1 = s2;
    s2 = temp;
}

void swapContactData(Contact* a, Contact* b) {
    if (a != b) {
        manualSwapStrings(a->firstName, b->firstName);
        manualSwapStrings(a->lastName, b->lastName);
        manualSwapStrings(a->phoneNumber, b->phoneNumber);
        manualSwapStrings(a->email, b->email);
    }
}

void sortContactsMenu() {
    if (!head || !head->next) {
        cout << "Not enough contacts to sort.\n";
        return;
    }
    cout << "Choose sorting algorithm:\n1. Bubble Sort \n2. Insertion Sort \n3. Selection Sort \nChoice: ";
    int ch;
    cin >> ch;
    cin.ignore();

    auto start = high_resolution_clock::now();

    if (ch == 1) bubbleSort();
    else if (ch == 2) insertionSort();
    else if (ch == 3) selectionSort();
    else { cout << "Invalid choice!\n"; return; }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Sorted in " << duration.count() << " microseconds\n";

    saveToFile();
}

void bubbleSort() {
    if (!head || !head->next) return;
    bool swapped;
    Contact* lastSorted = nullptr;

    do {
        swapped = false;
        Contact* current = head;
        while (current->next != lastSorted) {
            if (compareContacts(current, current->next) > 0) {
                swapContactData(current, current->next);
                swapped = true;
            }
            current = current->next;
        }
        lastSorted = current;
    } while (swapped);
    cout << "Contacts sorted using Bubble Sort.\n";
}

void insertionSort() {
    if (!head || !head->next) return;

    Contact* sortedList = nullptr;
    Contact* current = head;

    while (current != nullptr) {
        Contact* nextNode = current->next;
        
        current->next = nullptr;

        if (sortedList == nullptr || compareContacts(current, sortedList) < 0) {
            current->next = sortedList;
            sortedList = current;
        } else {
            Contact* searchPtr = sortedList;
            while (searchPtr->next != nullptr && compareContacts(current, searchPtr->next) > 0) {
                searchPtr = searchPtr->next;
            }
            current->next = searchPtr->next;
            searchPtr->next = current;
        }
        current = nextNode;
    }
    head = sortedList;
    cout << "Contacts sorted using Insertion Sort.\n";
}

void selectionSort() {
    if (!head || !head->next) return;

    for (Contact* i = head; i != nullptr; i = i->next) {
        Contact* minNode = i;
        for (Contact* j = i->next; j != nullptr; j = j->next) {
            if (compareContacts(j, minNode) < 0) {
                minNode = j;
            }
        }
        if (minNode != i) {
            swapContactData(i, minNode);
        }
    }
    cout << "Contacts sorted using Selection Sort.\n";
}