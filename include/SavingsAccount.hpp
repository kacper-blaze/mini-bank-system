#pragma once
#include "Account.hpp"

/**
 * @brief Konto oszczędnościowe.
 *
 * Cechy:
 *  - Oprocentowanie: naliczane przez applyInterest() wg rocznej stopy procentowej
 *  - Limit wypłat: maksymalnie MAX_DAILY_WITHDRAWALS wypłat dziennie
 *  - Brak debetu: withdraw() nigdy nie zejdzie poniżej 0
 */
class SavingsAccount : public Account {
public:
    static constexpr int    MAX_DAILY_WITHDRAWALS = 3;
    static constexpr double DEFAULT_INTEREST_RATE = 0.04; // 4% rocznie

    explicit SavingsAccount(std::shared_ptr<Customer> owner,
                            double annualInterestRate = DEFAULT_INTEREST_RATE);

    /**
     * @brief Nalicza odsetki na podstawie aktualnego salda.
     *        Wywołać raz w miesiącu / raz w roku zależnie od potrzeb.
     * @return Kwota doliczona do salda.
     */
    double applyInterest();

    /**
     * @brief Nadpisuje withdraw() - sprawdza dzienny limit wypłat.
     */
    bool withdraw(double amount,
                  const std::string& description = "Cash Withdrawal",
                  TransactionType type = TransactionType::Withdrawal) override;

    void display() const override;

    int getRemainingWithdrawals() const;
    double getInterestRate() const;

    /**
     * @brief Resetuje licznik wypłat - należy wywoływać raz dziennie.
     */
    void resetDailyWithdrawalCount();

private:
    double annualInterestRate;
    int dailyWithdrawalCount;
};
