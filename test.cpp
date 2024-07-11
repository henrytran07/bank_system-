#include <iostream>
#include <random>
#include <vector>
#include <unordered_map>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <cstring>
#include <tuple>

using namespace std;

// Define AES block size

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

struct PersonalInfo;

class Person {
private:
    static const int MAXIMUM_PEOPLE_IN_DATABASE = 1000;
    static int generated_number;
    string name_generator();
    string DOB_generator();
    string ssn_generator();
    int random_number(int lower, int higher);
    void handleErrors();
    void generateKeyandIV(vector<unsigned char>& key, vector<unsigned char>& iv);

protected:
    string name;
    string date_of_birth;
    string social_security_number;

    unordered_map<vector<unsigned char>, PersonalInfo> table; // Table to store PersonalInfo

    static random_device rd;
    static mt19937 gen;

public:
    Person();
    string getName() const { return name; }
    string getDOB() const { return date_of_birth; }
    string getSSN() const { return social_security_number; }
    void generatePersons();
    void table_insert();
    bool findPerson(const vector<unsigned char>& key);
};

random_device Person::rd;
mt19937 Person::gen(rd());

struct PersonalInfo {
    PersonalInfo() = default;
    string name;
    string date_of_birth;
    string ssn;
    
    
    PersonalInfo(const Person& person)
        : name(person.getName()),
          date_of_birth(person.getDOB()),
          ssn(person.getSSN()) {}

    friend ostream& operator<<(ostream& os, const PersonalInfo& person) {
        os << "Name: " << person.name << ", Date of birth: " << person.date_of_birth << ", SSN: " << person.ssn << endl;
        return os;
    }
};

int Person::generated_number = 0;

Person::Person() {
    generated_number = 0;
}

void Person::generatePersons() {
    while (generated_number < MAXIMUM_PEOPLE_IN_DATABASE) {
        name = name_generator();
        date_of_birth = DOB_generator();
        social_security_number = ssn_generator();
        table_insert();
        generated_number++;
    }
}

string Person::name_generator() {
    int name_length = random_number(5, 10);
    string first_name, last_name;

    for (int i = 0; i < name_length; i++) {
        first_name.push_back('a' + random_number(0, 25));
    }
    for (int j = 0; j < name_length; j++) {
        last_name.push_back('a' + random_number(0, 25));
    }

    string name = first_name + " " + last_name;
    return name;
}

string Person::ssn_generator() {
    string ss;
    for (int i = 0; i < 9; i++) {
        int number = random_number(0, 9); // Corrected range
        ss += to_string(number);
    }
    string ssn = ss.substr(0, 3) + "-" + ss.substr(3, 2) + "-" + ss.substr(5, 4);
    return ssn;
}

string Person::DOB_generator() {
    int month = random_number(1, 12);
    int year = random_number(1924, 2024);
    int day;
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            day = random_number(1, 31);
            break;
        case 4: case 6: case 9: case 11:
            day = random_number(1, 30);
            break;
        case 2:
            if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0)
                day = random_number(1, 29);
            else
                day = random_number(1, 28);
            break;
    }
    string DOB = to_string(year) + "-" + to_string(month) + "-" + to_string(day);
    return DOB;
}

int Person::random_number(int lower, int higher) {
    uniform_int_distribution<int> dis(lower, higher);
    return dis(gen);
}

void Person::handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

void Person::generateKeyandIV(vector<unsigned char>& key, vector<unsigned char>& iv) {
    if (!RAND_bytes(key.data(), key.size())) handleErrors();
    if (!RAND_bytes(iv.data(), iv.size())) handleErrors();
}


vector<unsigned char> encrypt(const tuple<string, string, string>& data,
                              const vector<unsigned char>& key,
                              const vector<unsigned char>& iv) {
    // Concatenate data into plaintext
    string concatenated_data = get<0>(data) + " " + get<1>(data) + " " + get<2>(data);

    // Initialize AES encryption context
    AES_KEY aesKey;
    if (AES_set_encrypt_key(key.data(), 128, &aesKey) != 0) {
        cerr << "AES_set_encrypt_key failed!" << endl;
        exit(1);
    }

    // Pad the input data to match AES_BLOCK_SIZE
    int padded_length = concatenated_data.size();
    int padding = AES_BLOCK_SIZE - (padded_length % AES_BLOCK_SIZE);
    padded_length += padding;

    vector<unsigned char> plaintext(padded_length);
    memcpy(plaintext.data(), concatenated_data.c_str(), concatenated_data.size());
    memset(plaintext.data() + concatenated_data.size(), padding, padding);

    // Perform AES CBC encryption
    vector<unsigned char> ciphertext(padded_length);
    AES_cbc_encrypt(plaintext.data(),             // Input data
                    ciphertext.data(),            // Output buffer
                    padded_length,               // Length of input data
                    &aesKey,                     // AES key
                    const_cast<unsigned char*>(iv.data()), // IV (cast away constness)
                    AES_ENCRYPT);                // Encryption mode

    return ciphertext;
}


void Person::table_insert() {
    vector<unsigned char> key(32); // Adjust the size according to your key requirements
    vector<unsigned char> iv(16);  // Adjust the size according to your IV requirements

    generateKeyandIV(key, iv);

    vector<unsigned char> encrypted_data = encrypt(make_tuple(name, date_of_birth, social_security_number), key, iv);
    table[key] = PersonalInfo(*this);
}

bool Person::findPerson(const vector<unsigned char>& key) {
    auto it = table.find(key);
    if (it != table.end()) {
        const PersonalInfo& info = it->second;
        cout << info << endl;
        return true;
    }
    return false;
}

int main() {
    Person p;
    p.generatePersons();

    // Example: Find a person by key
    // Replace with actual key generation logic
    vector<unsigned char> key(32);  // Replace with actual key generation logic
    bool found = p.findPerson(key);
    if (!found) {
        cout << "Person not found." << endl;
    }

    return 0;
}
