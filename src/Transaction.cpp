#include "Transaction.hpp"
#include <iostream>
#include <utility>

#include "Utils.hpp"

Transaction::Transaction(const TransactionType type, const double amount, std::string  description)
    : type(type), amount(amount), description(std::move(description)) {
    timestamp = getCurrentTimestamp();
}

void Transaction::display() const {
    std::string typeStr;
    switch(type) {
        case TransactionType::Deposit:     typeStr = "[DEPOSIT]     "; break;
        case TransactionType::Withdrawal:  typeStr = "[WITHDRAWAL]  "; break;
        case TransactionType::TransferIn:  typeStr = "[TRANSFER IN] "; break;
        case TransactionType::TransferOut: typeStr = "[TRANSFER OUT]"; break;
    }
    std::cout << timestamp << " | " << typeStr << " | Amount: $" << amount << " | " << description << "\n";
}