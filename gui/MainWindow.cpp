#include "MainWindow.hpp"
#include <QMessageBox>

MainWindow::MainWindow(std::shared_ptr<BankSystem> bank, QWidget *parent)
    : QMainWindow(parent), bank(bank) {
    setupUI();
    refreshAccounts();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Create Account Section
    QLabel *createLabel = new QLabel("Create Account:");
    firstNameEdit = new QLineEdit();
    firstNameEdit->setPlaceholderText("First Name");
    lastNameEdit = new QLineEdit();
    lastNameEdit->setPlaceholderText("Last Name");
    phoneEdit = new QLineEdit();
    phoneEdit->setPlaceholderText("Phone");
    emailEdit = new QLineEdit();
    emailEdit->setPlaceholderText("Email");

    QPushButton *createBtn = new QPushButton("Create Account");
    connect(createBtn, &QPushButton::clicked, this, &MainWindow::onCreateAccount);

    QHBoxLayout *createLayout = new QHBoxLayout();
    createLayout->addWidget(firstNameEdit);
    createLayout->addWidget(lastNameEdit);
    createLayout->addWidget(phoneEdit);
    createLayout->addWidget(emailEdit);
    createLayout->addWidget(createBtn);

    mainLayout->addWidget(createLabel);
    mainLayout->addLayout(createLayout);

    // Deposit Section
    QLabel *depositLabel = new QLabel("Deposit:");
    accountNumEdit = new QLineEdit();
    accountNumEdit->setPlaceholderText("Account Number");
    amountEdit = new QLineEdit();
    amountEdit->setPlaceholderText("Amount");

    QPushButton *depositBtn = new QPushButton("Deposit");
    connect(depositBtn, &QPushButton::clicked, this, &MainWindow::onDeposit);

    QHBoxLayout *depositLayout = new QHBoxLayout();
    depositLayout->addWidget(accountNumEdit);
    depositLayout->addWidget(amountEdit);
    depositLayout->addWidget(depositBtn);

    mainLayout->addWidget(depositLabel);
    mainLayout->addLayout(depositLayout);

    // Withdraw Section
    QLabel *withdrawLabel = new QLabel("Withdraw:");
    QPushButton *withdrawBtn = new QPushButton("Withdraw");
    connect(withdrawBtn, &QPushButton::clicked, this, &MainWindow::onWithdraw);

    QHBoxLayout *withdrawLayout = new QHBoxLayout();
    withdrawLayout->addWidget(accountNumEdit);
    withdrawLayout->addWidget(amountEdit);
    withdrawLayout->addWidget(withdrawBtn);

    mainLayout->addWidget(withdrawLabel);
    mainLayout->addLayout(withdrawLayout);

    // Transfer Section
    QLabel *transferLabel = new QLabel("Transfer:");
    toAccountNumEdit = new QLineEdit();
    toAccountNumEdit->setPlaceholderText("To Account Number");

    QPushButton *transferBtn = new QPushButton("Transfer");
    connect(transferBtn, &QPushButton::clicked, this, &MainWindow::onTransfer);

    QHBoxLayout *transferLayout = new QHBoxLayout();
    transferLayout->addWidget(accountNumEdit);
    transferLayout->addWidget(toAccountNumEdit);
    transferLayout->addWidget(amountEdit);
    transferLayout->addWidget(transferBtn);

    mainLayout->addWidget(transferLabel);
    mainLayout->addLayout(transferLayout);

    // Display Accounts Section
    QPushButton *displayBtn = new QPushButton("Display All Accounts");
    connect(displayBtn, &QPushButton::clicked, this, &MainWindow::onDisplayAccounts);
    mainLayout->addWidget(displayBtn);

    // Output Display
    outputDisplay = new QTextEdit();
    outputDisplay->setReadOnly(true);
    mainLayout->addWidget(outputDisplay);

    setWindowTitle("Mini Bank System");
    resize(600, 500);
}

void MainWindow::refreshAccounts() {
    accountSelector = new QComboBox();
    for (const auto& acc : bank->getAccounts()) {
        accountSelector->addItem(QString::fromStdString(acc->getAccountNumber()));
    }
}

void MainWindow::onCreateAccount() {
    QString first = firstNameEdit->text();
    QString last = lastNameEdit->text();
    QString phone = phoneEdit->text();
    QString email = emailEdit->text();

    if (first.isEmpty() || last.isEmpty() || phone.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill all fields");
        return;
    }

    auto customer = bank->createCustomer(first.toStdString(), last.toStdString(), 
                                         phone.toStdString(), email.toStdString());
    auto acc = bank->createAccount(customer);

    outputDisplay->append("Account Created: " + QString::fromStdString(acc->getAccountNumber()));
    
    firstNameEdit->clear();
    lastNameEdit->clear();
    phoneEdit->clear();
    emailEdit->clear();
    refreshAccounts();
}

void MainWindow::onDeposit() {
    QString accNum = accountNumEdit->text();
    QString amountStr = amountEdit->text();

    if (accNum.isEmpty() || amountStr.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill account number and amount");
        return;
    }

    double amount = amountStr.toDouble();
    if (bank->depositToAccount(accNum.toStdString(), amount)) {
        outputDisplay->append("Deposit Successful to " + accNum);
    } else {
        outputDisplay->append("Deposit Failed");
    }
}

void MainWindow::onWithdraw() {
    QString accNum = accountNumEdit->text();
    QString amountStr = amountEdit->text();

    if (accNum.isEmpty() || amountStr.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill account number and amount");
        return;
    }

    double amount = amountStr.toDouble();
    if (bank->withdrawFromAccount(accNum.toStdString(), amount)) {
        outputDisplay->append("Withdrawal Successful from " + accNum);
    } else {
        outputDisplay->append("Withdrawal Failed");
    }
}

void MainWindow::onTransfer() {
    QString fromAcc = accountNumEdit->text();
    QString toAcc = toAccountNumEdit->text();
    QString amountStr = amountEdit->text();

    if (fromAcc.isEmpty() || toAcc.isEmpty() || amountStr.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill all fields");
        return;
    }

    double amount = amountStr.toDouble();
    if (bank->transferFunds(fromAcc.toStdString(), toAcc.toStdString(), amount)) {
        outputDisplay->append("Transfer Successful from " + fromAcc + " to " + toAcc);
    } else {
        outputDisplay->append("Transfer Failed");
    }
}

void MainWindow::onDisplayAccounts() {
    outputDisplay->clear();
    outputDisplay->append("=== All Accounts ===");
    for (const auto& acc : bank->getAccounts()) {
        outputDisplay->append(QString::fromStdString(acc->getAccountNumber()) + 
                             ": $" + QString::number(acc->getBalance()));
    }
}
