#include "TransactionHistory.h"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

TransactionNode::TransactionNode(const std::string& t, double amt, const std::string& dt)
    : type(t), amount(amt), dateTime(dt), next(nullptr) {}

TransactionHistory::TransactionHistory() : head(nullptr), count(0) {}

TransactionHistory::~TransactionHistory() {
    TransactionNode* current = head;
    while (current != nullptr) {
        TransactionNode* nextNode = current->next;
        delete current;
        current = nextNode;
    }
}

void TransactionHistory::appendNode(TransactionNode* newNode) {
    if (head == nullptr) {
        head = newNode;
    } else {
        TransactionNode* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
    count++;
}

void TransactionHistory::addTransaction(const std::string& type, double amount) {
    std::time_t now = std::time(nullptr);
    std::tm* ltm = std::localtime(&now);

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << ltm->tm_mday << "-"
       << std::setfill('0') << std::setw(2) << (1 + ltm->tm_mon) << "-"
       << (1900 + ltm->tm_year) << " "
       << std::setfill('0') << std::setw(2) << ltm->tm_hour << ":"
       << std::setfill('0') << std::setw(2) << ltm->tm_min << ":"
       << std::setfill('0') << std::setw(2) << ltm->tm_sec;

    appendNode(new TransactionNode(type, amount, ss.str()));
}

void TransactionHistory::addTransactionRecord(const std::string& type, double amount, const std::string& timestamp) {
    appendNode(new TransactionNode(type, amount, timestamp));
}

void TransactionHistory::display() const {
    if (head == nullptr) {
        std::cout << "        No transactions yet.\n";
        return;
    }

    const TransactionNode* temp = head;
    int num = 1;
    while (temp != nullptr) {
        std::cout << "        " << num++ << ". " << temp->type
                  << " $" << std::fixed << std::setprecision(2) << temp->amount
                  << " on " << temp->dateTime << std::endl;
        temp = temp->next;
    }
}

int TransactionHistory::getCount() const {
    return count;
}

void TransactionHistory::forEach(const std::function<void(const TransactionNode*)>& visitor) const {
    const TransactionNode* temp = head;
    while (temp != nullptr) {
        visitor(temp);
        temp = temp->next;
    }
}
