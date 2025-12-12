#include "Library.h"
#include <fstream>
#include <iostream>
#include <sstream>

Library::Library(const std::string& dataFile) : dataFile(dataFile) { loadFromFile(); }

void Library::addBook(const Book& book) { books.push_back(book); }

void Library::addUser(const User& user) { users.push_back(user); }

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

void Library::saveToFile() const {
    std::ofstream file(dataFile);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file for writing: " + dataFile);
    }

    // Сохранение книг
    for (const Book& book : books) {
        file << "BOOK\n";
        file << "Title: " << book.getTitle() << "\n";
        file << "Author: " << book.getAuthor() << "\n";
        file << "Year: " << book.getYear() << "\n";
        file << "ISBN: " << book.getISBN() << "\n";
        file << "Available: " << (book.isBookAvailable() ? "yes" : "no") << "\n";
        file << "BorrowedBy: " << book.getBorrowedBy() << "\n\n";
    }

    // Разделитель секций
    file << "---USERS---\n\n";

    // Сохранение пользователей
    for (const User& user : users) {
        file << "USER\n";
        file << "Name: " << user.getName() << "\n";
        file << "UserID: " << user.getUserId() << "\n";
        
        // Формирование строки с ISBN книг
        std::string borrowedBooksStr;
        const std::vector<std::string>& borrowed = user.getBorrowedBooks();
        for (size_t i = 0; i < borrowed.size(); ++i) {
            borrowedBooksStr += borrowed[i];
            if (i < borrowed.size() - 1) {
                borrowedBooksStr += "|";
            }
        }
        
        file << "BorrowedBooks: " << borrowedBooksStr << "\n";
        file << "MaxBooks: " << user.getMaxBooksAllowed() << "\n\n";
    }

    file.close();
}

void Library::loadFromFile() {
    std::ifstream file(dataFile);
    if (!file.is_open()) {
        // Создаем пустой файл с заготовкой
        std::ofstream newFile(dataFile);
        if (newFile.is_open()) {
            newFile << "---USERS---\n";
            newFile.close();
        }
        return;
    }

    std::string line;
    bool inUsersSection = false;
    books.clear();
    users.clear();

    while (getline(file, line)) {
        if (line.empty()) continue;

        if (line.find("---USERS---") != std::string::npos) {
            inUsersSection = true;
            continue;
        }

        if (!inUsersSection && line.find("BOOK") != std::string::npos) {
            std::string title, author, isbn, borrowedBy;
            int year = 0;
            bool available = true;

            while (getline(file, line) && !line.empty()) {
                if (line.find("Title: ") == 0) {
                    title = line.substr(7);
                } else if (line.find("Author: ") == 0) {
                    author = line.substr(8);
                } else if (line.find("Year: ") == 0) {
                    try {
                        year = std::stoi(line.substr(6));
                    } catch (...) {
                        year = 2000; 
                    }
                } else if (line.find("ISBN: ") == 0) {
                    isbn = line.substr(6);
                } else if (line.find("Available: ") == 0) {
                    available = (line.substr(11) == "yes");
                } else if (line.find("BorrowedBy: ") == 0) {
                    borrowedBy = line.substr(12);
                }
            }

            try {
                if (!title.empty() && !author.empty() && !isbn.empty()) {
                    Book book(title, author, year, isbn);
                    if (!available && !borrowedBy.empty()) {
                        book.borrowBook(borrowedBy);
                    }
                    books.push_back(book);
                }
            } catch (const std::exception& e) {
                std::cerr << "Ошибка при загрузке книги '" << title << "': " << e.what() << std::endl;
            }
        } else if (inUsersSection && line.find("USER") != std::string::npos) {
            std::string name, userId, borrowedBooksStr;
            int maxBooks = 3;

            while (getline(file, line) && !line.empty()) {
                if (line.find("Name: ") == 0) {
                    name = line.substr(6);
                } else if (line.find("UserID: ") == 0) {
                    userId = line.substr(8);
                } else if (line.find("BorrowedBooks: ") == 0) {
                    borrowedBooksStr = line.substr(15);
                } else if (line.find("MaxBooks: ") == 0) {
                    try {
                        maxBooks = std::stoi(line.substr(10));
                    } catch (...) {
                        maxBooks = 3;
                    }
                }
            }

            if (!name.empty() && !userId.empty()) {
                User user(name, userId);
                if (!borrowedBooksStr.empty()) {
                    std::stringstream ss(borrowedBooksStr);
                    std::string isbn;
                    while (getline(ss, isbn, '|')) {
                        if (!isbn.empty()) {
                            user.addBook(isbn);
                        }
                    }
                }
                users.push_back(user);
            }
        }
    }

    file.close();
}

void Library::borrowBook(const std::string& userName, const std::string& isbn) {
    User* user = findUserByName(userName);
    if (!user) {
        throw std::runtime_error("User not found: " + userName);
    }
    
    if (!user->canBorrowMore()) {
        throw std::runtime_error("User has reached maximum book limit");
    }
    
    Book* book = findBookByISBN(isbn);
    if (!book) {
        throw std::runtime_error("Book not found with ISBN: " + isbn);
    }
    
    if (!book->isBookAvailable()) {
        throw std::runtime_error("Book is already borrowed by " + book->getBorrowedBy());
    }
    
    book->borrowBook(userName);
    user->addBook(isbn);
}

void Library::returnBook(const std::string& isbn) {
    Book* book = findBookByISBN(isbn);
    if (!book) {
        throw std::runtime_error("Book not found with ISBN: " + isbn);
    }
    
    if (book->isBookAvailable()) {
        throw std::runtime_error("Book is already available in library");
    }
    
    User* user = findUserByName(book->getBorrowedBy());
    if (!user) {
        throw std::runtime_error("Original borrower not found");
    }
    
    book->returnBook();
    user->removeBook(isbn);
}