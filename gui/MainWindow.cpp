#include "MainWindow.hpp"
#include <QMessageBox>
#include <QIcon>
#include <QTableWidgetItem>
#include <QHeaderView>

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
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(12);

    // Create Account Section
    QGroupBox *createGroup = new QGroupBox("Create Account");
    QVBoxLayout *createGroupLayout = new QVBoxLayout(createGroup);
    createGroupLayout->setSpacing(8);

    QHBoxLayout *nameLayout = new QHBoxLayout();
    firstNameEdit = new QLineEdit();
    firstNameEdit->setPlaceholderText("First Name");
    lastNameEdit = new QLineEdit();
    lastNameEdit->setPlaceholderText("Last Name");
    nameLayout->addWidget(firstNameEdit);
    nameLayout->addWidget(lastNameEdit);

    QHBoxLayout *contactLayout = new QHBoxLayout();
    phoneEdit = new QLineEdit();
    phoneEdit->setPlaceholderText("Phone");
    emailEdit = new QLineEdit();
    emailEdit->setPlaceholderText("Email");
    contactLayout->addWidget(phoneEdit);
    contactLayout->addWidget(emailEdit);

    QPushButton *createBtn = new QPushButton("Create Account");
    createBtn->setIcon(QIcon::fromTheme("list-add"));
    connect(createBtn, &QPushButton::clicked, this, &MainWindow::onCreateAccount);

    createGroupLayout->addLayout(nameLayout);
    createGroupLayout->addLayout(contactLayout);
    createGroupLayout->addWidget(createBtn);

    mainLayout->addWidget(createGroup);

    // Deposit Section
    QGroupBox *depositGroup = new QGroupBox("Deposit");
    QHBoxLayout *depositLayout = new QHBoxLayout(depositGroup);
    depositLayout->setSpacing(8);

    accountNumEdit = new QLineEdit();
    accountNumEdit->setPlaceholderText("Account Number");
    amountEdit = new QLineEdit();
    amountEdit->setPlaceholderText("Amount");

    QPushButton *depositBtn = new QPushButton("Deposit");
    depositBtn->setIcon(QIcon::fromTheme("go-down"));
    connect(depositBtn, &QPushButton::clicked, this, &MainWindow::onDeposit);

    depositLayout->addWidget(accountNumEdit);
    depositLayout->addWidget(amountEdit);
    depositLayout->addWidget(depositBtn);

    mainLayout->addWidget(depositGroup);

    // Withdraw Section
    QGroupBox *withdrawGroup = new QGroupBox("Withdraw");
    QHBoxLayout *withdrawLayout = new QHBoxLayout(withdrawGroup);
    withdrawLayout->setSpacing(8);

    QPushButton *withdrawBtn = new QPushButton("Withdraw");
    withdrawBtn->setIcon(QIcon::fromTheme("go-up"));
    connect(withdrawBtn, &QPushButton::clicked, this, &MainWindow::onWithdraw);

    withdrawLayout->addWidget(accountNumEdit);
    withdrawLayout->addWidget(amountEdit);
    withdrawLayout->addWidget(withdrawBtn);

    mainLayout->addWidget(withdrawGroup);

    // Transfer Section
    QGroupBox *transferGroup = new QGroupBox("Transfer");
    QHBoxLayout *transferLayout = new QHBoxLayout(transferGroup);
    transferLayout->setSpacing(8);

    toAccountNumEdit = new QLineEdit();
    toAccountNumEdit->setPlaceholderText("To Account Number");

    QPushButton *transferBtn = new QPushButton("Transfer");
    transferBtn->setIcon(QIcon::fromTheme("go-next"));
    connect(transferBtn, &QPushButton::clicked, this, &MainWindow::onTransfer);

    transferLayout->addWidget(accountNumEdit);
    transferLayout->addWidget(toAccountNumEdit);
    transferLayout->addWidget(amountEdit);
    transferLayout->addWidget(transferBtn);

    mainLayout->addWidget(transferGroup);

    // Accounts Table
    QGroupBox *accountsGroup = new QGroupBox("Accounts");
    QVBoxLayout *accountsGroupLayout = new QVBoxLayout(accountsGroup);

    QPushButton *refreshBtn = new QPushButton("Refresh");
    refreshBtn->setIcon(QIcon::fromTheme("view-refresh"));
    connect(refreshBtn, &QPushButton::clicked, this, &MainWindow::onDisplayAccounts);

    accountsTable = new QTableWidget();
    accountsTable->setColumnCount(3);
    accountsTable->setHorizontalHeaderLabels({"Account Number", "Owner", "Balance"});
    accountsTable->horizontalHeader()->setStretchLastSection(true);
    accountsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    accountsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    accountsTable->setAlternatingRowColors(true);

    accountsGroupLayout->addWidget(refreshBtn);
    accountsGroupLayout->addWidget(accountsTable);

    mainLayout->addWidget(accountsGroup);

    setWindowTitle("Mini Bank System");
    resize(800, 600);
}

void MainWindow::refreshAccounts() {
    accountSelector = new QComboBox();
    for (const auto& acc : bank->getAccounts()) {
        accountSelector->addItem(QString::fromStdString(acc->getAccountNumber()));
    }
    onDisplayAccounts();
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

    QMessageBox::information(this, "Success", "Account Created: " + QString::fromStdString(acc->getAccountNumber()));

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
        QMessageBox::information(this, "Success", "Deposit Successful");
        onDisplayAccounts();
    } else {
        QMessageBox::warning(this, "Error", "Deposit Failed");
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
        QMessageBox::information(this, "Success", "Withdrawal Successful");
        onDisplayAccounts();
    } else {
        QMessageBox::warning(this, "Error", "Withdrawal Failed");
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
        QMessageBox::information(this, "Success", "Transfer Successful");
        onDisplayAccounts();
    } else {
        QMessageBox::warning(this, "Error", "Transfer Failed");
    }
}

void MainWindow::onDisplayAccounts() {
    accountsTable->setRowCount(0);
    int row = 0;
    for (const auto& acc : bank->getAccounts()) {
        accountsTable->insertRow(row);
        accountsTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(acc->getAccountNumber())));
        auto owners = acc->getOwner();
        QString ownerName = owners.empty() ? "Unknown" : QString::fromStdString(owners[0]->getFullName());
        accountsTable->setItem(row, 1, new QTableWidgetItem(ownerName));
        accountsTable->setItem(row, 2, new QTableWidgetItem(QString::number(acc->getBalance(), 'f', 2)));
        row++;
    }
}
