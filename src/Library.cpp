#include "Library.h"
#include <fstream>
#include <iostream>
#include <sstream>

Library::Library(const std::string& dataFile) : dataFile(dataFile) {
    loadFromFile();
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
    Book* currentBook = nullptr;
    User* currentUser = nullptr;

    while (getline(file, line)) {
        if (line.empty()) continue;

        if (line.find("---USERS---") != std::string::npos) {
            inUsersSection = true;
            continue;
        }

        if (line.find("BOOK") == 0 && !inUsersSection) {
            // Начало новой книги
            currentBook = new Book("", "", 0, "");
            continue;
        }

        if (line.find("USER") == 0 && inUsersSection) {
            // Начало нового пользователя
            currentUser = new User("", "");
            continue;
        }

        if (!inUsersSection && currentBook) {
            // Парсинг данных книги
            if (line.find("Title: ") == 0) {
                currentBook->setTitle(line.substr(7));
            } else if (line.find("Author: ") == 0) {
                currentBook->setAuthor(line.substr(8));
            } else if (line.find("Year: ") == 0) {
                try {
                    currentBook->setYear(std::stoi(line.substr(6)));
                } catch (...) {
                    currentBook->setYear(2000); // Значение по умолчанию
                }
            } else if (line.find("ISBN: ") == 0) {
                currentBook->setISBN(line.substr(6));
            } else if (line.find("Available: ") == 0) {
                currentBook->setAvailable(line.substr(11) == "yes");
            } else if (line.find("BorrowedBy: ") == 0) {
                currentBook->setBorrowedBy(line.substr(12));
            } else if (line == "" && !line.empty()) {
                // Пустая строка означает конец данных книги
                books.push_back(*currentBook);
                delete currentBook;
                currentBook = nullptr;
            }
        } else if (inUsersSection && currentUser) {
            // Парсинг данных пользователя
            if (line.find("Name: ") == 0) {
                currentUser->setName(line.substr(6));
            } else if (line.find("UserID: ") == 0) {
                currentUser->setUserId(line.substr(8));
            } else if (line.find("BorrowedBooks: ") == 0) {
                std::string booksStr = line.substr(15);
                if (!booksStr.empty()) {
                    std::stringstream ss(booksStr);
                    std::string isbn;
                    while (getline(ss, isbn, '|')) {
                        currentUser->addBook(isbn);
                    }
                }
            } else if (line.find("MaxBooks: ") == 0) {
                try {
                    currentUser->setMaxBooks(std::stoi(line.substr(10)));
                } catch (...) {
                    currentUser->setMaxBooks(3); // Значение по умолчанию
                }
            } else if (line == "" && !line.empty()) {
                // Пустая строка означает конец данных пользователя
                users.push_back(*currentUser);
                delete currentUser;
                currentUser = nullptr;
            }
        }
    }

    // Добавление последних объектов, если файл не заканчивается пустой строкой
    if (currentBook) {
        books.push_back(*currentBook);
        delete currentBook;
    }
    if (currentUser) {
        users.push_back(*currentUser);
        delete currentUser;
    }

    file.close();
}