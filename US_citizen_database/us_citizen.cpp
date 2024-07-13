#include "us_citizen.h"
#include <iostream>
#include <random>
#include <numeric>
#include <algorithm>
#include <vector>
#include <functional>
#include <unordered_map>
#include <utility>
#include <tuple>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <string>
#include <unordered_map>
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
random_device Person::rd;
mt19937 Person::gen(rd());

struct PersonalInfo
{
    string name, date_of_birth, ssn;

    PersonalInfo(const string &nm, const string &dob, const string &social)
        : name(nm), date_of_birth(dob), ssn(social) {}

    tuple<string, string, string> getPersonalInfo() const
    {
        return make_tuple(name, date_of_birth, ssn);
    }
};

Person::Person()
{
    generated_people();
}
void Person::generated_people()
{
    for (int i = 0; i < MAXIMUM_PEOPLE; i++)
    {
        name = name_generator();
        date_of_birth = DOB_generator();
        social_security_number = ssn_generator();

        PersonalInfo personal_info(name, date_of_birth, social_security_number);
        vector<unsigned char> encrypted_key; 
        encrypted_key = generate_key_for_table();
        

        tuple<string, string, string> value;
        value = personal_info.getPersonalInfo();
        table_insert(encrypted_key, value);
    }
}
int Person::random_number(int low, int high)
{
    uniform_int_distribution<int> dis(low, high);
    return dis(gen);
}

string Person::name_generator()
{
    int name_length = random_number(5, 10);
    string first_name, last_name;

    for (int i = 0; i < name_length; i++)
    {
        first_name.push_back('a' + random_number(0, 25));
    }
    for (int j = 0; j < name_length; j++)
    {
        last_name.push_back('a' + random_number(0, 25));
    }

    string name = first_name + " " + last_name;
    return name;
}

string Person::ssn_generator()
{
    string ssn, ss;
    for (int i = 0; i < 9; i++)
    {
        int number = random_number(1, 9);
        string random = to_string(number);
        ss += random;
    }
    ssn = ss.substr(0, 3) + "-" + ss.substr(3, 2) + "-" + ss.substr(5, 4);
    return ssn;
}

string Person::DOB_generator()
{
    int month = random_number(1, 12);
    int year = random_number(1924, 2024);
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
    string DOB = to_string(year) + "-" + (month < 10 ? "0" : "") + to_string(month) + "-" + (day < 10 ? "0" : "") + to_string(day);
    return DOB;
}

size_t Person::hashFunction(const vector<unsigned char> &key)
{
    size_t hashValue = 0;
    for (const auto &element : key)
    {
        hashValue ^= hash<unsigned char>{}(element) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
    }
    return hashValue;
}

void handleErrors()
{
    ERR_print_errors_fp(stderr);
    abort();
}

void Person:: generateKeyAndIV(vector<unsigned char> &key, vector<unsigned char> &iv)
{
    if (!RAND_bytes(key.data(), key.size()))
        handleErrors();
    if (!RAND_bytes(iv.data(), iv.size()))
        handleErrors();
}

vector<unsigned char> Person:: encrypt(string &personal_info, const vector<unsigned char> &key, const vector<unsigned char> &iv)
{
    EVP_CIPHER_CTX *ctx = nullptr;
    vector<unsigned char> ciphertext(personal_info.size() + AES_BLOCK_SIZE);

    int len, ciphertext_len;

    if (ctx != EVP_CIPHER_CTX_new())
        handleErrors();

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key.data(), iv.data()))
        handleErrors();

    if (1 != EVP_EncryptUpdate(ctx, ciphertext.data(), &len, (unsigned char *)personal_info.c_str(), personal_info.size()))
        handleErrors();
    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len))
        handleErrors();
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    ciphertext.resize(ciphertext_len);

    return ciphertext;
}

vector<unsigned char> Person::generate_key_for_table()
{
    vector<unsigned char> key(32);
    vector<unsigned char> iv(32);
    generateKeyAndIV(key, iv);

    string nm = getName();
    string dob = getDOB();
    string ssn = getSSN();
    PersonalInfo personal_info(nm, dob, ssn);

    tuple<string, string, string> value;
    value = personal_info.getPersonalInfo();
    string value_str = get<0> (value) + " " + get<1> (value) + " " + get<2> (value);
    vector<unsigned char> ciphertext = encrypt(value_str, key, iv);

    return ciphertext; 
}

void Person::table_insert(const vector<unsigned char> &key, const tuple<string, string, string> &value)
{
    size_t index = hashFunction(key) % MAXIMUM_PEOPLE;
    size_t original_index = index;
    auto &map = table[index];
    auto it = map.find(key);

    while (it != map.end())
    {
        index = (index + 1) % MAXIMUM_PEOPLE;
        if (index == original_index)
        {
            cout << "Table is full" << endl;
            return;
        }
        map = table[index];
        it = map.find(key);
    }

    map[key] = value;
}


string Person::search(const vector<unsigned char> &key)
{
    size_t index = hashFunction(key);
    size_t original_index = index;
    auto &map = table[index];
    auto it = map.find(key);

    if (it != map.end())
    {
        auto &value = it->second;
        string personal_info = get<0>(value) + " " + get<1>(value) + " " + get<2>(value);

        return personal_info;
    }
    index = (index + 1) % MAXIMUM_PEOPLE;

    while (index != original_index)
    {
        auto &map = table[index];
        auto it = map.find(key);

        if (it != map.end())

        {
            auto &value = it->second;
            string personal_info = get<0>(value) + " " + get<1>(value) + " " + get<2>(value);

            return personal_info;
        }

        index = (index + 1) % MAXIMUM_PEOPLE;
    }

    return "Person not found";
}
