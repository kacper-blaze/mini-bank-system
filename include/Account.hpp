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

class Account {
public:
    Account(std::shared_ptr<Customer> accountOwner);
    bool deposit(double amount, const std::string& description = "Cash Deposit", TransactionType type = TransactionType::Deposit);
    bool withdraw(double amount, const std::string& description = "Cash Withdrawal", TransactionType type = TransactionType::Withdrawal);

    void addOwner(const std::shared_ptr<Customer> &newOwner);
    void removeOwner(const std::shared_ptr<Customer> &owner);

    double getBalance() const;
    std::string getAccountNumber() const;
    static int getAccountCount();
    std::vector<std::shared_ptr<Customer>> getOwner() const;

    void display() const;
    void printStatement() const;
    void freezeAccount();
    void closeAccount();

private:
    std::string accountNumber;
    std::vector<std::shared_ptr<Customer>> owners;
    double balance;
    std::vector<Transaction> transactionHistory;
    static int accountCount;
    AccountStatus status = AccountStatus::Active;
};
