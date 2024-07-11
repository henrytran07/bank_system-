
#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream> 

using namespace std; 

class BankAccount {
    protected:
        string name; 
        double limit, balance; 
    public: 
        BankAccount(string nm, double lim, double bal) : name(nm), limit(lim), balance(bal) {}
        double getBalance() const {return balance;}
        double getLimit() const {return limit;}
};
#endif 