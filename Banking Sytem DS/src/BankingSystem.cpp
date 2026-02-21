#include "BankingSystem.h"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

namespace {
void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
}

BankingSystem::BankingSystem()
    : currentUser(nullptr), dataFilePath("data/accounts.txt") {}

void BankingSystem::run() {
    ensureDataDirectory();
    loadAccountsFromFile();

    bool running = true;

    std::cout << "\n*** Welcome to the Banking System ***\n";
    std::cout << "BST + Linked List + Queue Integration\n";

    while (running) {
        int choice = 0;

        if (currentUser == nullptr) {
            showAuthMenu();
        } else {
            showUserMenu();
        }

        if (!(std::cin >> choice)) {
            std::cout << "Invalid input! Please enter a number.\n";
            clearInput();
            continue;
        }
        clearInput();

        if (currentUser == nullptr) {
            handleAuthChoice(choice, running);
        } else {
            handleUserChoice(choice, running);
        }
    }

    std::cout << "\nProcessing queued transactions after exit...\n";
    queue.processQueue(accounts);
    saveAccountsToFile();
    std::cout << "\nAll changes saved. Goodbye!\n";
}

void BankingSystem::showAuthMenu() const {
    std::cout << "\n========================================\n";
    std::cout << "          DIGITAL BANK PORTAL\n";
    std::cout << "========================================\n";
    std::cout << "1.  Login to Existing Account\n";
    std::cout << "2.  Register New Account\n";
    std::cout << "3.  Exit\n";
    std::cout << "========================================\n";
    std::cout << "Enter your choice: ";
}

void BankingSystem::showUserMenu() const {
    std::cout << "\n========================================\n";
    std::cout << "   Welcome, " << currentUser->holderName << "\n";
    std::cout << "========================================\n";
    std::cout << "1.  View Account Summary\n";
    std::cout << "2.  Deposit Money\n";
    std::cout << "3.  Withdraw Money\n";
    std::cout << "4.  View Transaction History\n";
    std::cout << "5.  View Pending Queue (" << queue.getSize() << ")\n";
    std::cout << "6.  Exit Application (queue auto-processed)\n";
    std::cout << "7.  Logout\n";
    std::cout << "========================================\n";
    std::cout << "Enter your choice: ";
}

void BankingSystem::handleAuthChoice(int choice, bool& running) {
    switch (choice) {
        case 1:
            login();
            break;
        case 2:
            registerAccount();
            break;
        case 3:
            running = false;
            break;
        default:
            std::cout << "Invalid choice! Please try again.\n";
    }
}

void BankingSystem::handleUserChoice(int choice, bool& running) {
    switch (choice) {
        case 1:
            viewAccountSummary();
            break;
        case 2:
            deposit();
            break;
        case 3:
            withdraw();
            break;
        case 4:
            displayHistory();
            break;
        case 5:
            displayPendingQueue();
            break;
        case 6:
            running = false;
            break;
        case 7:
            logout();
            break;
        default:
            std::cout << "Invalid choice! Please try again.\n";
    }
}

void BankingSystem::registerAccount() {
    int accNum = 0;
    std::string name;
    std::string password;
    std::string confirmPassword;
    double initialBalance = 0.0;

    std::cout << "\n--- Register New Account ---\n";
    std::cout << "Enter Account Number (numeric): ";
    if (!(std::cin >> accNum)) {
        std::cout << "Invalid account number!\n";
        clearInput();
        return;
    }
    clearInput();

    if (accounts.search(accNum) != nullptr) {
        std::cout << "Account number already exists!\n";
        return;
    }

    std::cout << "Enter Account Holder Name: ";
    std::getline(std::cin, name);

    std::cout << "Create Password: ";
    std::getline(std::cin, password);

    std::cout << "Confirm Password: ";
    std::getline(std::cin, confirmPassword);

    if (password != confirmPassword) {
        std::cout << "Passwords do not match!\n";
        return;
    }

    std::cout << "Enter Initial Balance: $";
    if (!(std::cin >> initialBalance)) {
        std::cout << "Invalid amount!\n";
        clearInput();
        return;
    }
    clearInput();

    if (initialBalance < 0) {
        std::cout << "Balance must be non-negative.\n";
        return;
    }

    accounts.insert(accNum, name, password, initialBalance);
    saveAccountsToFile();
    std::cout << "Account created successfully! You can now log in.\n";
}

void BankingSystem::login() {
    int accNum = 0;
    std::string password;

    std::cout << "\n--- Account Login ---\n";
    std::cout << "Enter Account Number: ";
    if (!(std::cin >> accNum)) {
        std::cout << "Invalid account number!\n";
        clearInput();
        return;
    }
    clearInput();

    std::cout << "Enter Password: ";
    std::getline(std::cin, password);

    AccountNode* account = accounts.search(accNum);
    if (account == nullptr || account->password != password) {
        std::cout << "Incorrect credentials!\n";
        return;
    }

    currentUser = account;
    std::cout << "\nWelcome back, " << currentUser->holderName << "!\n";
}

void BankingSystem::logout() {
    if (currentUser != nullptr) {
        std::cout << "\n" << currentUser->holderName << ", you have been logged out.\n";
    }
    currentUser = nullptr;
}

void BankingSystem::viewAccountSummary() const {
    if (currentUser == nullptr) {
        std::cout << "Please login first.\n";
        return;
    }

    std::cout << "\n--- Account Summary ---\n";
    std::cout << "Account Number : " << currentUser->accountNumber << "\n";
    std::cout << "Holder Name    : " << currentUser->holderName << "\n";
    std::cout << "Current Balance: $" << std::fixed << std::setprecision(2) << currentUser->balance << "\n";
    std::cout << "Total Transactions: " << currentUser->history->getCount() << "\n";
}

void BankingSystem::enqueueTransaction(const std::string& type, double amount) {
    if (currentUser == nullptr) {
        std::cout << "Please login first.\n";
        return;
    }

    queue.enqueue(currentUser->accountNumber, type, amount);
    std::cout << "Queued successfully. It will be executed after you exit (Menu option 6).\n";
}

void BankingSystem::deposit() {
    if (currentUser == nullptr) {
        std::cout << "Please login first.\n";
        return;
    }

    double amount = 0.0;
    std::cout << "\nEnter amount to deposit: $";
    if (!(std::cin >> amount)) {
        std::cout << "Invalid amount!\n";
        clearInput();
        return;
    }
    clearInput();

    if (amount <= 0) {
        std::cout << "Amount must be greater than zero.\n";
        return;
    }

    std::cout << "Select mode:\n1. Immediate (apply now)\n2. Queue for end-of-day processing\nChoice: ";
    int mode = 0;
    if (!(std::cin >> mode)) {
        std::cout << "Invalid mode!\n";
        clearInput();
        return;
    }
    clearInput();

    if (mode == 1) {
        currentUser->balance += amount;
        currentUser->history->addTransaction("Deposit", amount);
        saveAccountsToFile();
        std::cout << "Deposit successful! New balance: $" << std::fixed << std::setprecision(2)
                  << currentUser->balance << "\n";
    } else if (mode == 2) {
        enqueueTransaction("Deposit", amount);
    } else {
        std::cout << "Invalid choice!\n";
    }
}

void BankingSystem::withdraw() {
    if (currentUser == nullptr) {
        std::cout << "Please login first.\n";
        return;
    }

    double amount = 0.0;
    std::cout << "\nEnter amount to withdraw: $";
    if (!(std::cin >> amount)) {
        std::cout << "Invalid amount!\n";
        clearInput();
        return;
    }
    clearInput();

    if (amount <= 0) {
        std::cout << "Amount must be greater than zero.\n";
        return;
    }

    std::cout << "Select mode:\n1. Immediate (apply now)\n2. Queue for end-of-day processing\nChoice: ";
    int mode = 0;
    if (!(std::cin >> mode)) {
        std::cout << "Invalid mode!\n";
        clearInput();
        return;
    }
    clearInput();

    if (mode == 1) {
        if (currentUser->balance < amount) {
            std::cout << "Insufficient balance! Current balance: $" << std::fixed << std::setprecision(2)
                      << currentUser->balance << "\n";
            return;
        }

        currentUser->balance -= amount;
        currentUser->history->addTransaction("Withdrawal", amount);
        saveAccountsToFile();

        std::cout << "Withdrawal successful! New balance: $" << std::fixed << std::setprecision(2)
                  << currentUser->balance << "\n";
    } else if (mode == 2) {
        std::cout << "Note: queued withdrawals will validate balance during nightly processing.\n";
        enqueueTransaction("Withdrawal", amount);
    } else {
        std::cout << "Invalid choice!\n";
    }
}

void BankingSystem::displayHistory() const {
    if (currentUser == nullptr) {
        std::cout << "Please login first.\n";
        return;
    }

    std::cout << "\n--- Transaction History ---\n";
    currentUser->history->display();
}

void BankingSystem::displayPendingQueue() const {
    queue.display();
}

void BankingSystem::loadAccountsFromFile() {
    std::ifstream file(dataFilePath);
    if (!file.is_open()) {
        return;
    }

    std::string line;
    AccountNode* currentSection = nullptr;

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        if (line.rfind("ACCOUNT|", 0) == 0) {
            std::stringstream ss(line);
            std::string tag;
            std::string accStr;
            std::string name;
            std::string password;
            std::string balanceStr;

            std::getline(ss, tag, '|');
            std::getline(ss, accStr, '|');
            std::getline(ss, name, '|');
            std::getline(ss, password, '|');
            std::getline(ss, balanceStr, '|');

            try {
                int accNum = std::stoi(accStr);
                double balance = std::stod(balanceStr);
                accounts.insert(accNum, name, password, balance);
                currentSection = accounts.search(accNum);
            } catch (const std::exception&) {
                std::cout << "Skipping malformed account record: " << line << "\n";
                currentSection = nullptr;
            }
            continue;
        }

        if (line.rfind("TXN|", 0) == 0) {
            if (currentSection == nullptr) {
                std::cout << "Dangling transaction record ignored: " << line << "\n";
                continue;
            }

            std::stringstream ss(line);
            std::string tag;
            std::string type;
            std::string amountStr;
            std::string dateTime;

            std::getline(ss, tag, '|');
            std::getline(ss, type, '|');
            std::getline(ss, amountStr, '|');
            std::getline(ss, dateTime);

            try {
                double amount = std::stod(amountStr);
                currentSection->history->addTransactionRecord(type, amount, dateTime);
            } catch (const std::exception&) {
                std::cout << "Skipping malformed transaction: " << line << "\n";
            }
            continue;
        }

        if (line == "END") {
            currentSection = nullptr;
            continue;
        }

        // Legacy flat format fallback: acc|name|password|balance
        std::stringstream legacy(line);
        std::string accStr;
        std::string name;
        std::string password;
        std::string balanceStr;
        if (std::getline(legacy, accStr, '|') && std::getline(legacy, name, '|') &&
            std::getline(legacy, password, '|') && std::getline(legacy, balanceStr)) {
            try {
                int accNum = std::stoi(accStr);
                double balance = std::stod(balanceStr);
                accounts.insert(accNum, name, password, balance);
            } catch (const std::exception&) {
                std::cout << "Skipping malformed legacy record: " << line << "\n";
            }
        }
    }
}

void BankingSystem::saveAccountsToFile() const {
    std::ofstream file(dataFilePath, std::ios::trunc);
    if (!file.is_open()) {
        std::cout << "Error: Unable to save accounts to file.\n";
        return;
    }

    auto formatMoney = [](double value) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << value;
        return oss.str();
    };

    accounts.forEach([&file, &formatMoney](const AccountNode* node) {
        file << "ACCOUNT|" << node->accountNumber << '|' << node->holderName << '|' << node->password << '|'
             << formatMoney(node->balance) << '\n';

        node->history->forEach([&file, &formatMoney](const TransactionNode* txn) {
            file << "TXN|" << txn->type << '|' << formatMoney(txn->amount) << '|' << txn->dateTime << '\n';
        });

        file << "END\n";
    });
}

void BankingSystem::ensureDataDirectory() const {
    std::filesystem::path path(dataFilePath);
    std::filesystem::path folder = path.parent_path();
    if (!folder.empty()) {
        std::error_code ec;
        std::filesystem::create_directories(folder, ec);
        if (ec) {
            std::cout << "Warning: Unable to verify data folder (" << ec.message() << ").\n";
        }
    }
}
