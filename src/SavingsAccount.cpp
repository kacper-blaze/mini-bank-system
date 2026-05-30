#include "SavingsAccount.hpp"
#include <iostream>

SavingsAccount::SavingsAccount(std::shared_ptr<Customer> owner, double rate)
    : Account(std::move(owner), AccountType::Savings),
      annualInterestRate(rate),
      dailyWithdrawalCount(0) {}

bool SavingsAccount::withdraw(double amount,
                               const std::string& description,
                               TransactionType type) {
    if (status != AccountStatus::Active) return false;

    if (dailyWithdrawalCount >= MAX_DAILY_WITHDRAWALS) {
        std::cout << "Withdrawal denied: daily limit of " << MAX_DAILY_WITHDRAWALS
                  << " withdrawals reached.\n";
        return false;
    }

    // Konto oszczędnościowe nie pozwala na debet
    if (amount <= 0 || balance < amount) return false;

    balance -= amount;
    transactionHistory.emplace_back(type, amount, description);
    dailyWithdrawalCount++;
    return true;
}

double SavingsAccount::applyInterest() {
    if (status != AccountStatus::Active || balance <= 0) return 0.0;

    // Miesięczna stopa = roczna / 12
    double interest = balance * (annualInterestRate / 12.0);
    balance += interest;
    transactionHistory.emplace_back(TransactionType::Deposit, interest, "Monthly Interest");
    std::cout << "Interest applied: $" << interest
              << " (rate: " << annualInterestRate * 100 << "% p.a.)\n";
    return interest;
}

void SavingsAccount::resetDailyWithdrawalCount() {
    dailyWithdrawalCount = 0;
}

int SavingsAccount::getRemainingWithdrawals() const {
    return MAX_DAILY_WITHDRAWALS - dailyWithdrawalCount;
}

double SavingsAccount::getInterestRate() const { return annualInterestRate; }

void SavingsAccount::display() const {
    std::cout << "====================================\n";
    std::cout << "ACCOUNT NUMBER:  " << getAccountNumber() << "\n";
    std::cout << "TYPE:            Savings\n";
    std::cout << "BALANCE:         $" << balance << "\n";
    std::cout << "INTEREST RATE:   " << annualInterestRate * 100 << "% p.a.\n";
    std::cout << "WITHDRAWALS LEFT:" << getRemainingWithdrawals()
              << " / " << MAX_DAILY_WITHDRAWALS << " today\n";
    std::cout << "OWNERS:\n";
    for (const auto& c : getOwner()) {
        std::cout << "  - " << c->getFullName() << "\n";
    }
    std::cout << "====================================\n";
}
