#include <iostream>
#include <fstream>
#include <cstring>

const int TABLE_SIZE = 10;
const int MAX_RECORDS = 100;

struct Record {
    int key;
    char data[50];
};

struct Node {
    Record record;
    Node* next;
};

class HashTable {
private:
    Node* table[TABLE_SIZE];

public:
    HashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = nullptr;
        }
    }

    ~HashTable() {
        clear();
    }

    void printTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            std::cout << "Index " << i << ": ";
            Node* current = table[i];
            while (current != nullptr) {
                std::cout << "[" << current->record.key << ", " << current->record.data << "] ";
                current = current->next;
            }
            std::cout << std::endl;
        }
    }

    void printRecords() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                std::cout << "Key: " << current->record.key << std::endl;
                std::cout << "Data: " << current->record.data << std::endl;
                std::cout << std::endl;
                current = current->next;
            }
        }
    }

    void clear() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
            table[i] = nullptr;
        }
    }

    void insert(const Record& record) {
        int index = hash(record.key);
        Node* newNode = new Node;
        newNode->record = record;
        newNode->next = table[index];
        table[index] = newNode;
    }

    Node* search(int key) {
        int index = hash(key);
        Node* current = table[index];
        while (current != nullptr) {
            if (current->record.key == key) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    void saveToFile(const char* filename) {
        std::ofstream file(filename);
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                file.write(reinterpret_cast<const char*>(&current->record), sizeof(Record));
                current = current->next;
            }
        }
        file.close();
    }

    void loadFromFile(const char* filename) {
        clear();
        std::ifstream file(filename);
        Record record;
        while (file.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
            insert(record);
        }
        file.close();
    }

private:
    int hash(int key) {
        return key % TABLE_SIZE;
    }
};

void addRecord(HashTable& hashtable) {
    Record record;
    std::cout << "Enter key: ";
    std::cin >> record.key;
    std::cout << "Enter data: ";
    std::cin.ignore();
    std::cin.getline(record.data, sizeof(record.data));

    hashtable.insert(record);
    std::cout << "Record added successfully!" << std::endl;
}

void searchRecord(HashTable& hashtable) {
    int key;
    std::cout << "Enter key to search: ";
    std::cin >> key;

    Node* result = hashtable.search(key);
    if (result != nullptr) {
        std::cout << "Record found!" << std::endl;
        std::cout << "Key: " << result->record.key << std::endl;
        std::cout << "Data: " << result->record.data << std::endl;
    }
    else {
        std::cout << "Record not found!" << std::endl;
    }
}

int main() {
    HashTable hashtable;

    while (true) {
        std::cout << "1. Add record" << std::endl;
        std::cout << "2. Search record" << std::endl;
        std::cout << "3. Save records to file and clear memory" << std::endl;
        std::cout << "4. Load records from file" << std::endl;
        std::cout << "5. Print table" << std::endl;
        std::cout << "6. Print records" << std::endl;
        std::cout << "7. Exit" << std::endl;

        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            addRecord(hashtable);
            break;
        case 2:
            searchRecord(hashtable);
            break;
        case 3: {
            std::cout << "Enter filename to save records: ";
            std::string filename;
            std::cin >> filename;
            hashtable.saveToFile(filename.c_str());
            hashtable.clear();
            std::cout << "Records saved to file and memory cleared!" << std::endl;
            break;
        }
        case 4: {
            std::cout << "Enter filename to load records: ";
            std::string filename;
            std::cin >> filename;
            hashtable.loadFromFile(filename.c_str());
            std::cout << "Records loaded from file!" << std::endl;
            break;
        }
        case 5:
            std::cout << "Hash table:" << std::endl;
            hashtable.printTable();
            break;
        case 6:
            std::cout << "Records:" << std::endl;
            hashtable.printRecords();
            break;
        case 7:
            return 0;
        default:
            std::cout << "Invalid choice! Please try again." << std::endl;
        }
    }

    return 0;
}
