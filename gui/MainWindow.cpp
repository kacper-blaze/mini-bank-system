#include "MainWindow.hpp"
#include <QApplication>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QSplitter>

MainWindow::MainWindow(std::shared_ptr<BankSystem> bank, QWidget* parent)
    : QMainWindow(parent), bank(std::move(bank)) {
    setupUI();
    applyStyle();
    rebuildAccountTable();
    setWindowTitle("Mini Bank System");
    resize(900, 650);
}

MainWindow::~MainWindow() {}

void MainWindow::applyStyle() {
    qApp->setStyleSheet(R"(
        QMainWindow, QWidget {
            background-color: #1a1a2e;
            color: #e2e8f0;
            font-family: "Segoe UI", sans-serif;
            font-size: 13px;
        }
        QGroupBox {
            border: 1px solid #3f3f6e;
            border-radius: 8px;
            margin-top: 10px;
            padding: 10px;
            font-weight: bold;
            color: #818cf8;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 4px;
        }
        QLineEdit, QComboBox {
            background: #16213e;
            color: #e2e8f0;
            border: 1px solid #3f3f6e;
            border-radius: 5px;
            padding: 5px 8px;
            min-height: 22px;
        }
        QLineEdit:focus, QComboBox:focus {
            border-color: #818cf8;
        }
        QPushButton {
            background-color: #4f46e5;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 7px 16px;
            font-weight: bold;
            min-width: 80px;
        }
        QPushButton:hover   { background-color: #6366f1; }
        QPushButton:pressed { background-color: #3730a3; }
        QPushButton#dangerBtn {
            background-color: #dc2626;
        }
        QPushButton#dangerBtn:hover { background-color: #ef4444; }
        QTableWidget {
            background: #16213e;
            gridline-color: #2d2d5e;
            border: 1px solid #3f3f6e;
            border-radius: 6px;
        }
        QTableWidget::item { padding: 4px 8px; }
        QTableWidget::item:selected {
            background: #4f46e5;
            color: white;
        }
        QHeaderView::section {
            background-color: #0f0f23;
            color: #818cf8;
            padding: 6px 8px;
            border: none;
            border-bottom: 1px solid #3f3f6e;
            font-weight: bold;
        }
        QTextEdit {
            background: #0f0f23;
            color: #a3e635;
            font-family: "Courier New", monospace;
            font-size: 12px;
            border: 1px solid #3f3f6e;
            border-radius: 6px;
            padding: 6px;
        }
        QSplitter::handle { background: #3f3f6e; }
        QScrollBar:vertical {
            background: #16213e;
            width: 8px;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical {
            background: #4f46e5;
            border-radius: 4px;
            min-height: 20px;
        }
    )");
}

// UI Setup

void MainWindow::setupUI() {
    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* root = new QVBoxLayout(central);
    root->setContentsMargins(12, 12, 12, 12);
    root->setSpacing(10);

    // upper panel
    QHBoxLayout* topRow = new QHBoxLayout();
    topRow->setSpacing(10);

    // new account
    QGroupBox* createBox = new QGroupBox("New Account");
    QVBoxLayout* createLayout = new QVBoxLayout(createBox);
    createLayout->setSpacing(6);

    firstNameEdit = new QLineEdit(); firstNameEdit->setPlaceholderText("First Name");
    lastNameEdit  = new QLineEdit(); lastNameEdit->setPlaceholderText("Last Name");
    phoneEdit     = new QLineEdit(); phoneEdit->setPlaceholderText("Phone");
    emailEdit     = new QLineEdit(); emailEdit->setPlaceholderText("Email");

    accountTypeCombo = new QComboBox();
    accountTypeCombo->addItem("Basic",    0);
    accountTypeCombo->addItem("Savings",  1);
    accountTypeCombo->addItem("Checking", 2);

    QPushButton* createBtn = new QPushButton("Create Account");
    connect(createBtn, &QPushButton::clicked, this, &MainWindow::onCreateAccount);

    createLayout->addWidget(firstNameEdit);
    createLayout->addWidget(lastNameEdit);
    createLayout->addWidget(phoneEdit);
    createLayout->addWidget(emailEdit);
    createLayout->addWidget(accountTypeCombo);
    createLayout->addWidget(createBtn);

    // Operations
    QGroupBox* opsBox = new QGroupBox("Operations");
    QVBoxLayout* opsLayout = new QVBoxLayout(opsBox);
    opsLayout->setSpacing(6);

    accountNumEdit   = new QLineEdit(); accountNumEdit->setPlaceholderText("Account Number (e.g. BNK-1)");
    toAccountNumEdit = new QLineEdit(); toAccountNumEdit->setPlaceholderText("To Account (transfer only)");
    amountEdit       = new QLineEdit(); amountEdit->setPlaceholderText("Amount");

    QPushButton* depositBtn  = new QPushButton("Deposit");
    QPushButton* withdrawBtn = new QPushButton("Withdraw");
    QPushButton* transferBtn = new QPushButton("Transfer");

    connect(depositBtn,  &QPushButton::clicked, this, &MainWindow::onDeposit);
    connect(withdrawBtn, &QPushButton::clicked, this, &MainWindow::onWithdraw);
    connect(transferBtn, &QPushButton::clicked, this, &MainWindow::onTransfer);

    QHBoxLayout* btnRow = new QHBoxLayout();
    btnRow->addWidget(depositBtn);
    btnRow->addWidget(withdrawBtn);
    btnRow->addWidget(transferBtn);

    opsLayout->addWidget(accountNumEdit);
    opsLayout->addWidget(toAccountNumEdit);
    opsLayout->addWidget(amountEdit);
    opsLayout->addLayout(btnRow);
    opsLayout->addStretch();

    topRow->addWidget(createBox);
    topRow->addWidget(opsBox);
    root->addLayout(topRow);

    // bottom splitter
    QSplitter* splitter = new QSplitter(Qt::Vertical);

    // accounts table
    QGroupBox* tableBox = new QGroupBox("Accounts");
    QVBoxLayout* tableLayout = new QVBoxLayout(tableBox);

    accountTable = new QTableWidget();
    accountTable->setColumnCount(5);
    accountTable->setHorizontalHeaderLabels({"Account #", "Type", "Owner(s)", "Balance", "Status"});
    accountTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    accountTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    accountTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    accountTable->setAlternatingRowColors(true);
    accountTable->setStyleSheet("QTableWidget { alternate-background-color: #1e1e3a; }");

    QPushButton* refreshBtn = new QPushButton("Refresh");
    connect(refreshBtn, &QPushButton::clicked, this, &MainWindow::onRefreshTable);
    QHBoxLayout* tableHeaderRow = new QHBoxLayout();
    tableHeaderRow->addStretch();
    tableHeaderRow->addWidget(refreshBtn);

    tableLayout->addLayout(tableHeaderRow);
    tableLayout->addWidget(accountTable);

    // Log
    QGroupBox* logBox = new QGroupBox("Activity Log");
    QVBoxLayout* logLayout = new QVBoxLayout(logBox);
    logDisplay = new QTextEdit();
    logDisplay->setReadOnly(true);
    logDisplay->setMaximumHeight(140);
    logLayout->addWidget(logDisplay);

    splitter->addWidget(tableBox);
    splitter->addWidget(logBox);
    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 1);

    root->addWidget(splitter, 1);
}

// helpers

void MainWindow::log(const QString& message) {
    logDisplay->append(message);
}

void MainWindow::rebuildAccountTable() {
    const auto& accs = bank->getAccounts();
    accountTable->setRowCount(static_cast<int>(accs.size()));

    for (int row = 0; row < static_cast<int>(accs.size()); ++row) {
        const auto& acc = accs[row];

        QString owners;
        for (const auto& c : acc->getOwner()) {
            if (!owners.isEmpty()) owners += ", ";
            owners += QString::fromStdString(c->getFullName());
        }

        accountTable->setItem(row, 0, new QTableWidgetItem(
            QString::fromStdString(acc->getAccountNumber())));
        accountTable->setItem(row, 1, new QTableWidgetItem(
            QString::fromStdString(acc->getAccountTypeString())));
        accountTable->setItem(row, 2, new QTableWidgetItem(owners));
        accountTable->setItem(row, 3, new QTableWidgetItem(
            "$" + QString::number(acc->getBalance(), 'f', 2)));

        // color balance
        auto* balanceItem = accountTable->item(row, 3);
        if (acc->getBalance() < 0)
            balanceItem->setForeground(QColor("#f87171"));
        else
            balanceItem->setForeground(QColor("#4ade80"));

        accountTable->setItem(row, 4, new QTableWidgetItem("Active"));
    }
}

// slots

void MainWindow::onCreateAccount() {
    QString first = firstNameEdit->text().trimmed();
    QString last  = lastNameEdit->text().trimmed();
    QString phone = phoneEdit->text().trimmed();
    QString email = emailEdit->text().trimmed();

    if (first.isEmpty() || last.isEmpty() || phone.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Missing Fields", "Please fill in all customer fields.");
        return;
    }

    // find or create customer
    auto customer = bank->findCustomer((first + " " + last).toStdString());
    if (!customer) {
        customer = bank->createCustomer(first.toStdString(), last.toStdString(),
                                        phone.toStdString(), email.toStdString());
        log("New customer registered: " + first + " " + last);
    } else {
        log("Existing customer found: " + first + " " + last +
            " (opening additional account)");
    }

    std::shared_ptr<Account> acc;
    int typeIdx = accountTypeCombo->currentIndex();
    if (typeIdx == 1)
        acc = bank->createSavingsAccount(customer);
    else if (typeIdx == 2)
        acc = bank->createCheckingAccount(customer);
    else
        acc = bank->createAccount(customer);

    log("Account created: " + QString::fromStdString(acc->getAccountNumber()) +
        " [" + QString::fromStdString(acc->getAccountTypeString()) + "]");

    firstNameEdit->clear(); lastNameEdit->clear();
    phoneEdit->clear();     emailEdit->clear();

    rebuildAccountTable();
}

void MainWindow::onDeposit() {
    QString accNum    = accountNumEdit->text().trimmed();
    QString amountStr = amountEdit->text().trimmed();
    if (accNum.isEmpty() || amountStr.isEmpty()) {
        QMessageBox::warning(this, "Missing Fields", "Enter account number and amount.");
        return;
    }

    bool ok;
    double amount = amountStr.toDouble(&ok);
    if (!ok || amount <= 0) {
        QMessageBox::warning(this, "Invalid Amount", "Please enter a valid positive number.");
        return;
    }

    if (bank->depositToAccount(accNum.toStdString(), amount)) {
        log("Deposited $" + QString::number(amount, 'f', 2) + " → " + accNum);
    } else {
        log("Deposit FAILED for " + accNum);
    }
    rebuildAccountTable();
}

void MainWindow::onWithdraw() {
    QString accNum    = accountNumEdit->text().trimmed();
    QString amountStr = amountEdit->text().trimmed();
    if (accNum.isEmpty() || amountStr.isEmpty()) {
        QMessageBox::warning(this, "Missing Fields", "Enter account number and amount.");
        return;
    }

    bool ok;
    double amount = amountStr.toDouble(&ok);
    if (!ok || amount <= 0) {
        QMessageBox::warning(this, "Invalid Amount", "Please enter a valid positive number.");
        return;
    }

    if (bank->withdrawFromAccount(accNum.toStdString(), amount)) {
        log("Withdrew $" + QString::number(amount, 'f', 2) + " ← " + accNum);
    } else {
        log("Withdrawal FAILED for " + accNum + " (insufficient funds or limit reached)");
    }
    rebuildAccountTable();
}

void MainWindow::onTransfer() {
    QString fromAcc   = accountNumEdit->text().trimmed();
    QString toAcc     = toAccountNumEdit->text().trimmed();
    QString amountStr = amountEdit->text().trimmed();

    if (fromAcc.isEmpty() || toAcc.isEmpty() || amountStr.isEmpty()) {
        QMessageBox::warning(this, "Missing Fields", "Fill in both account numbers and amount.");
        return;
    }

    bool ok;
    double amount = amountStr.toDouble(&ok);
    if (!ok || amount <= 0) {
        QMessageBox::warning(this, "Invalid Amount", "Please enter a valid positive number.");
        return;
    }

    if (bank->transferFunds(fromAcc.toStdString(), toAcc.toStdString(), amount)) {
        log("Transferred $" + QString::number(amount, 'f', 2) +
            " from " + fromAcc + " to " + toAcc);
    } else {
        log("Transfer FAILED: " + fromAcc + " → " + toAcc);
    }
    rebuildAccountTable();
}

void MainWindow::onRefreshTable() {
    rebuildAccountTable();
    log("Table refreshed.");
}
