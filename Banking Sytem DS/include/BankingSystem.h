#ifndef BANKING_SYSTEM_H
#define BANKING_SYSTEM_H

#include <string>

#include "AccountBST.h"
#include "TransactionQueue.h"

class BankingSystem {
public:
    BankingSystem();
    void run();

private:
    AccountBST accounts;
    AccountNode* currentUser;
    const std::string dataFilePath;
    TransactionQueue queue;

    void showAuthMenu() const;
    void showUserMenu() const;
    void handleAuthChoice(int choice, bool& running);
    void handleUserChoice(int choice, bool& running);
    void registerAccount();
    void login();
    void logout();
    void viewAccountSummary() const;
    void deposit();
    void withdraw();
    void displayHistory() const;
    void displayPendingQueue() const;
    void enqueueTransaction(const std::string& type, double amount);
    void loadAccountsFromFile();
    void saveAccountsToFile() const;
    void ensureDataDirectory() const;
};

#endif // BANKING_SYSTEM_H
