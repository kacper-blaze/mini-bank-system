#pragma once
#include <memory>
#include <string>
#include <vector>

class Account;

class Customer {
public:
    Customer(std::string firstName,
             std::string lastName,
             std::string phone,
             std::string email);

    std::string getFullName() const;
    std::string getPhone() const;
    std::string getEmail() const;

    void setPhone(const std::string& newPhone);
    void setEmail(const std::string& newEmail);
    void setFirstName(const std::string& newFirstName);
    void setLastName(const std::string& newLastName);

    void addAccount(const std::shared_ptr<Account>& account);
    const std::vector<std::shared_ptr<Account>>& getAccounts() const;
    int getAccountCount() const;

    void display() const;

private:
    std::string firstName;
    std::string lastName;
    std::string phoneNumber;
    std::string emailAddress;

    // weak_ptr zapobiega cyklowi: Account -> Customer -> Account
    std::vector<std::weak_ptr<Account>> accounts;

    // Cache żeby móc zwrócić const ref bez kopiowania przy każdym dostępie
    mutable std::vector<std::shared_ptr<Account>> cachedAccounts;
};
