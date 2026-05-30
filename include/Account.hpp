#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Customer.hpp"
#include "Transaction.hpp"

enum class AccountStatus {
    Active,
    Frozen,
    Closed
};

enum class AccountType {
    Basic,
    Savings,
    Checking
};

class Account {
public:
    explicit Account(std::shared_ptr<Customer> accountOwner, AccountType type = AccountType::Basic);
    virtual ~Account() = default;

    // Core financial operations - virtual so subclasses can override behaviour
    virtual bool deposit(double amount,
                         const std::string& description = "Cash Deposit",
                         TransactionType type = TransactionType::Deposit);
    virtual bool withdraw(double amount,
                          const std::string& description = "Cash Withdrawal",
                          TransactionType type = TransactionType::Withdrawal);

    // Multi-owner management
    void addOwner(const std::shared_ptr<Customer>& newOwner);
    void removeOwner(const std::shared_ptr<Customer>& owner);

    // Getters
    double getBalance() const;
    std::string getAccountNumber() const;
    AccountType getAccountType() const;
    std::string getAccountTypeString() const;
    static int getAccountCount();
    std::vector<std::shared_ptr<Customer>> getOwner() const;

    // Display
    virtual void display() const;
    void printStatement() const;

    // State management
    void freezeAccount();
    void closeAccount();

protected:
    double balance;
    AccountStatus status;
    std::vector<Transaction> transactionHistory;

private:
    std::string accountNumber;
    std::vector<std::shared_ptr<Customer>> owners;
    AccountType accountType;
    static int accountCount;
};
