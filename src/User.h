#ifndef USER_H
#define USER_H

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
};

#endif