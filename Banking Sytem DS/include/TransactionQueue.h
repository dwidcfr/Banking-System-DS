#ifndef TRANSACTION_QUEUE_H
#define TRANSACTION_QUEUE_H

#include <string>

class AccountBST;
struct AccountNode;

struct QueueNode {
    int accountNumber;
    std::string type;
    double amount;
    QueueNode* next;

    QueueNode(int acc, const std::string& t, double amt);
};

class TransactionQueue {
public:
    TransactionQueue();
    ~TransactionQueue();

    void enqueue(int accNum, const std::string& type, double amount);
    bool isEmpty() const;
    int processQueue(AccountBST& bst);
    int getSize() const;
    void display() const;

private:
    QueueNode* front;
    QueueNode* rear;
    int size;
};

#endif // TRANSACTION_QUEUE_H
