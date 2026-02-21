
#include "TransactionQueue.h"

#include <iostream>
#include <sstream>
#include <iomanip>

#include "AccountBST.h"

QueueNode::QueueNode(int acc, const std::string& t, double amt)
    : accountNumber(acc), type(t), amount(amt), next(nullptr) {}

TransactionQueue::TransactionQueue() : front(nullptr), rear(nullptr), size(0) {}

TransactionQueue::~TransactionQueue() {
    while (!isEmpty()) {
        QueueNode* temp = front;
        front = front->next;
        delete temp;
    }
}

void TransactionQueue::enqueue(int accNum, const std::string& type, double amount) {
    QueueNode* newNode = new QueueNode(accNum, type, amount);

    if (rear == nullptr) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
    size++;
    std::cout << "Transaction added to queue. Queue size: " << size << std::endl;
}

bool TransactionQueue::isEmpty() const {
    return front == nullptr;
}

int TransactionQueue::processQueue(AccountBST& bst) {
    if (isEmpty()) {
        std::cout << "No pending transactions in queue.\n";
        return 0;
    }

    int processed = 0;
    while (!isEmpty()) {
        QueueNode* temp = front;

        AccountNode* account = bst.search(temp->accountNumber);
        if (account == nullptr) {
            std::cout << "Error: Account #" << temp->accountNumber << " not found. Skipping.\n";
        } else {
            if (temp->type == "Deposit") {
                account->balance += temp->amount;
                account->history->addTransaction("Deposit", temp->amount);
                std::cout << "Processed: Deposited $" << temp->amount
                          << " to Account #" << temp->accountNumber << std::endl;
            } else if (temp->type == "Withdrawal") {
                if (account->balance >= temp->amount) {
                    account->balance -= temp->amount;
                    account->history->addTransaction("Withdrawal", temp->amount);
                    std::cout << "Processed: Withdrew $" << temp->amount
                              << " from Account #" << temp->accountNumber << std::endl;
                } else {
                    std::cout << "Insufficient balance for Account #"
                              << temp->accountNumber << ". Skipping.\n";
                }
            }
            processed++;
        }

        front = front->next;
        if (front == nullptr) {
            rear = nullptr;
        }
        delete temp;
        size--;
    }

    std::cout << "\nTotal transactions processed: " << processed << std::endl;
    return processed;
}

int TransactionQueue::getSize() const {
    return size;
}

void TransactionQueue::display() const {
    if (isEmpty()) {
        std::cout << "\nQueue is empty.\n";
        return;
    }

    std::cout << "\n--- Pending Transactions Queue ---\n";
    const QueueNode* temp = front;
    int position = 1;
    while (temp != nullptr) {
        std::ostringstream amount;
        amount << std::fixed << std::setprecision(2) << temp->amount;
        std::cout << position++ << ". Account #" << temp->accountNumber << " | " << temp->type
                  << " $" << amount.str() << "\n";
        temp = temp->next;
    }
}
