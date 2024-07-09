#include <iostream> 
#include "CreditCard.h"
#include <openssl/ssl.h>

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
ostream& operator<< (ostream& os, const CreditCard &c) {
    os << "Names: " << c.name << ", "
       << "Balance: " << c.balance << ", "
       << "Limit: " << c.limit;
    return os; 
}

bool CreditCard::chargeIt(double price) {
    if (price + balance > limit) {
        cout << "Card declined!" << endl; 
        return false; 
    }
    balance += price; 
    return true; 
}

void CreditCard::getPayment(double payment) {
    balance -= payment; 
    if (balance < 0) {
        balance = 0;
    }
}

void CreditCard::overdue() {
    float APR_penalty = 0.299; 
    double interest = balance * APR_penalty; 
    balance += interest;
    balance = ceil(balance * 100) / 100; 
}
