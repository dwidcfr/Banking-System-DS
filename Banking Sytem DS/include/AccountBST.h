#ifndef ACCOUNT_BST_H
#define ACCOUNT_BST_H

#include <functional>
#include <string>

#include "TransactionHistory.h"

struct AccountNode {
    int accountNumber;
    std::string holderName;
    std::string password;
    double balance;
    TransactionHistory* history;
    AccountNode* left;
    AccountNode* right;

    AccountNode(int accNum, const std::string& name, const std::string& pass, double bal);
    ~AccountNode();
};

class AccountBST {
public:
    AccountBST();
    ~AccountBST();

    void insert(int accNum, const std::string& name, const std::string& password, double bal);
    AccountNode* search(int accNum) const;
    void displayAll() const;
    void applyInterest(double rate);
    void forEach(const std::function<void(const AccountNode*)>& visitor) const;

private:
    AccountNode* root;

    AccountNode* insertHelper(AccountNode* node, int accNum, const std::string& name, const std::string& password, double bal);
    AccountNode* searchHelper(AccountNode* node, int accNum) const;
    void inorderHelper(AccountNode* node) const;
    void applyInterestHelper(AccountNode* node, double rate);
    void destroyTree(AccountNode* node);
    void forEachHelper(AccountNode* node, const std::function<void(const AccountNode*)>& visitor) const;
};

#endif // ACCOUNT_BST_H
