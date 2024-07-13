
#include "customer_information.h"
#include <iostream> 
#include <vector> 
#include <tuple>
#include <fstream> 
#include <sstream> 
#include <stdexcept> 
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
struct PersonalInfo{
    string name, ssn, dob; 

    PersonalInfo(const string& nm, const string& SSN, const string& DOB): name(nm), ssn(SSN), dob(DOB){}

    tuple<string, string, string> myPersonalInfo() const {
        return make_tuple(name, ssn, dob);
    }
};

struct AccountInfo {
    string user_name, password; 
    AccountInfo(const string& usm, const string& pass): user_name(usm), password(pass){}

    tuple<string, string> AccountInfoTuple() const {
        return make_tuple(user_name, password);
    }
};

bool checkName(string customer_name){
    string first_name, last_name; 
    stringstream ss(customer_name);

    if (!(ss >> first_name >> last_name)){
        return false; 
    }

    string remaining;

    if (ss >> remaining)
        return false; 
    
    return true; 
}

bool checkDOB(string &date_of_birth){
    int year, month, day; 
    char delimeter1, delimeter2; 

    stringstream ss(date_of_birth);

    if (!(ss >> year >> delimeter1 >> month >> delimeter2 >> year)){
        throw invalid_argument("Invalid date format.");
        return false; 
    }
        
    
    if (delimeter1 != '-' || delimeter2 != '-'){
        throw invalid_argument("Invalid date format. ");
        return false; 
    }
        
    if (year < 1924 || year > 1924){
        throw invalid_argument("Invalid year");
        return false; 
    } 
       

    if (month < 1 || month > 12){
        throw invalid_argument("Invalid month");
        return false; 
    }
        

    if (day < 1 || day > 31){
        throw invalid_argument("Invalid day");
        return false; 
    }   
    bool is_leap_year = (year % 4 == 0);
    if (month == 2){
        if (is_leap_year && day > 28){
            throw invalid_argument("Invalid day");
            return false;
        }
        if ((!is_leap_year) && day > 29){
            throw invalid_argument("Invalid day");
            return false; 
        }     
    }

    return true; 
}

bool checkSSN(string ssn){
    stringstream ss(ssn);
    char delimeter1, delimeter2; 
    int num[9]; 
    int num1, num2, num3;

    if (!(ss >> num1 >> delimeter1 >> num2 >> delimeter2 >> num3)){
        throw invalid_argument("Invalid format");
        return false; 
    }

    if (delimeter1 != '-' || delimeter2 != '-'){
        throw invalid_argument("Invalid format");
        return false; 
    }
    return true; 
}

void Customer:: user_input(){
    cout << "Welcome to our banking system. Please verify your identity. " << endl; 

    do {
        cout << "First step: Please enter your full name (first name + last name): ";
        getline(cin, customer_name);
    } while ((!checkName(customer_name)));

    do {
        cout << "Second step: Please enter your date of birth(year-month-day): " ;
        getline(cin, customer_dob);
    } while ((!checkDOB(customer_dob)));
 
    do {
        cout << "Third step: Please enter your social security number (xxx-xx-xxxx): ";
        getline(cin, customer_SSN);
    } while (!checkSSN(customer_SSN));

}

bool Customer:: ID_verification() {
  
    PersonalInfo person(customer_name, customer_dob, customer_SSN); 

    vector<unsigned char> encrypt_key; 
    encrypt_key = generate_key_for_table();
    tuple<string, string, string> value; 
    value = person.myPersonalInfo();


    string result = search(encrypt_key);

    if (result == "Person not found") {
        return false; 
    }

    cout << "Correct information. " << endl;
    return true;  
}

bool correctPassword(string password){
    bool hasLetter = false; 
    bool hasSpecialCharacter = false;
    int digitCount = 0; 
    for (const auto& a: password){
        if (isalpha(a)){
            hasLetter = true; 
        }
        if (a == '!'|| a== '@'|| a== '#'|| a== '$'|| a== '%'|| a== '^'|| a== '&'|| a== '&'|| a== '*'){
                hasSpecialCharacter = true; 
        }

        if (isdigit(a)){
            digitCount++; 
        }
    }

    return hasLetter && hasSpecialCharacter && digitCount <= 2;  
}
void Customer:: banking_account() {
    cout << "Please enter your username: ";
    cin >> user_name;

    string verified_password;
    do {
        cout << "Please enter your password (character + special character(!, @, #...) + at most 2 numbers): ";
        cin >> password; 

        cout << "Please enter your password again for verification: ";
        cin >> verified_password; 
    } while((verified_password != password) || !(correctPassword(password)));
}

string Customer:: customer_account() const {
    return user_name + " " + password;
}

void Customer:: customer_information(){
    string customer; 
    customer = customer_account();

    vector<unsigned char>  key_encrypt; 
    key_encrypt = generate_key_for_table();

    ofstream outputFile; 
    outputFile.open("customer_information.txt", ios::app);

    if (!outputFile.is_open()){
        cerr << "Unable to open the file" << endl; 
        exit(1);
    }

    for (const auto& byte: key_encrypt){
        outputFile << hex << setfill('0') << setw(2) << static_cast<int>(byte);
    }  
}

void Customer::table_insert(const vector<unsigned char>& key, const tuple<string, string>& value){
    size_t index = hashFunction(key);
    size_t original_index = index; 

    auto& map = customer_storage[index];
    auto it = map.find(key);

    if (it == map.end()){
        index = (index + 1) % (MAXIMUM_PEOPLE);

        if (index == original_index){
            cout << "Table is full " << endl;
        }
    }

    customer_storage[index][key] = value; 
}
