#include "Account.hpp"
#include <algorithm>
#include <iostream>
#include <utility>

int Account::accountCount = 0;

Account::Account(std::shared_ptr<Customer> accountOwner, AccountType type)
    : balance(0.0),
      status(AccountStatus::Active),
      accountType(type) {
    owners.push_back(std::move(accountOwner));
    accountCount++;
    accountNumber = "BNK-" + std::to_string(accountCount);
}

bool Account::deposit(double amount, const std::string& description, TransactionType type) {
    if (status != AccountStatus::Active) return false;
    if (amount <= 0) return false;

    balance += amount;
    transactionHistory.emplace_back(type, amount, description);
    return true;
}

bool Account::withdraw(double amount, const std::string& description, TransactionType type) {
    if (status != AccountStatus::Active) return false;
    if (amount <= 0 || balance < amount) return false;

    balance -= amount;
    transactionHistory.emplace_back(type, amount, description);
    return true;
}

void Account::addOwner(const std::shared_ptr<Customer>& newOwner) {
    if (newOwner) owners.push_back(newOwner);
}

void Account::removeOwner(const std::shared_ptr<Customer>& owner) {
    if (owners.size() == 1 && owners.front() == owner) {
        if (balance > 0.0) {
            std::cout << "Error: Cannot remove the last owner while the account has a positive balance ($"
                      << balance << ").\n";
            return;
        }
        owners.clear();
        status = AccountStatus::Closed;
        std::cout << "Last owner removed. Account has been automatically closed.\n";
        return;
    }
    owners.erase(std::remove(owners.begin(), owners.end(), owner), owners.end());
}

double Account::getBalance() const { return balance; }

std::string Account::getAccountNumber() const { return accountNumber; }

AccountType Account::getAccountType() const { return accountType; }

std::string Account::getAccountTypeString() const {
    switch (accountType) {
        case AccountType::Savings:  return "Savings";
        case AccountType::Checking: return "Checking";
        default:                    return "Basic";
    }
}

int Account::getAccountCount() { return accountCount; }

std::vector<std::shared_ptr<Customer>> Account::getOwner() const { return owners; }

void Account::display() const {
    std::cout << "====================================\n";
    std::cout << "ACCOUNT NUMBER: " << accountNumber << "\n";
    std::cout << "TYPE:           " << getAccountTypeString() << "\n";
    std::cout << "BALANCE:        $" << balance << "\n";
    std::cout << "OWNERS:\n";
    for (const auto& c : owners) {
        std::cout << "  - " << c->getFullName() << "\n";
    }
    std::cout << "====================================\n";
}

void Account::printStatement() const {
    std::cout << "--- Transaction Statement for " << accountNumber << " ---\n";
    for (const auto& tx : transactionHistory) {
        tx.display();
    }
}

void Account::freezeAccount() { status = AccountStatus::Frozen; }
void Account::closeAccount()  { status = AccountStatus::Closed; }
