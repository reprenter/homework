#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <vector>

class User {
private:
    std::string name;
    std::string userId;
    std::vector<std::string> borrowedBooks;
    int maxBooksAllowed;

public:
    User(const std::string& name, const std::string& userId);
    
    std::string getName() const;
    std::string getUserId() const;
    const std::vector<std::string>& getBorrowedBooks() const;
    int getMaxBooksAllowed() const;
    bool canBorrowMore() const;
    void addBook(const std::string& isbn);
    void removeBook(const std::string& isbn);
    void displayProfile() const;
};

#endif