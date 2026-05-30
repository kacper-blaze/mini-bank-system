#include "CheckingAccount.hpp"
#include <iostream>

CheckingAccount::CheckingAccount(std::shared_ptr<Customer> owner,
                                  double overdraftLimit,
                                  double monthlyFee)
    : Account(std::move(owner), AccountType::Checking),
      overdraftLimit(overdraftLimit),
      monthlyFee(monthlyFee) {}

bool CheckingAccount::withdraw(double amount,
                                const std::string& description,
                                TransactionType type) {
    if (status != AccountStatus::Active) return false;
    if (amount <= 0) return false;

    // Dozwolony debet: saldo może spaść do -overdraftLimit
    if (balance - amount < -overdraftLimit) {
        std::cout << "Withdrawal denied: would exceed overdraft limit of $"
                  << overdraftLimit << ".\n";
        return false;
    }

    balance -= amount;
    transactionHistory.emplace_back(type, amount, description);
    return true;
}

bool CheckingAccount::chargeMonthlyFee() {
    if (status != AccountStatus::Active) return false;

    // Opłata pobierana nawet jeśli konto jest na minusie (do limitu debetu)
    if (balance - monthlyFee < -overdraftLimit) {
        std::cout << "Monthly fee of $" << monthlyFee
                  << " could not be charged (would exceed overdraft limit).\n";
        return false;
    }

    balance -= monthlyFee;
    transactionHistory.emplace_back(TransactionType::Withdrawal, monthlyFee, "Monthly Account Fee");
    std::cout << "Monthly fee of $" << monthlyFee << " charged to " << getAccountNumber() << ".\n";
    return true;
}

double CheckingAccount::getOverdraftLimit() const { return overdraftLimit; }
double CheckingAccount::getMonthlyFee() const     { return monthlyFee; }

void CheckingAccount::display() const {
    std::cout << "====================================\n";
    std::cout << "ACCOUNT NUMBER:  " << getAccountNumber() << "\n";
    std::cout << "TYPE:            Checking (ROR)\n";
    std::cout << "BALANCE:         $" << balance << "\n";
    std::cout << "OVERDRAFT LIMIT: $" << overdraftLimit << "\n";
    std::cout << "MONTHLY FEE:     $" << monthlyFee << "\n";
    std::cout << "OWNERS:\n";
    for (const auto& c : getOwner()) {
        std::cout << "  - " << c->getFullName() << "\n";
    }
    std::cout << "====================================\n";
}
