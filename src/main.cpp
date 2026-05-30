#include "BankSystem.hpp"
#include "MainWindow.hpp"
#include <QApplication>
#include <iostream>
#include <string>

// terminal interface
void showMenu() {
    std::cout << "\n==== MINI BANK SYSTEM ====\n";
    std::cout << "1. Create New Customer\n";
    std::cout << "2. Open Account for Existing Customer\n";
    std::cout << "3. Deposit Funds\n";
    std::cout << "4. Withdraw Funds\n";
    std::cout << "5. Transfer Money\n";
    std::cout << "6. Display All Accounts\n";
    std::cout << "7. Display Customer Accounts\n";
    std::cout << "8. Exit\n";
    std::cout << "Choose an option: ";
}

void runTerminalInterface(BankSystem& bank) {
    int choice = 0;

    while (true) {
        showMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 8) {
            std::cout << "Exiting system. Goodbye!\n";
            break;
        }

        switch (choice) {

            case 1: {
                // Tworzenie nowego klienta + pierwsze konto
                std::string first, last, phone, email;
                std::cout << "Enter First Name: "; std::cin >> first;
                std::cout << "Enter Last Name: ";  std::cin >> last;
                std::cout << "Enter Phone: ";      std::cin >> phone;
                std::cout << "Enter Email: ";      std::cin >> email;

                auto customer = bank.createCustomer(first, last, phone, email);

                std::cout << "Account type: [1] Basic  [2] Savings  [3] Checking: ";
                int typeChoice = 1;
                std::cin >> typeChoice;

                std::shared_ptr<Account> acc;
                if (typeChoice == 2)
                    acc = bank.createSavingsAccount(customer);
                else if (typeChoice == 3)
                    acc = bank.createCheckingAccount(customer);
                else
                    acc = bank.createAccount(customer);

                std::cout << "Success! Account Created: " << acc->getAccountNumber() << "\n";
                break;
            }

            case 2: {
                // Otwieranie kolejnego konta dla istniejącego klienta
                std::string fullName;
                std::cout << "Enter customer full name (First Last): ";
                std::cin.ignore();
                std::getline(std::cin, fullName);

                auto customer = bank.findCustomer(fullName);
                if (!customer) {
                    std::cout << "Customer not found.\n";
                    break;
                }

                std::cout << "Account type: [1] Basic  [2] Savings  [3] Checking: ";
                int typeChoice = 1;
                std::cin >> typeChoice;

                std::shared_ptr<Account> acc;
                if (typeChoice == 2)
                    acc = bank.createSavingsAccount(customer);
                else if (typeChoice == 3)
                    acc = bank.createCheckingAccount(customer);
                else
                    acc = bank.createAccount(customer);

                std::cout << "New account opened: " << acc->getAccountNumber() << "\n";
                break;
            }

            case 3: {
                std::string accNum; double amount;
                std::cout << "Account Number: "; std::cin >> accNum;
                std::cout << "Amount to Deposit: $"; std::cin >> amount;
                if (bank.depositToAccount(accNum, amount)) std::cout << "Deposit Successful!\n";
                else std::cout << "Deposit Failed.\n";
                break;
            }

            case 4: {
                std::string accNum; double amount;
                std::cout << "Account Number: "; std::cin >> accNum;
                std::cout << "Amount to Withdraw: $"; std::cin >> amount;
                if (bank.withdrawFromAccount(accNum, amount)) std::cout << "Withdrawal Successful!\n";
                else std::cout << "Withdrawal Failed.\n";
                break;
            }

            case 5: {
                std::string fromAcc, toAcc; double amount;
                std::cout << "From Account Number: "; std::cin >> fromAcc;
                std::cout << "To Account Number: ";   std::cin >> toAcc;
                std::cout << "Amount to Transfer: $"; std::cin >> amount;
                if (bank.transferFunds(fromAcc, toAcc, amount)) std::cout << "Transfer Successful!\n";
                else std::cout << "Transfer Failed.\n";
                break;
            }

            case 6:
                bank.displayAllAccounts();
                break;

            case 7: {
                std::string fullName;
                std::cout << "Enter customer full name (First Last): ";
                std::cin.ignore();
                std::getline(std::cin, fullName);
                bank.displayCustomerAccounts(fullName);
                break;
            }

            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    }
}

int main(int argc, char* argv[]) {
    BankSystem bank;

    // Demo: Anna ma dwa różne konta
    auto anna = bank.createCustomer("Anna", "Kowalska", "123456789", "anna@email.com");
    auto annaSavings  = bank.createSavingsAccount(anna);
    auto annaChecking = bank.createCheckingAccount(anna);
    annaSavings->deposit(2000.0);
    annaChecking->deposit(500.0);

    bool useQt = false;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--gui" || arg == "-g") { useQt = true; break; }
    }

    if (useQt) {
        QApplication app(argc, argv);
        auto window = std::make_shared<MainWindow>(std::make_shared<BankSystem>(bank));
        window->show();
        return app.exec();
    } else {
        runTerminalInterface(bank);
    }

    return 0;
}
