
#include <iostream> 
#include "CreditCard.h"

using namespace std; 

ostream& operator << (ostream& os, const CreditCard &c){
    os  << "Name: " << c.name
        << "Balance: " << c.balance
        << "Limit: " << c.limit; 
    return os; 
}
bool CreditCard:: chargeIt(double price){
    if (price + balance > limit) {
        cout << "Card declined!" << endl; 
        return false; 
    }
    balance += price; 
}

void CreditCard::getPayment(double payment){
    if (balance <= 0) {
        cout << "Your balance is 0" << endl; 
        balance = 0; 
        return; 
    }
    balance -= payment; 
}

void CreditCard:: overdue(){
    float APR_penalty = 0.299; 
    double interest = balance * APR_penalty; 
    balance += interest;
    balance = ceil(balance * 100) / 100; 
}