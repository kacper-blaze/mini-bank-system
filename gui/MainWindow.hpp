#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QComboBox>
#include <memory>
#include "BankSystem.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(std::shared_ptr<BankSystem> bank, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCreateAccount();
    void onDeposit();
    void onWithdraw();
    void onTransfer();
    void onDisplayAccounts();

private:
    void setupUI();
    void refreshAccounts();

    std::shared_ptr<BankSystem> bank;

    QLineEdit *firstNameEdit;
    QLineEdit *lastNameEdit;
    QLineEdit *phoneEdit;
    QLineEdit *emailEdit;
    QLineEdit *accountNumEdit;
    QLineEdit *toAccountNumEdit;
    QLineEdit *amountEdit;
    QTableWidget *accountsTable;
    QComboBox *accountSelector;
};
