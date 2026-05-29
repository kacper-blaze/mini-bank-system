#pragma once
#include "BankSystem.hpp"
#include <cassert>
#include <iostream>

inline void runBankSystemTestSuite() {
    BankSystem bank;
    auto c1 = bank.createCustomer("A", "B", "1", "a@m.com");
    auto c2 = bank.createCustomer("C", "D", "2", "c@m.com");

    auto acc1 = bank.createAccount(c1);
    auto acc2 = bank.createAccount(c2);

    bank.depositToAccount(acc1->getAccountNumber(), 1000.0);

    // Test Transfer execution framework
    bool transferSuccess = bank.transferFunds(acc1->getAccountNumber(), acc2->getAccountNumber(), 400.0);
    assert(transferSuccess == true);
    assert(acc1->getBalance() == 600.0);
    assert(acc2->getBalance() == 400.0);

    std::cout << " [PASS] BankSystem Test Suite\n";
}