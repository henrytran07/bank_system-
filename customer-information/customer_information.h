
#ifndef CUSTOMER_INFORMATION_H
#define CUSTOMER_INFORMATION_H
#include <iostream> 
#include <random> 
#include <unordered_map>
#include <map> 
#include <utility> 
#include <tuple> 
using namespace std; 
struct PersonalInfo; 
class Person{
    private: 
        static const int MAXIMUM_PEOPLE = 1000;
        static int generated_number; 

        void generated_people();
        string name_generator();
        string DOB_generator();
        string ssn_generator();
        int random_number(int low, int high);
        size_t hashFunction(const vector<unsigned char> &key);
        vector<unsigned char> generate_key_for_table();
        void table_insert(const vector<unsigned char> &key, const tuple<string, string, string>& value);
    protected: 
        static random_device rd; 
        static mt19937 gen;
        string name;
        string date_of_birth; 
        string social_security_number; 
        
        unordered_map<size_t, unordered_map<vector<unsigned char>, tuple<string, string, string>>> table[MAXIMUM_PEOPLE];
        
        string search(const vector<unsigned char>& key);
    public: 
        Person();
        string getName() const {return name;}; 
        string getDOB() const {return date_of_birth;}
        string getSSN() const {return social_security_number;}
        
};
#endif 