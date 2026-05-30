#include "Customer.hpp"
#include "Account.hpp"
#include <iostream>
#include <utility>

Customer::Customer(std::string firstName, std::string lastName,
                   std::string phone, std::string email)
    : firstName(std::move(firstName)),
      lastName(std::move(lastName)),
      phoneNumber(std::move(phone)),
      emailAddress(std::move(email)) {}

std::string Customer::getFullName() const { return firstName + " " + lastName; }
std::string Customer::getPhone()    const { return phoneNumber; }
std::string Customer::getEmail()    const { return emailAddress; }

void Customer::setEmail(const std::string& e)     { emailAddress = e; }
void Customer::setPhone(const std::string& p)     { phoneNumber = p; }
void Customer::setFirstName(const std::string& f) { firstName = f; }
void Customer::setLastName(const std::string& l)  { lastName = l; }

void Customer::addAccount(const std::shared_ptr<Account>& account) {
    accounts.push_back(account); // przechowujemy weak_ptr
}

const std::vector<std::shared_ptr<Account>>& Customer::getAccounts() const {
    // Zwracamy referencję do wektora strong-ptr - budujemy go na żądanie
    // Żeby móc zwrócić const ref, trzymamy cache jako mutable
    cachedAccounts.clear();
    for (const auto& weak : accounts) {
        if (auto acc = weak.lock()) {
            cachedAccounts.push_back(acc);
        }
    }
    return cachedAccounts;
}

int Customer::getAccountCount() const {
    int count = 0;
    for (const auto& w : accounts) {
        if (!w.expired()) count++;
    }
    return count;
}

void Customer::display() const {
    std::cout << "OWNER DETAILS:\n";
    std::cout << "  Name:     " << getFullName() << "\n";
    std::cout << "  Phone:    " << phoneNumber   << "\n";
    std::cout << "  Email:    " << emailAddress  << "\n";
    std::cout << "  Accounts: " << getAccountCount() << "\n";
}
