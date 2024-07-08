
#ifndef CUSTOMER_INFORMATION_H
#define CUSTOMER_INFORMATION_H
#include <iostream> 
#include <random> 
#include <unordered_map>
#include <map> 
#include <utility> 
using namespace std; 
struct PersonalInfo; 
class Person{
    private: 
        string name_generator();
        string DOB_generator();
        string ssn_generator();
        int random_number();
        int year_generator();
        int digit_ssn_generator();
        int month_generator();
        size_t hashFunction(const string &key);
        string generate_key_for_table();
        void table_insert(string &key, const PersonalInfo& value);
    protected: 
        string name;
        string date_of_birth; 
        string social_security_number; 

        unordered_map<string, PersonalInfo> table; 
        static random_device rd; 
        static mt19937 gen;
    public: 
        Person();
        string getName() const {return name;}; 
        string getDOB() const {return date_of_birth;}
        string getSSN() const {return social_security_number;}
};
#endif 