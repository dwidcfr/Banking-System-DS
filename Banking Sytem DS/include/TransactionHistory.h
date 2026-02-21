#ifndef TRANSACTION_HISTORY_H
#define TRANSACTION_HISTORY_H

#include <functional>
#include <string>

struct TransactionNode {
    std::string type;
    double amount;
    std::string dateTime;
    TransactionNode* next;

    TransactionNode(const std::string& t, double amt, const std::string& dt);
};

class TransactionHistory {
public:
    TransactionHistory();
    ~TransactionHistory();

    void addTransaction(const std::string& type, double amount);
    void addTransactionRecord(const std::string& type, double amount, const std::string& timestamp);
    void display() const;
    int getCount() const;
    void forEach(const std::function<void(const TransactionNode*)>& visitor) const;

private:
    TransactionNode* head;
    int count;

    void appendNode(TransactionNode* newNode);
};

#endif // TRANSACTION_HISTORY_H
