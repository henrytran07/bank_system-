
#ifndef CUSTOMER_INFORMATION_H
#define CUSTOMER_INFORMATION_H
#include "/Users/tranhuuloc/Documents/DSA_CODE/CreditCard/bank_system/bank_system-/US_citizen_database/us_citizen.h"
#include <iostream>
#include <fstream> 
#include <tuple>
#include <unordered_map> 
using namespace std; 
struct PersonalInfo; 
struct AccountInfo; 
class Customer: public Person {
    private: 
        string customer_name, customer_SSN, customer_dob;
        string user_name, password;
        unordered_map<size_t, unordered_map<vector<unsigned char>, tuple<string, string>> > customer_storage; 

        bool ID_verification();
        void user_input();
        void banking_account();
        string customer_account() const; 
        void customer_information(); 
        void table_insert(const vector<unsigned char>& key, tuple<string, string>& value);
    public: 
        
             
};

#endif
