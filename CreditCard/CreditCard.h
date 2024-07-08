
#ifndef CREDIT_CARD_H
#define CREDIT_CARD_H
#include <iostream> 
#include "account.h"

using namespace std; 

class CreditCard : public BankAccount {
    public: 
        CreditCard(string nm, double lim, double bal) : BankAccount(nm, lim, bal) {} 
        bool chargeIt(double price);
        void getPayment(double payment);
        friend ostream &operator<< (ostream& os, const CreditCard& c);
        void overdue();
};

#endif