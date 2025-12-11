#include "Book.h"
#include <stdexcept>
#include <ctime>

Book::Book(const std::string& title, const std::string& author, int year, const std::string& isbn)
    : title(title), author(author), year(year), isbn(isbn), isAvailable(true) {
    time_t now = time(0);
    tm* current_time = localtime(&now);
    int current_year = current_time->tm_year + 1900;
    
    if (year < 1450 || year > current_year) {
        throw std::invalid_argument("Invalid publication year");
    }
}

std::string Book::getTitle() const {
    return title;
}

std::string Book::getAuthor() const {
    return author;
}

int Book::getYear() const {
    return year;
}

std::string Book::getISBN() const {
    return isbn;
}

bool Book::isBookAvailable() const {
    return isAvailable;
}

std::string Book::getBorrowedBy() const {
    return borrowedBy;
}

void Book::borrowBook(const std::string& userName) {
    if (!isAvailable) {
        throw std::runtime_error("Book is already borrowed");
    }
    isAvailable = false;
    borrowedBy = userName;
}

void Book::returnBook() {
    if (isAvailable) {
        throw std::runtime_error("Book is already available");
    }
    isAvailable = true;
    borrowedBy.clear();
}

void Book::displayInfo() const {
    std::cout << "Title: " << title << std::endl;
    std::cout << "Author: " << author << std::endl;
    std::cout << "Year: " << year << std::endl;
    std::cout << "ISBN: " << isbn << std::endl;
    std::cout << "Status: " << (isAvailable ? "Available" : "Borrowed by " + borrowedBy) << std::endl;
}

void Book::setTitle(const std::string& title) {
    this->title = title;
}

void Book::setAuthor(const std::string& author) {
    this->author = author;
}

void Book::setYear(int year) {
    time_t now = time(0);
    tm* current_time = localtime(&now);
    int current_year = current_time->tm_year + 1900;
    
    if (year < 1450 || year > current_year) {
        throw std::invalid_argument("Invalid publication year");
    }
    this->year = year;
}

void Book::setISBN(const std::string& isbn) {
    if (isbn.empty()) {
        throw std::invalid_argument("ISBN cannot be empty");
    }
    this->isbn = isbn;
}

void Book::setAvailable(bool available) {
    this->isAvailable = available;
}

void Book::setBorrowedBy(const std::string& name) {
    this->borrowedBy = name;
    this->isAvailable = name.empty();
}