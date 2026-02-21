# Digital Banking System

A robust, console-based banking application written in C++. This project demonstrates the practical application of core data structures to manage user accounts, process transactions, and maintain account histories.

## 🚀 Features

* **Account Management:** Register new accounts with secure passwords and initial balances.
* **Authentication System:** Login and logout functionality to protect user data.
* **Dual-Mode Transactions:** * *Immediate:* Deposits and withdrawals are applied to the account instantly.
    * *Queued:* Transactions are added to a pending queue and processed at the end of the session.
* **Transaction History:** Keeps a detailed, timestamped log of all account activities.
* **Data Persistence:** Automatically saves and loads account data, balances, and transaction histories to a local file (`data/accounts.txt`).

## 🧠 Data Structures Utilized

This system integrates several fundamental data structures to optimize performance and organization:

* **Binary Search Tree (BST) - `AccountBST`:** Used to store and manage bank accounts. The BST allows for efficient searching, insertion, and retrieval of accounts using the Account Number as the key.
* **Linked List - `TransactionHistory`:** Each account maintains its own singly linked list to dynamically store an arbitrary number of past transactions without wasting contiguous memory.
* **Queue - `TransactionQueue`:** Utilizes a First-In-First-Out (FIFO) queue to manage delayed transactions, simulating batch processing systems used in real-world banks.

## 📂 Project Structure

* `main.cpp`: The entry point of the application.
* `BankingSystem.h` / `.cpp`: Manages the main menu, user sessions, file I/O, and bridges the data structures.
* `AccountBST.h` / `.cpp`: Implements the Binary Search Tree for account management.
* `TransactionHistory.h` / `.cpp`: Implements the Linked List for individual transaction logs.
* `TransactionQueue.h` / `.cpp`: Implements the Queue for pending deposits and withdrawals.
