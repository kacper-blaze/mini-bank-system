#pragma once
#include "CheckingAccount.hpp"
#include "Customer.hpp"
#include <cassert>
#include <iostream>
#include <memory>

inline void runCheckingAccountTestSuite() {
    auto client = std::make_shared<Customer>("Tom", "Black", "222", "t@mail.com");
    CheckingAccount acc(client, 500.0, 9.99); // debet do 500, opłata 9.99

    acc.deposit(100.0);

    // Debet: można wypłacić 100 + 500 = max 600
    assert(acc.withdraw(400.0) == true);  // saldo: -300
    assert(acc.getBalance() == -300.0);

    // Przekroczenie limitu debetu
    assert(acc.withdraw(300.0) == false); // -300 - 300 = -600 > limit

    // Opłata miesięczna
    acc.deposit(400.0); // saldo: 100
    assert(acc.chargeMonthlyFee() == true);
    assert(acc.getBalance() == 100.0 - 9.99);

    std::cout << " [PASS] CheckingAccount Test Suite\n";
}
