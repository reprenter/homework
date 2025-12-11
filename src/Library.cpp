#include "Library.h"
#include <fstream>
#include <iostream>

Library::Library(const std::string& dataFile) : dataFile(dataFile) {
    std::ifstream file(dataFile);
    if (!file.is_open()) {
        std::ofstream newFile(dataFile);
        if (newFile.is_open()) {
            newFile << "---USERS---" << std::endl;
            newFile.close();
        }
    }
}

void Library::addBook(const Book& book) {
    books.push_back(book);
}

void Library::addUser(const User& user) {
    users.push_back(user);
}

Book* Library::findBookByISBN(const std::string& isbn) {
    for (Book& book : books) {
        if (book.getISBN() == isbn) {
            return &book;
        }
    }
    return nullptr;
}

User* Library::findUserByName(const std::string& name) {
    for (User& user : users) {
        if (user.getName() == name) {
            return &user;
        }
    }
    return nullptr;
}

void Library::displayAllBooks() const {
    if (books.empty()) {
        std::cout << "No books in library" << std::endl;
        return;
    }
    for (const Book& book : books) {
        book.displayInfo();
        std::cout << "-------------------" << std::endl;
    }
}

void Library::displayAllUsers() const {
    if (users.empty()) {
        std::cout << "No users registered" << std::endl;
        return;
    }
    for (const User& user : users) {
        user.displayProfile();
        std::cout << "-------------------" << std::endl;
    }
}

void Library::saveToFile() const {}

void Library::loadFromFile() {}