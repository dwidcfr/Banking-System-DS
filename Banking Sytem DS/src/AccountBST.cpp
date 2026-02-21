#include "AccountBST.h"

#include <iomanip>
#include <iostream>
#include <sstream>

AccountNode::AccountNode(int accNum, const std::string& name, const std::string& pass, double bal)
    : accountNumber(accNum),
      holderName(name),
      password(pass),
      balance(bal),
      history(new TransactionHistory()),
      left(nullptr),
      right(nullptr) {}

AccountNode::~AccountNode() {
    delete history;
}

AccountBST::AccountBST() : root(nullptr) {}

AccountBST::~AccountBST() {
    destroyTree(root);
}

void AccountBST::insert(int accNum, const std::string& name, const std::string& password, double bal) {
    root = insertHelper(root, accNum, name, password, bal);
}

AccountNode* AccountBST::search(int accNum) const {
    return searchHelper(root, accNum);
}

void AccountBST::displayAll() const {
    if (root == nullptr) {
        std::cout << "    No accounts in the system.\n";
        return;
    }
    inorderHelper(root);
}

void AccountBST::applyInterest(double rate) {
    applyInterestHelper(root, rate);
}

AccountNode* AccountBST::insertHelper(AccountNode* node, int accNum, const std::string& name, const std::string& password, double bal) {
    if (node == nullptr) {
        return new AccountNode(accNum, name, password, bal);
    }

    if (accNum < node->accountNumber) {
        node->left = insertHelper(node->left, accNum, name, password, bal);
    } else if (accNum > node->accountNumber) {
        node->right = insertHelper(node->right, accNum, name, password, bal);
    } else {
        std::cout << "Account number already exists!\n";
    }

    return node;
}

AccountNode* AccountBST::searchHelper(AccountNode* node, int accNum) const {
    if (node == nullptr || node->accountNumber == accNum) {
        return node;
    }

    if (accNum < node->accountNumber) {
        return searchHelper(node->left, accNum);
    }
    return searchHelper(node->right, accNum);
}

void AccountBST::inorderHelper(AccountNode* node) const {
    if (node != nullptr) {
        inorderHelper(node->left);
        std::cout << "    Account #" << node->accountNumber
                  << " | " << node->holderName
                  << " | Balance: $" << std::fixed << std::setprecision(2) << node->balance << std::endl;
        inorderHelper(node->right);
    }
}

void AccountBST::applyInterestHelper(AccountNode* node, double rate) {
    if (node != nullptr) {
        applyInterestHelper(node->left, rate);

        double interest = node->balance * (rate / 100.0);
        node->balance += interest;

        std::stringstream ss;
        ss << "Interest (" << rate << "%)";
        node->history->addTransaction(ss.str(), interest);

        applyInterestHelper(node->right, rate);
    }
}

void AccountBST::destroyTree(AccountNode* node) {
    if (node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

void AccountBST::forEach(const std::function<void(const AccountNode*)>& visitor) const {
    forEachHelper(root, visitor);
}

void AccountBST::forEachHelper(AccountNode* node, const std::function<void(const AccountNode*)>& visitor) const {
    if (node == nullptr) {
        return;
    }

    forEachHelper(node->left, visitor);
    visitor(node);
    forEachHelper(node->right, visitor);
}
