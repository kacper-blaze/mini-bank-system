#pragma once
#include <string>

enum class TransactionType {
    Deposit,
    Withdrawal,
    TransferIn,
    TransferOut
};

class Transaction {
public:
    Transaction(TransactionType type, double amount, std::string  description);

    void display() const;

private:
    TransactionType type;
    double amount;
    std::string description;
    std::string timestamp;
};