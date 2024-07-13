#ifndef US_CITIZEN_H
#define US_CITIZEN_H
#include <iostream>
#include <random>
#include <unordered_map>
#include <map>
#include <utility>
#include <tuple>
using namespace std;
struct PersonalInfo;
class Person
{
private:
    void generated_people();
    string name_generator();
    string DOB_generator();
    string ssn_generator();
    int random_number(int low, int high);
  
    void table_insert(const vector<unsigned char> &key, const tuple<string, string, string> &value);
protected:
    static random_device rd;
    static mt19937 gen;
    string name;
    string date_of_birth;
    string social_security_number;
    static const int MAXIMUM_PEOPLE = 1000;

    void generateKeyAndIV(vector<unsigned char> &key, vector<unsigned char> &iv);
    vector<unsigned char> encrypt(string &personal_info, const vector<unsigned char> &key, const vector<unsigned char> &iv);
    size_t hashFunction(const vector<unsigned char> &key);
    unordered_map<size_t, unordered_map<vector<unsigned char>, tuple<string, string, string>> > table;
    vector<unsigned char> generate_key_for_table();
    string search(const vector<unsigned char> &key);

public:
    Person();
    string getName() const { return name; };
    string getDOB() const { return date_of_birth; }
    string getSSN() const { return social_security_number; }
};
#endif
