
#include "customer_information.h"
#include <iostream>
#include <random>
#include <numeric>
#include <algorithm>
#include <vector>
#include <functional>
#include <unordered_map>
#include <utility>
using namespace std;

random_device Person::rd;
mt19937 Person::gen(rd());

struct PersonalInfo
{
    string name;
    string date_of_birth;
    string ssn;

    PersonalInfo(const Person &person)
        : name(person.getName()),
          date_of_birth(person.getDOB()),
          ssn(person.getSSN()) {}
};

Person::Person()
{
    srand(time(nullptr));
    name = name_generator();
    date_of_birth = DOB_generator();
    social_security_number = ssn_generator();
}

int Person::random_number()
{
    uniform_int_distribution<int> dis(1, 26);
    return dis(gen);
}

string Person::name_generator()
{
    int name_length = random_number();
    string first_name, last_name;

    int random_step = random_number();
    for (int i = 0; i < name_length; i++)
    {
        first_name.push_back('a' + random_number());
    }
    random_step = random_number();
    for (int j = 0; j < name_length; j++)
    {
        last_name.push_back('a' + random_number());
    }

    string name = first_name + " " + last_name;
    return name;
}
int Person::digit_ssn_generator()
{
    uniform_int_distribution<int> dis(1, 9);
    return dis(gen);
}
string Person::ssn_generator()
{
    string ssn, ss;
    for (int i = 0; i < 9; i++)
    {
        int number = digit_ssn_generator();
        string random = to_string(number);
        ss += random;
    }
    ssn = ss.substr(0, 3) + "-" + ss.substr(3, 2) + "-" + ss.substr(5, 4);
    return ssn;
}

int Person::year_generator()
{
    uniform_int_distribution<int> dis(1924, 2024);
    return dis(gen);
}

int Person::month_generator()
{
    uniform_int_distribution<int> dis(1, 12);
    return dis(gen);
}
string Person::DOB_generator()
{
    int month = month_generator();
    int year = year_generator();
    int day;
    switch (month)
    {
    case 1:
    case 3:
    case 7:
    case 8:
    case 10:
    case 12:
        day = rand() % 31 + 1;
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        day = rand() % 30 + 1;
        break;
    case 2:
        if (year % 4 == 0)
            day = rand() % 29 + 1;
        else
            day = rand() % 28 + 1;
        break;
    }
    string DOB = to_string(year) + "-" + to_string(month) + "-" + to_string(day);
    return DOB;
}

size_t Person::hashFunction(const string &key)
{
    size_t hash1 = hash<string>{}(key);
    return hash1;
}

string Person::generate_key_for_table()
{
    int value_length = random_number();
    string value;
    for (int i = 0; i < value_length; i++)
    {
        value.push_back('a' + random_number());
    }
    return value;
}

void Person::table_insert(string &key, const PersonalInfo &value)
{
    if (key.empty())
        key = generate_key_for_table();

    auto it = table.find(key);
    if (it == table.end())
    {
        table[key] = value;
        cout << "Insertion successful for key '" << key << "'.\n";
    }
    else
    {

        cout << "Key '" << key << "' already exists. Insertion failed.\n";
    }
}
