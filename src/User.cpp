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