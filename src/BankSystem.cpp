#include "BankSystem.hpp"
#include "Logger.hpp"
#include <iostream>

// register account
void BankSystem::registerAccount(const std::shared_ptr<Account>& account,
                                  const std::shared_ptr<Customer>& owner) {
    accounts.push_back(account);
    owner->addAccount(account);           // klient wie o swoim nowym koncie
    Logger::log("SYSTEM: Created account " + account->getAccountNumber() +
                " [" + account->getAccountTypeString() + "] for " + owner->getFullName());
}

// register customer
std::shared_ptr<Customer> BankSystem::createCustomer(const std::string& first,
                                                      const std::string& last,
                                                      const std::string& phone,
                                                      const std::string& email) {
    auto customer = std::make_shared<Customer>(first, last, phone, email);
    customers.push_back(customer);
    Logger::log("SYSTEM: Registered new customer: " + customer->getFullName());
    return customer;
}

std::shared_ptr<Customer> BankSystem::findCustomer(const std::string& fullName) const {
    for (const auto& c : customers) {
        if (c->getFullName() == fullName) return c;
    }
    return nullptr;
}

// create accounts
std::shared_ptr<Account> BankSystem::createAccount(const std::shared_ptr<Customer>& owner) {
    auto account = std::make_shared<Account>(owner, AccountType::Basic);
    registerAccount(account, owner);
    return account;
}

std::shared_ptr<SavingsAccount> BankSystem::createSavingsAccount(
        const std::shared_ptr<Customer>& owner, double interestRate) {
    auto account = std::make_shared<SavingsAccount>(owner, interestRate);
    registerAccount(account, owner);
    return account;
}

std::shared_ptr<CheckingAccount> BankSystem::createCheckingAccount(
        const std::shared_ptr<Customer>& owner, double overdraftLimit, double monthlyFee) {
    auto account = std::make_shared<CheckingAccount>(owner, overdraftLimit, monthlyFee);
    registerAccount(account, owner);
    return account;
}

// financial operations
std::shared_ptr<Account> BankSystem::findAccount(const std::string& accNum) const {
    for (const auto& acc : accounts) {
        if (acc->getAccountNumber() == accNum) return acc;
    }
    return nullptr;
}

bool BankSystem::depositToAccount(const std::string& accNum, double amount) {
    auto acc = findAccount(accNum);
    if (acc && acc->deposit(amount)) {
        Logger::log("TX SUCCESS: Deposited $" + std::to_string(amount) + " into " + accNum);
        return true;
    }
    Logger::log("TX FAILED: Deposit into " + accNum + " failed.");
    return false;
}

bool BankSystem::withdrawFromAccount(const std::string& accNum, double amount) {
    auto acc = findAccount(accNum);
    if (acc && acc->withdraw(amount)) {
        Logger::log("TX SUCCESS: Withdrew $" + std::to_string(amount) + " from " + accNum);
        return true;
    }
    Logger::log("TX FAILED: Withdrawal from " + accNum + " failed.");
    return false;
}

bool BankSystem::transferFunds(const std::string& fromAccNum,
                                const std::string& toAccNum,
                                double amount) {
    const auto fromAcc = findAccount(fromAccNum);
    const auto toAcc   = findAccount(toAccNum);

    if (!fromAcc || !toAcc || amount <= 0 || fromAcc->getBalance() < amount) {
        Logger::log("TX FAILED: Transfer from " + fromAccNum + " to " + toAccNum + " failed validation.");
        return false;
    }

    if (fromAcc->withdraw(amount, "Transfer to " + toAccNum, TransactionType::TransferOut)) {
        toAcc->deposit(amount, "Transfer from " + fromAccNum, TransactionType::TransferIn);
        Logger::log("TX SUCCESS: Transferred $" + std::to_string(amount) +
                    " from " + fromAccNum + " to " + toAccNum);
        return true;
    }
    return false;
}

// display
void BankSystem::displayAllAccounts() const {
    std::cout << "\n=== CENTRAL BANK REGISTRY ===\n";
    for (const auto& acc : accounts) {
        acc->display();
    }
}

void BankSystem::displayCustomerAccounts(const std::string& fullName) const {
    auto customer = findCustomer(fullName);
    if (!customer) {
        std::cout << "Customer not found: " << fullName << "\n";
        return;
    }
    std::cout << "\n=== Accounts for " << fullName << " ===\n";
    for (const auto& acc : customer->getAccounts()) {
        acc->display();
    }
}

const std::vector<std::shared_ptr<Account>>&  BankSystem::getAccounts()  const { return accounts; }
const std::vector<std::shared_ptr<Customer>>& BankSystem::getCustomers() const { return customers; }
