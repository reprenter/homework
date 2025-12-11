#include "User.h"

User::User(const std::string& name, const std::string& userId)
    : name(name), userId(userId), maxBooksAllowed(3) {}

std::string User::getName() const {
    return name;
}

std::string User::getUserId() const {
    return userId;
}

const std::vector<std::string>& User::getBorrowedBooks() const {
    return borrowedBooks;
}

int User::getMaxBooksAllowed() const {
    return maxBooksAllowed;
}

bool User::canBorrowMore() const {
    return borrowedBooks.size() < maxBooksAllowed;
}

void User::addBook(const std::string& isbn) {
    if (!canBorrowMore()) {
        throw std::runtime_error("User has reached maximum book limit");
    }
    borrowedBooks.push_back(isbn);
}

void User::removeBook(const std::string& isbn) {
    auto it = std::find(borrowedBooks.begin(), borrowedBooks.end(), isbn);
    if (it == borrowedBooks.end()) {
        throw std::runtime_error("Book not found in user's borrowed list");
    }
    borrowedBooks.erase(it);
}

void User::displayProfile() const {
    std::cout << "Name: " << name << std::endl;
    std::cout << "User ID: " << userId << std::endl;
    std::cout << "Books borrowed: " << borrowedBooks.size() << "/" << maxBooksAllowed << std::endl;
    if (!borrowedBooks.empty()) {
        std::cout << "Borrowed ISBNs: ";
        for (size_t i = 0; i < borrowedBooks.size(); ++i) {
            std::cout << borrowedBooks[i];
            if (i < borrowedBooks.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
}