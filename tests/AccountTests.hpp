#pragma once
#include "Account.hpp"
#include "Customer.hpp"
#include <cassert>
#include <iostream>
#include <memory>

inline void runAccountTestSuite() {
    auto client = std::make_shared<Customer>("John", "Doe", "111", "j@mail.com");
    Account acc(client);

    // Test deposit boundaries
    assert(acc.deposit(500.0) == true);
    assert(acc.getBalance() == 500.0);
    assert(acc.deposit(-100.0) == false); // Enforce protection against negative values

    // Test transaction safety restrictions
    assert(acc.withdraw(600.0) == false); // Overdraft check
    assert(acc.withdraw(200.0) == true);
    assert(acc.getBalance() == 300.0);

    std::cout << " [PASS] Account Test Suite\n";
}