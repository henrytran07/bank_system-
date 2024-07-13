
#include "customer_information.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iomanip>
using namespace std;

namespace std
{
    template <>
    struct hash<vector<unsigned char>>
    {
        size_t operator()(const vector<unsigned char> &vec) const
        {
            // Implement a hash function for std::vector<unsigned char>
            // Example implementation: using a simple hash_combine method
            size_t hash = 0;
            for (unsigned char byte : vec)
            {
                hash ^= std::hash<unsigned char>{}(byte) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
            return hash;
        }
    };
}

struct PersonalInfo
{
    string name, ssn, dob;

    PersonalInfo(const string &nm, const string &SSN, const string &DOB) : name(nm), ssn(SSN), dob(DOB) {}

    tuple<string, string, string> myPersonalInfo() const
    {
        return make_tuple(name, ssn, dob);
    }
};

struct AccountInfo
{
    string user_name, password;
    AccountInfo(const string &usm, const string &pass) : user_name(usm), password(pass) {}

    tuple<string, string> AccountInfoTuple() const
    {
        return make_tuple(user_name, password);
    }
};

bool checkName(const string &customer_name)
{
    string first_name, last_name;
    stringstream ss(customer_name);

    if (!(ss >> first_name >> last_name))
    {
        return false;
    }

    string remaining;

    if (ss >> remaining)
        return false;

    return true;
}

bool checkDOB(string &date_of_birth)
{
    int year, month, day;
    char delimeter1, delimeter2;

    stringstream ss(date_of_birth);

    if (!(ss >> year >> delimeter1 >> month >> delimeter2 >> day))
    {
        throw invalid_argument("Invalid date format.");
        return false;
    }

    if (delimeter1 != '-' || delimeter2 != '-')
    {
        throw invalid_argument("Invalid date format.");
        return false;
    }

    if (year < 1924 || year > 2024)
    {
        throw invalid_argument("Invalid year");
        return false;
    }

    if (month < 1 || month > 12)
    {
        throw invalid_argument("Invalid month");
        return false;
    }

    if (day < 1 || day > 31)
    {
        throw invalid_argument("Invalid day");
        return false;
    }

    bool is_leap_year = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
    if (month == 2)
    {
        if ((!is_leap_year && day > 28) || (is_leap_year && day > 29))
        {
            throw invalid_argument("Invalid day for February");
            return false;
        }
    }
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
    {
        throw invalid_argument("Invalid day for the month");
        return false;
    }

    return true;
}

bool checkSSN(const string &ssn)
{
    if (ssn.length() != 11 || ssn[3] != '-' || ssn[6] != '-')
    {
        throw invalid_argument("Invalid format");
        return false;
    }

    for (size_t i = 0; i < ssn.length(); ++i)
    {
        if ((i == 3 || i == 6) && ssn[i] == '-')
            continue;
        if (!isdigit(ssn[i]))
        {
            throw invalid_argument("Invalid format");
            return false;
        }
    }

    return true;
}


void Customer::user_input()
{
    cout << "Welcome to our banking system. Please verify your identity. " << endl;

    do
    {
        cout << "First step: Please enter your full name (first name + last name): ";
        getline(cin, customer_name);
    } while ((!checkName(customer_name)));

    do
    {
        cout << "Second step: Please enter your date of birth (year-month-day): ";
        getline(cin, customer_dob);
    } while ((!checkDOB(customer_dob)));

    do
    {
        cout << "Third step: Please enter your social security number (xxx-xx-xxxx): ";
        getline(cin, customer_SSN);
    } while (!checkSSN(customer_SSN));
}

bool Customer::ID_verification()
{
    PersonalInfo person(customer_name, customer_SSN, customer_dob);

    vector<unsigned char> encrypt_key;
    encrypt_key = generate_key_for_table();
    tuple<string, string, string> value;
    value = person.myPersonalInfo();

    string result = search(encrypt_key);

    if (result == "Person not found")
    {
        return false;
    }

    cout << "Correct information. " << endl;
    return true;
}

bool correctPassword(const string &password)
{
    bool hasLetter = false;
    bool hasSpecialCharacter = false;
    int digitCount = 0;
    for (const auto &a : password)
    {
        if (isalpha(a))
        {
            hasLetter = true;
        }
        if (a == '!' || a == '@' || a == '#' || a == '$' || a == '%' || a == '^' || a == '&' || a == '*')
        {
            hasSpecialCharacter = true;
        }
        if (isdigit(a))
        {
            digitCount++;
        }
    }
    return hasLetter && hasSpecialCharacter && digitCount <= 2;
}

void Customer::banking_account()
{
    cout << "Please enter your username: ";
    cin >> user_name;

    string verified_password;
    do
    {
        cout << "Please enter your password (character + special character(!, @, #...) + at most 2 numbers): ";
        cin >> password;

        cout << "Please enter your password again for verification: ";
        cin >> verified_password;
    } while ((verified_password != password) || !(correctPassword(password)));
}

string Customer::customer_account() const
{
    return user_name + " " + password;
}

void Customer::customer_information()
{
    string customer;
    customer = customer_account();

    vector<unsigned char> key_encrypt;
    key_encrypt = generate_key_for_table();

    ofstream outputFile;
    outputFile.open("customer_information.txt", ios::app);

    if (!outputFile.is_open())
    {
        cerr << "Unable to open the file" << endl;
        exit(1);
    }

    for (const auto &byte : key_encrypt)
    {
        outputFile << hex << setfill('0') << setw(2) << static_cast<int>(byte);
    }
    outputFile << " " << customer << endl;
}

void Customer::table_insert(const vector<unsigned char> &key, const tuple<string, string> &value)
{
    size_t index = hashFunction(key);
    size_t original_index = index;

    auto &map = customer_storage[index];
    auto it = map.find(key);

    if (it == map.end())
    {
        index = (index + 1) % MAXIMUM_PEOPLE;

        if (index == original_index)
        {
            cout << "Table is full " << endl;
            return;
        }
    }

    customer_storage[index][key] = value;
}

#include "customer_information.h"
#include <iostream>

int main() {
    Customer customer;

    // User input for personal information
    customer.user_input();

    // Verify the ID
    if (!customer.ID_verification()) {
        std::cout << "ID verification failed. Please check your information." << std::endl;
        return 1;
    }

    std::cout << "ID verification successful!" << std::endl;

    // Create a banking account
    customer.banking_account();

    // Save customer information
    customer.customer_information();
    std::cout << "Customer information saved successfully!" << std::endl;

    return 0;
}


