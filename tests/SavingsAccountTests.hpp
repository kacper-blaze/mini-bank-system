#pragma once
#include "SavingsAccount.hpp"
#include "Customer.hpp"
#include <cassert>
#include <iostream>
#include <memory>

inline void runSavingsAccountTestSuite() {
    auto client = std::make_shared<Customer>("Eva", "Green", "111", "e@mail.com");
    SavingsAccount acc(client, 0.12); // 12% rocznie = 1% miesięcznie

    // Podstawowe operacje
    assert(acc.deposit(1000.0) == true);
    assert(acc.getBalance() == 1000.0);

    // Odsetki: 1000 * 1% = 10
    double interest = acc.applyInterest();
    assert(interest == 10.0);
    assert(acc.getBalance() == 1010.0);

    // Limit wypłat dziennych
    assert(acc.withdraw(10.0) == true);   // 1
    assert(acc.withdraw(10.0) == true);   // 2
    assert(acc.withdraw(10.0) == true);   // 3
    assert(acc.withdraw(10.0) == false);  // 4 - zablokowane
    assert(acc.getRemainingWithdrawals() == 0);

    acc.resetDailyWithdrawalCount();
    assert(acc.getRemainingWithdrawals() == SavingsAccount::MAX_DAILY_WITHDRAWALS);

    // Brak debetu
    assert(acc.withdraw(999999.0) == false);

    std::cout << " [PASS] SavingsAccount Test Suite\n";
}
