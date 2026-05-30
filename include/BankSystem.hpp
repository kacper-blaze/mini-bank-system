#pragma once
#include "Account.hpp"
#include "CheckingAccount.hpp"
#include "Customer.hpp"
#include "SavingsAccount.hpp"
#include <memory>
#include <string>
#include <vector>

class BankSystem {
public:
    BankSystem() = default;

    // client registration
    std::shared_ptr<Customer> createCustomer(const std::string& first,
                                             const std::string& last,
                                             const std::string& phone,
                                             const std::string& email);
    std::shared_ptr<Customer> findCustomer(const std::string& fullName) const;

    // account creation
    std::shared_ptr<Account>         createAccount(const std::shared_ptr<Customer>& owner);
    std::shared_ptr<SavingsAccount>  createSavingsAccount(const std::shared_ptr<Customer>& owner,
                                                           double interestRate = SavingsAccount::DEFAULT_INTEREST_RATE);
    std::shared_ptr<CheckingAccount> createCheckingAccount(const std::shared_ptr<Customer>& owner,
                                                            double overdraftLimit = CheckingAccount::DEFAULT_OVERDRAFT_LIMIT,
                                                            double monthlyFee     = CheckingAccount::DEFAULT_MONTHLY_FEE);

    // financial operations
    bool depositToAccount(const std::string& accNum, double amount);
    bool withdrawFromAccount(const std::string& accNum, double amount);
    bool transferFunds(const std::string& fromAccNum, const std::string& toAccNum, double amount);

    // find
    std::shared_ptr<Account> findAccount(const std::string& accNum) const;

    // display
    void displayAllAccounts() const;
    void displayCustomerAccounts(const std::string& fullName) const;

    const std::vector<std::shared_ptr<Account>>&   getAccounts()  const;
    const std::vector<std::shared_ptr<Customer>>&  getCustomers() const;

private:
    std::vector<std::shared_ptr<Customer>> customers;
    std::vector<std::shared_ptr<Account>>  accounts;

    void registerAccount(const std::shared_ptr<Account>& account,
                         const std::shared_ptr<Customer>& owner);
};
