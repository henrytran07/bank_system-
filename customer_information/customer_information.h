#ifndef CUSTOMER_INFORMATION_H
#define CUSTOMER_INFORMATION_H

#include <iostream>
#include <random>
#include <unordered_map>
#include <utility>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/err.h>
using namespace std;

// Forward declaration of PersonalInfo struct
struct PersonalInfo;

class Person {
private:
    static const int MAXIMUM = 1000; 
    static int generated_number; 
    string name_generator();
    string DOB_generator();
    string ssn_generator();
    int random_number(int lower, int higher);
    vector<unsigned char> generate_key_for_table();
    void table_insert();
    void handleErrors();
    vector<unsigned char> encrypt(const string& name, const string& date_of_birth, const string& social_security, const vector<unsigned char>& key, const vector<unsigned char>& iv);
    void generateKeyandIV(vector<unsigned char> &key, vector<unsigned char> &iv);

    
protected:
    string name;
    string date_of_birth;
    string social_security_number;

    unordered_map<vector<unsigned char>, PersonalInfo> table;  // Table to store PersonalInfo

    static random_device rd;
    static mt19937 gen;

public:
    Person();
    string getName() const { return name; }
    string getDOB() const { return date_of_birth; }
    string getSSN() const { return social_security_number;}
    bool findPerson(vector<unsigned char> &key);
};

#endif // CUSTOMER_INFORMATION_H