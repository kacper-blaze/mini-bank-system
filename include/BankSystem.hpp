#pragma once
#include "Account.hpp"
#include "Customer.hpp"
#include <vector>
#include <memory>
#include <string>

class BankSystem {
public:
    BankSystem() = default;

    // Core System operations
    std::shared_ptr<Customer> createCustomer(const std::string& first, const std::string& last, const std::string& phone, const std::string& email);
    std::shared_ptr<Account> createAccount(const std::shared_ptr<Customer>& primaryOwner);

    // Financial transactions routing
    bool depositToAccount(const std::string& accNum, double amount);
    bool withdrawFromAccount(const std::string& accNum, double amount);
    bool transferFunds(const std::string& fromAccNum, const std::string& toAccNum, double amount);

    // Lookups
    std::shared_ptr<Account> findAccount(const std::string& accNum) const;
    void displayAllAccounts() const;
    const std::vector<std::shared_ptr<Account>>& getAccounts() const;

private:
    std::vector<std::shared_ptr<Customer>> customers;
    std::vector<std::shared_ptr<Account>> accounts;
};