
#include "customer_information.h"
#include <iostream> 
#include <vector> 
#include <tuple>
#include <fstream> 
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



void Customer:: user_input(){
    cout << "Welcome to our banking system. Please verify your identity. " << endl; 

    cout << "First step: Please enter your full name (first name + last name): ";
    getline(cin, customer_name);

    cout << "Second step: Please enter your date of birth(year-month-day): " ;
    getline(cin, customer_dob);

    cout << "Third step: Please enter your social security number (xxx-xx-xxxx): ";
    getline(cin, customer_SSN);
}

bool Customer:: ID_verification() {
  
    PersonalInfo person(customer_name, customer_dob, customer_SSN); 

    vector<unsigned char> key, iv, encrypt_key; 
    key = generate_key_for_table();
    generateKeyAndIV(key, iv);
    tuple<string, string, string> value; 
    value = person.myPersonalInfo();
    string value_string = get<0> (value) + get<1> (value) + get<2> (value); 
    encrypt_key = encrypt(value_string, key, iv);

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

    vector<unsigned char> key, iv, key_encrypt; 
    key = generate_key_for_table();
    generateKeyAndIV(key, iv);
    key_encrypt = encrypt(customer, key, iv);

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

void Customer::table_insert(const vector<unsigned char>& key, tuple<string, string>& value){
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
