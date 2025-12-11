#ifndef LIBRARY_H
#define LIBRARY_H

#include "Book.h"
#include "User.h"
#include <vector>
#include <string>

class Library {
private:
    std::vector<Book> books;
    std::vector<User> users;
    std::string dataFile;

public:
    Library(const std::string& dataFile);

    void addBook(const Book& book);
    void addUser(const User& user);
    Book* findBookByISBN(const std::string& isbn);
    User* findUserByName(const std::string& name);
    void displayAllBooks() const;
    void displayAllUsers() const;
    void saveToFile() const;
    void loadFromFile();
};

#endif