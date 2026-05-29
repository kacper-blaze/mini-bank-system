#include "BankSystem.hpp"
#include "Logger.hpp"
#include <iostream>

std::shared_ptr<Customer> BankSystem::createCustomer(const std::string& first, const std::string& last, const std::string& phone, const std::string& email) {
    auto customer = std::make_shared<Customer>(first, last, phone, email);
    customers.push_back(customer);

    Logger::log("SYSTEM: Registered new customer: " + customer->getFullName());
    return customer;
}

std::shared_ptr<Account> BankSystem::createAccount(const std::shared_ptr<Customer>& primaryOwner) {
    auto account = std::make_shared<Account>(primaryOwner);
    accounts.push_back(account);

    Logger::log("SYSTEM: Created account " + account->getAccountNumber() + " for " + primaryOwner->getFullName());
    return account;
}

std::shared_ptr<Account> BankSystem::findAccount(const std::string& accNum) const {
    for (const auto& acc : accounts) {
        if (acc->getAccountNumber() == accNum) {
            return acc;
        }
    }
    return nullptr; // Not found
}

bool BankSystem::depositToAccount(const std::string& accNum, const double amount) {
    const auto acc = findAccount(accNum);
    if (acc && acc->deposit(amount)) {
        Logger::log("TX SUCCESS: Deposited $" + std::to_string(amount) + " into " + accNum);
        return true;
    }
    Logger::log("TX FAILED: Attempted deposit into " + accNum + " failed.");
    return false;
}

bool BankSystem::withdrawFromAccount(const std::string& accNum, const double amount) {
    auto acc = findAccount(accNum);
    if (acc && acc->withdraw(amount)) {
        Logger::log("TX SUCCESS: Withdrew $" + std::to_string(amount) + " from " + accNum);
        return true;
    }
    Logger::log("TX FAILED: Attempted withdrawal from " + accNum + " failed (Insufficient funds or frozen).");
    return false;
}

bool BankSystem::transferFunds(const std::string& fromAccNum, const std::string& toAccNum, const double amount) {
    const auto fromAcc = findAccount(fromAccNum);
    const auto toAcc = findAccount(toAccNum);

    // 2. Strict Validation Block (Check existence, amounts, and balance FIRST)
    if (!fromAcc || !toAcc || amount <= 0 || fromAcc->getBalance() < amount) {
        Logger::log("TX FAILED: Transfer from " + fromAccNum + " to " + toAccNum + " failed validation.");
        return false;
    }

    // 3. Execution Block (Atomic operation with custom statement descriptions)
    if (fromAcc->withdraw(amount, "Transfer to " + toAccNum)) {
        toAcc->deposit(amount, "Transfer from " + fromAccNum);

        Logger::log("TX SUCCESS: Transferred $" + std::to_string(amount) + " from " + fromAccNum + " to " + toAccNum);
        return true;
    }

    return false;
}

void BankSystem::displayAllAccounts() const {
    std::cout << "\n=== CENTRAL BANK REGISTRY ===\n";
    for (const auto& acc : accounts) {
        acc->display();
    }
}

const std::vector<std::shared_ptr<Account>>& BankSystem::getAccounts() const {
    return accounts;
}