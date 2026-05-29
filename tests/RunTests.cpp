#include "AccountTests.hpp"
#include "BankSystemTests.hpp"
#include <iostream>

int main() {
    std::cout << "====================================\n";
    std::cout << "  LAUNCHING AUTOMATED AUDIT TESTS   \n";
    std::cout << "====================================\n";

    runAccountTestSuite();
    runBankSystemTestSuite();

    std::cout << "====================================\n";
    std::cout << "  SUCCESS: All Code Components Safe \n";
    std::cout << "====================================\n";

    return 0;
}