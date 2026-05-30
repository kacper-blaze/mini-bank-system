#pragma once
#include "Account.hpp"

/**
 * @brief Konto rozliczeniowe (ROR).
 *
 * Cechy:
 *  - Debet: saldo może zejść do -overdraftLimit (domyślnie -500)
 *  - Opłata miesięczna: chargeMonthlyFee() pobiera stałą kwotę
 *  - Wszystkie inne zasady jak w koncie bazowym
 */
class CheckingAccount : public Account {
public:
    static constexpr double DEFAULT_OVERDRAFT_LIMIT = 500.0;
    static constexpr double DEFAULT_MONTHLY_FEE     = 9.99;

    explicit CheckingAccount(std::shared_ptr<Customer> owner,
                             double overdraftLimit = DEFAULT_OVERDRAFT_LIMIT,
                             double monthlyFee     = DEFAULT_MONTHLY_FEE);

    /**
     * @brief Nadpisuje withdraw() — pozwala na debet do overdraftLimit.
     */
    bool withdraw(double amount,
                  const std::string& description = "Cash Withdrawal",
                  TransactionType type = TransactionType::Withdrawal) override;

    /**
     * @brief Pobiera miesięczną opłatę za prowadzenie konta.
     * @return true jeśli opłata została pobrana, false jeśli konto zamknięte/zamrożone.
     */
    bool chargeMonthlyFee();

    void display() const override;

    double getOverdraftLimit() const;
    double getMonthlyFee() const;

private:
    double overdraftLimit; // Maksymalna kwota debetu (wartość dodatnia, np. 500)
    double monthlyFee;
};
