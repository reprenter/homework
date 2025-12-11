#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <iostream>

class Book {
private:
    std::string title;
    std::string author;
    int year;
    std::string isbn;
    bool isAvailable;
    std::string borrowedBy;

public:
    Book(const std::string& title, const std::string& author, int year, const std::string& isbn);
    
    std::string getTitle() const;
    std::string getAuthor() const;
    int getYear() const;
    std::string getISBN() const;
    bool isBookAvailable() const;
    std::string getBorrowedBy() const;
    void borrowBook(const std::string& userName);
    void returnBook();
    void displayInfo() const;
    void setTitle(const std::string& title);
    void setAuthor(const std::string& author);
    void setYear(int year);
    void setISBN(const std::string& isbn);
    void setAvailable(bool available);
    void setBorrowedBy(const std::string& name);
};

#endif