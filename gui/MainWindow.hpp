#pragma once
#include "BankSystem.hpp"
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <memory>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(std::shared_ptr<BankSystem> bank, QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void onCreateAccount();
    void onDeposit();
    void onWithdraw();
    void onTransfer();
    void onRefreshTable();

private:
    void setupUI();
    void applyStyle();
    void rebuildAccountTable();
    void log(const QString& message);

    std::shared_ptr<BankSystem> bank;

    // new account
    QLineEdit*  firstNameEdit;
    QLineEdit*  lastNameEdit;
    QLineEdit*  phoneEdit;
    QLineEdit*  emailEdit;
    QComboBox*  accountTypeCombo;

    // operations
    QLineEdit*  accountNumEdit;
    QLineEdit*  toAccountNumEdit;
    QLineEdit*  amountEdit;

    // views
    QTableWidget* accountTable;
    QTextEdit*    logDisplay;
};
