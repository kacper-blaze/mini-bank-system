#include "BankSystem.hpp"
#include "MainWindow.hpp"
#include <QApplication>
#include <iostream>
#include <string>

void showMenu() {
    std::cout << "\n==== MINI BANK SYSTEM ====\n";
    std::cout << "1. Create New Account\n";
    std::cout << "2. Deposit Funds\n";
    std::cout << "3. Withdraw Funds\n";
    std::cout << "4. Transfer Money\n";
    std::cout << "5. Display All Accounts\n";
    std::cout << "6. Exit\n";
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

        if (choice == 6) {
            std::cout << "Exiting system. Goodbye!\n";
            break;
        }

        switch (choice) {
            case 1: {
                std::string first, last, phone, email;
                std::cout << "Enter First Name: "; std::cin >> first;
                std::cout << "Enter Last Name: ";  std::cin >> last;
                std::cout << "Enter Phone: ";      std::cin >> phone;
                std::cout << "Enter Email: ";      std::cin >> email;

                auto customer = bank.createCustomer(first, last, phone, email);
                auto acc = bank.createAccount(customer);
                std::cout << "Success! Account Created: " << acc->getAccountNumber() << "\n";
                break;
            }
            case 2: {
                std::string accNum;
                double amount;
                std::cout << "Account Number: "; std::cin >> accNum;
                std::cout << "Amount to Deposit: $"; std::cin >> amount;
                if (bank.depositToAccount(accNum, amount)) std::cout << "Deposit Successful!\n";
                else std::cout << "Deposit Failed.\n";
                break;
            }
            case 3: {
                std::string accNum;
                double amount;
                std::cout << "Account Number: "; std::cin >> accNum;
                std::cout << "Amount to Withdraw: $"; std::cin >> amount;
                if (bank.withdrawFromAccount(accNum, amount)) std::cout << "Withdrawal Successful!\n";
                else std::cout << "Withdrawal Failed.\n";
                break;
            }
            case 4: {
                std::string fromAcc, toAcc;
                double amount;
                std::cout << "From Account Number: "; std::cin >> fromAcc;
                std::cout << "To Account Number: ";   std::cin >> toAcc;
                std::cout << "Amount to Transfer: $";  std::cin >> amount;
                if (bank.transferFunds(fromAcc, toAcc, amount)) std::cout << "Transfer Successful!\n";
                else std::cout << "Transfer Failed.\n";
                break;
            }
            case 5: {
                for (const auto& acc : bank.getAccounts()) {
                    std::cout << acc->getAccountNumber() << ": " << acc->getBalance() << "\n";
                }
                break;
            }
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    }
}

int main(int argc, char *argv[]) {
    BankSystem bank;

    // Seed some initial demo data so the app isn't completely empty at boot
    auto demoUser = bank.createCustomer("Anna", "Kowalska", "123456789", "anna@email.com");
    bank.createAccount(demoUser);

    // Check for interface selection
    bool useQt = false;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--gui" || arg == "-g") {
            useQt = true;
            break;
        }
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