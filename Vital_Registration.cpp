#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

const string DATA_FILE = "vital_records.txt";

struct Record
{
    string code;
    string type;
    vector<string> attributes;
};

// Function prototypes
void registerRecord(vector<string> &records, const string &type);
void displayAllRecords(const vector<string> &records);
void generateCertificate(const string &record);
void deleteRecordByCode(vector<string> &records);
void searchRecordsByName(const vector<string> &records);
bool saveRecordsToFile(const vector<string> &records);
bool loadRecordsFromFile(vector<string> &records);
void displayConfirmation(const string &recordType, const string &code);
string generateNextCode(const vector<string> &records);
bool isValidDate(const string &date);
bool isValidGender(const string &gender);
int findRecordByCode(const vector<string> &records, const string &code);
void parseRecord(const string &record, string &code, string &type, vector<string> &attributes);

int main()
{
    int choice;
    char confirmation;
    vector<string> records;

    cout << "┌──────────────────┐" << endl;
    if (loadRecordsFromFile(records))
    {
        cout << "│ ✓ Records loaded |" << endl;
    }
    else
    {
        cout << "│ ℹ No existing records found" << endl;
    }
    cout << "└──────────────────┘" << endl;

    do
    {
        cout << "\n╭──────────────────────────────────────╮" << endl;
        cout << "│    VITAL EVENT REGISTRATION SYSTEM   │" << endl;
        cout << "├──────────────────────────────────────┤" << endl;
        cout << "│ 1. Birth Registration                │" << endl;
        cout << "│ 2. Marriage Registration             │" << endl;
        cout << "│ 3. Divorce Registration              │" << endl;
        cout << "│ 4. Display All Records               │" << endl;
        cout << "│ 5. Generate Certificate              │" << endl;
        cout << "│ 6. Delete Record by Code             │" << endl;
        cout << "│ 7. Search Records by Name            │" << endl;
        cout << "│ 8. Exit                              │" << endl;
        cout << "╰──────────────────────────────────────╯" << endl;
        cout << "Enter choice [1-8]: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            registerRecord(records, "Birth");
            break;
        case 2:
            registerRecord(records, "Marriage");
            break;
        case 3:
            registerRecord(records, "Divorce");
            break;
        case 4:
            displayAllRecords(records);
            break;
        case 5:
        {
            string recordCode;
            cout << "\n┌─ Certificate Generation ─┐" << endl;
            cout << "│ Enter record code: ";
            cin >> recordCode;

            int recordIndex = findRecordByCode(records, recordCode);

            if (recordIndex != -1)
            {
                generateCertificate(records[recordIndex]);
            }
            else
            {
                cout << "│ ⚠️ Error: Record not found with code: " << recordCode << endl;
            }
            cout << "└─────────────────────────┘" << endl;
            break;
        }
        case 6:
            deleteRecordByCode(records);
            break;
        case 7:
            searchRecordsByName(records);
            break;
        case 8:
            cout << "\n┌─ Exiting System ─┐" << endl;
            cout << "│ ℹ All records are already saved" << endl;
            cout << "└─────────────────┘" << endl;
            break;
        default:
            cout << "\n┌─ Error ─┐" << endl;
            cout << "│ ⚠️ Invalid choice! Please enter a number between 1 and 8." << endl;
            cout << "└─────────┘" << endl;
        }

        if (choice != 8)
        {
            cout << "\n┌───────────────────────┐" << endl;
            cout << "│ Continue? [y/n]: ";
            cin >> confirmation;
            cout << "└───────────────────────┘" << endl;
        }
        else
        {
            confirmation = 'n';
        }

    } while (confirmation == 'y' || confirmation == 'Y');

    cout << "\n╭──────────────────────────╮" << endl;
    cout << "│    SYSTEM TERMINATED     │" << endl;
    cout << "╰──────────────────────────╯" << endl;
    return 0;
}

int findRecordByCode(const vector<string> &records, const string &code)
{
    for (int i = 0; i < records.size(); i++)
    {
        // Extract the code part from the record (before the first colon)
        int colonPos = records[i].find(':');
        if (colonPos != -1)
        {
            string codeFromRecord = records[i].substr(0, colonPos);
            // Return the index of the found record
            if (codeFromRecord == code)
            {
                return i;
            }
        }
    }

    // Return -1 if no matching record found
    return -1;
}

// Function to search records by name
void searchRecordsByName(const vector<string> &records)
{
    string searchName;

    cout << "\n╭─────────────────────────╮" << endl;
    cout << "│   SEARCH BY NAME        │" << endl;
    cout << "├─────────────────────────┤" << endl;
    cin.ignore();
    cout << "│ Enter Name to Search: ";
    getline(cin, searchName);
    cout << "╰─────────────────────────╯" << endl;

    vector<string> matchingRecords;

    for (const string &record : records)
    {
        if (record.find(searchName) != -1)
        {
            matchingRecords.push_back(record);
        }
    }

    cout << "\n╭───────────────────────────────────────────────────────╮" << endl;
    cout << "│                  SEARCH RESULTS                       │" << endl;
    cout << "├───────────────────────────────────────────────────────┤" << endl;

    if (matchingRecords.empty())
    {
        cout << "│ No records found matching '" << searchName << "'             │" << endl;
    }
    else
    {
        cout << "│ Found " << matchingRecords.size() << " matching record(s):                           │" << endl;
        for (int i = 0; i < matchingRecords.size(); i++)
        {
            cout << "│ " << (i + 1) << ". " << matchingRecords[i] << endl;
        }
    }

    cout << "╰───────────────────────────────────────────────────────╯" << endl;
}

void displayConfirmation(const string &recordType, const string &code)
{
    cout << "\n┌─ Record Added ─┐" << endl;
    cout << "│ ✓ " << recordType << " registration complete." << endl;
    cout << "│ ✓ Record Code: " << code << endl;
    cout << "└───────────────┘" << endl;
}

bool saveRecordsToFile(const vector<string> &records)
{
    ofstream outFile(DATA_FILE);
    if (!outFile)
        return false;

    for (const string &record : records)
    {
        outFile << record << endl;
    }

    outFile.close();
    return true;
}

bool loadRecordsFromFile(vector<string> &records)
{
    ifstream inFile(DATA_FILE);
    if (!inFile.is_open())
        return false;

    records.clear();
    string line;
    while (getline(inFile, line))
    {
        if (!line.empty())
            records.push_back(line);
    }
    inFile.close();
    const bool isEmpty = records.empty();
    return isEmpty ? false : true;
}

string generateNextCode(const vector<string> &records)
{
    int nextCode = 1;

    if (!records.empty())
    {
        string lastRecord = records.back();
        int colonPos = lastRecord.find(':');
        if (colonPos != -1)
        {
            nextCode = stoi(lastRecord.substr(0, colonPos)) + 1;
        }
    }

    return to_string(nextCode);
}

bool isValidDate(const string &date)
{
    if (date.length() != 10)
        return false;
    if (date[4] != '-' || date[7] != '-')
        return false;

    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));

    if (year < 1900 || year > 2100)
        return false;
    if (month < 1 || month > 12)
        return false;
    if (day < 1 || day > 31)
        return false;

    return true;
}

bool isValidGender(const string &gender)
{
    return gender == "M" || gender == "F";
}

void registerRecord(vector<string> &records, const string &type)
{
    string code = generateNextCode(records);
    vector<string> attributes;

    cout << "\n╭─────────────────────────╮" << endl;
    cout << "│   " << type << " REGISTRATION" << endl;
    cout << "├─────────────────────────┤" << endl;

    cin.ignore();

    if (type == "Birth")
    {
        string childName, motherName, birthDate, gender;

        cout << "│ Child's Full Name: ";
        getline(cin, childName);
        attributes.push_back(childName);

        cout << "│ Mother's Full Name: ";
        getline(cin, motherName);
        attributes.push_back(motherName);

        // Gender validation
        do
        {
            cout << "│ Gender (M/F): ";
            getline(cin, gender);
            if (!isValidGender(gender))
            {
                cout << "│ ⚠️ Error: Invalid gender. Please enter M or F only." << endl;
            }
        } while (!isValidGender(gender));
        attributes.push_back(gender);

        // Date validation
        do
        {
            cout << "│ Date (YYYY-MM-DD): ";
            getline(cin, birthDate);
            if (!isValidDate(birthDate))
            {
                cout << "│ ⚠️ Error: Invalid date format. Please use YYYY-MM-DD format." << endl;
            }
        } while (!isValidDate(birthDate));
        attributes.push_back(birthDate);
    }
    else if (type == "Marriage")
    {
        string spouse1, spouse2, marriageDate, location;

        cout << "│ Spouse 1 Full Name: ";
        getline(cin, spouse1);
        attributes.push_back(spouse1);

        cout << "│ Spouse 2 Full Name: ";
        getline(cin, spouse2);
        attributes.push_back(spouse2);

        // Date validation
        do
        {
            cout << "│ Date (YYYY-MM-DD): ";
            getline(cin, marriageDate);
            if (!isValidDate(marriageDate))
            {
                cout << "│ ⚠️ Error: Invalid date format. Please use YYYY-MM-DD format." << endl;
            }
        } while (!isValidDate(marriageDate));
        attributes.push_back(marriageDate);

        cout << "│ Location: ";
        getline(cin, location);
        attributes.push_back(location);
    }
    else if (type == "Divorce")
    {
        string person1, person2, divorceDate, reason;

        cout << "│ Person 1 Full Name: ";
        getline(cin, person1);
        attributes.push_back(person1);

        cout << "│ Person 2 Full Name: ";
        getline(cin, person2);
        attributes.push_back(person2);
        cout << "│ Reason: ";
        getline(cin, reason);
        attributes.push_back(reason);
    }

    cout << "╰─────────────────────────╯" << endl;

    string recordStr = code + ":" + type + ": ";
    for (int i = 0; i < attributes.size(); i++)
    {
        recordStr += attributes[i];
        if (i < attributes.size() - 1)
        {
            recordStr += ", ";
        }
    }
    records.push_back(recordStr);
    saveRecordsToFile(records);
    displayConfirmation(type, code);
}

void deleteRecordByCode(vector<string> &records)
{
    string codeToDelete;

    cout << "\n╭─────────────────────────╮" << endl;
    cout << "│   DELETE RECORD         │" << endl;
    cout << "├─────────────────────────┤" << endl;
    cin.ignore();
    cout << "│ Enter Record Code: ";
    getline(cin, codeToDelete);
    cout << "╰─────────────────────────╯" << endl;

   
    int index = findRecordByCode(records, codeToDelete);

    if (index != -1)
    {
    
        string recordType = "Record";
        int firstColon = records[index].find(':');
        if (firstColon != -1)
        {
            string afterCode = records[index].substr(firstColon + 1);
            int secondColon = afterCode.find(':');
            if (secondColon != -1)
            {
                recordType = afterCode.substr(0, secondColon);
            }
        }


        char confirm;
        cout << "┌─ Confirm Deletion ─┐" << endl;
        cout << "│ Record found: " << records[index] << endl;
        cout << "│ Delete this " << recordType << " record? (y/n): ";
        cin >> confirm;

        if (confirm == 'y' || confirm == 'Y')
        {
            records.erase(records.begin() + index);

        
            if (saveRecordsToFile(records))
            {
                cout << "│ ✓ " << recordType << " record " << codeToDelete << " deleted successfully" << endl;
                cout << "│ ✓ Records saved to file" << endl;
            }
            else
            {
                cout << "│ ✓ " << recordType << " record " << codeToDelete << " deleted" << endl;
                cout << "│ ✗ Failed to save updated records" << endl;
            }
        }
        else
        {
            cout << "│ Deletion cancelled" << endl;
        }
        cout << "└───────────────┘" << endl;
    }
    else
    {
        cout << "┌─ Error ─┐" << endl;
        cout << "│ No record found with code: " << codeToDelete << endl;
        cout << "└─────────┘" << endl;
    }
}

void displayAllRecords(const vector<string> &records)
{
    cout << "\n╭───────────────────────────────────────────────────────╮" << endl;
    cout << "│                    ALL RECORDS                        │" << endl;
    cout << "├───────────────────────────────────────────────────────┤" << endl;
    if (records.empty())
    {
        cout << "│ No records found.                                    │" << endl;
    }
    else
    {
        for (int i = 0; i < records.size(); i++)
        {
            cout << "│ " << records[i] << endl;
        }
    }
    cout << "╰───────────────────────────────────────────────────────╯" << endl;
}

void parseRecord(const string &record, string &code, string &type, vector<string> &attributes)
{
    int first_colon = record.find(':');
    if (first_colon == -1)
        return;

    code = record.substr(0, first_colon);
    string after_code = record.substr(first_colon + 1);

    int second_colon = after_code.find(':');
    if (second_colon == -1)
        return;

    type = after_code.substr(0, second_colon);
    string details = after_code.substr(second_colon + 2); 

    attributes.clear();
    int pos = 0;
    string token;
    string temp = details;

    while ((pos = temp.find(',')) != -1)
    {
        token = temp.substr(0, pos);
        attributes.push_back(token);
        temp.erase(0, pos + 2); 
    }
    attributes.push_back(temp); 
}

void generateCertificate(const string &record)
{
    string code, type;
    vector<string> attributes;

    parseRecord(record, code, type, attributes);

    cout << "\n╭────────────────────────────────────────────╮" << endl;
    cout << "│         CERTIFICATE OF REGISTRATION        │" << endl;
    cout << "├────────────────────────────────────────────┤" << endl;
    cout << "│ This certifies that the information below  │" << endl;
    cout << "│ is a true and accurate record.             │" << endl;
    cout << "├────────────────────────────────────────────┤" << endl;
    cout << "│ Registration Number: " << code << endl;
    cout << "│ Document Type: " << type << endl;


    if (type == "Birth" && attributes.size() >= 4)
    {
        string childName = attributes[0];
        string motherName = attributes[1];
        string gender = attributes[2];
        string birthDate = attributes[3];
        cout << "│ Child's Full Name: " << childName << endl;
        cout << "│ Mother's Full Name: " << motherName << endl;
        cout << "│ Date of Birth: " << birthDate << endl;
        cout << "│ Gender: " << gender << endl;
    }
    else if (type == "Marriage" && attributes.size() >= 4)
    {
        string spouse1 = attributes[0];
        string spouse2 = attributes[1];
        string marriageDate = attributes[2];
        string location = attributes[3];
        cout << "│ Spouse 1 Full Name: " << spouse1 << endl;
        cout << "│ Spouse 2 Full Name: " << spouse2 << endl;
        cout << "│ Date of Marriage: " << marriageDate << endl;
        cout << "│ Location: " << location << endl;
    }
    else if (type == "Divorce" && attributes.size() >= 4)
    {
        string person1 = attributes[0];
        string person2 = attributes[1];
        string divorceDate = attributes[2];
        string reason = attributes[3];
        cout << "│ Person 1 Full Name: " << person1 << endl;
        cout << "│ Person 2 Full Name: " << person2 << endl;
        cout << "│ Date of Divorce: " << divorceDate << endl;
        cout << "│ Reason: " << reason << endl;
    }
    cout << "╰────────────────────────────────────────────╯" << endl;
}