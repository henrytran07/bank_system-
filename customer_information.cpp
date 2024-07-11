
#include "customer_information.h"
#include <iostream> 
#include <random> 
#include <numeric> 
#include <algorithm> 
#include <vector> 
#include <functional>
#include <unordered_map>
#include <utility> 
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <fstream> 
using namespace std; 

namespace std {
    template<>
    struct hash<std::vector<unsigned char>> {
        size_t operator()(const std::vector<unsigned char>& vec) const {
            // Simple hash function for std::vector<unsigned char>
            size_t hash = 0;
            for (unsigned char val : vec) {
                hash ^= std::hash<unsigned char>()(val) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
            return hash;
        }
    };
}
random_device Person::rd;
mt19937 Person::gen(rd());

struct PersonalInfo {
    string name;
    string date_of_birth;
    string ssn;
    PersonalInfo() = default; 
    PersonalInfo(const Person& person)
        : name(person.getName()),
          date_of_birth(person.getDOB()),
          ssn(person.getSSN()) {}
    friend ostream& operator<<(ostream& os, const PersonalInfo& person){
        os  << "Name: " << person.name
            << "Date of birth: " << person.date_of_birth
            << "SSN: " << person.ssn;
    }
};


Person:: Person() {
    generated_number = 0; 
    while (generated_number <= MAXIMUM){
        name = name_generator();
        date_of_birth = DOB_generator();
        social_security_number = ssn_generator();
        generated_number++; 
    }
   
    // table[make_pair(name, make_pair(date_of_birth, social_security_number))] = generate_value_for_table();
}

int Person:: random_number(int lower, int higher){
    uniform_int_distribution<int> dis(lower, higher);
    return dis(gen);
}

string Person:: name_generator() {
    int name_length = random_number(5, 10);
    string first_name, last_name; 
    
    
    for (int i = 0; i < name_length; i++){
        first_name.push_back('a' + random_number(0, 25));
    }
    for (int j = 0; j < name_length; j++){
        last_name.push_back('a' + random_number(0, 25));   
    }

    string name = first_name + " " + last_name; 
    return name; 
}

string Person:: ssn_generator() {
    string ssn, ss;    
    for (int i = 0; i < 9; i++){
        int number = random_number(1, 9);
        string random = to_string(number);
        ss += random; 
    }
    ssn = ss.substr(0, 3) + "-" + ss.substr(3, 2) + "-" + ss.substr(5, 4);
    return ssn;     
}

string Person:: DOB_generator() {
    int month = random_number(1, 12); 
    int year = random_number(1924, 2024);
    int day; 
    switch(month) {
        case 1: case 3: case 7: case 8: case 10: case 12:
            day = random_number(1, 31); 
            break; 
        case 4: case 6: case 9: case 11: 
            day = random_number(1, 30); 
            break; 
        case 2: 
            if (year % 4 == 0)
                day = random_number(1, 29);
            else 
                day = random_number(1, 28); 
            break; 
    }
    string DOB = to_string(year) + "-" + to_string(month) + "-" + to_string(day);
    return DOB; 
}

void Person:: handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}
vector<unsigned char> Person:: encrypt(const string& name, const string& date_of_birth, const string& social_security, const vector<unsigned char>& key, const vector<unsigned char>& iv){
    string plaintext = name + " " + date_of_birth + " " + social_security; 
    EVP_CIPHER_CTX *ctx; 
    vector<unsigned char> ciphertext(plaintext.size() + AES_BLOCK_SIZE);
    int len, ciphertext_len; 

    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key.data(), iv.data())) handleErrors();

    if (1 != EVP_EncryptUpdate(ctx, ciphertext.data(), &len, (unsigned char *)plaintext.c_str(), plaintext.size())) handleErrors();

    ciphertext_len = len; 

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len)) handleErrors();
    ciphertext_len += len; 

    EVP_CIPHER_CTX_free(ctx);

    ciphertext.resize(ciphertext_len);
    return ciphertext; 
}

void Person:: generateKeyandIV(vector<unsigned char>& key, vector<unsigned char>& iv){
    if(!RAND_bytes(key.data(), key.size())) handleErrors();
    if (!RAND_bytes(iv.data(), iv.size())) handleErrors();
}
vector<unsigned char> Person:: generate_key_for_table(){
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    vector<unsigned char> key(32);
    vector<unsigned char> iv(16);

    generateKeyandIV(key, iv);

    vector<unsigned char> ciphertext = encrypt(name, date_of_birth, social_security_number, key, iv);
    ofstream outputFile; 
    outputFile.open("customer_information.txt");
    
    if (!outputFile.is_open()){
        cerr << "Unable to read the file." << endl; 
        EVP_cleanup();
        ERR_free_strings();
        return {};
    }

    outputFile.write(reinterpret_cast<const char*> (ciphertext.data()), ciphertext.size());
    outputFile.close();
    EVP_cleanup();
    ERR_free_strings();
    return ciphertext; 
}

void Person::table_insert() {
    vector<unsigned char> key = generate_key_for_table();
    PersonalInfo info(*this); // Assuming getName(), getDOB(), getSSN() methods exist

    auto it = table.find(key);
    if (it == table.end()) {
        table[key] = info;
    }
}

bool Person::findPerson(vector<unsigned char>& key){
    auto it = table.find(key);
    if (it != table.end()){
        const PersonalInfo& info = it -> second; 
        cout << info << endl; 
        return true; 
    }
    return false; 
} 

void generateRandomKey(vector<unsigned char>& key, size_t size) {
    if (!RAND_bytes(key.data(), size)) {
        cerr << "Error generating random key" << endl;
        // Handle error, possibly by throwing an exception or returning an error code
    }
}
int main() {
    // Create a Person object
    Person p;

    // Generate a random key for searching
    vector<unsigned char> key(32);
    generateRandomKey(key, key.size());

    // Search for a person in the table
    if (p.findPerson(key)) {
        cout << "Person found!" << endl;
    } else {
        cout << "Person not found." << endl;
    }

    return 0;
}