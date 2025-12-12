#include "Book.h"
#include "User.h"
#include "Library.h"
#include <iostream>
#include <string>
#include <limits>

void displayMenu() {
    std::cout << "\n=== БИБЛИОТЕКА ===\n";
    std::cout << "1. Просмотреть все книги\n";
    std::cout << "2. Просмотреть всех пользователей\n";
    std::cout << "3. Добавить новую книгу\n";
    std::cout << "4. Зарегистрировать пользователя\n";
    std::cout << "5. Выдать книгу пользователю\n";
    std::cout << "6. Принять книгу от пользователя\n";
    std::cout << "7. Поиск книги по ISBN\n";
    std::cout << "8. Просмотреть профиль пользователя\n";
    std::cout << "9. Сохранить данные в файл\n";
    std::cout << "10. Выход\n";
    std::cout << "Ваш выбор: ";
}

int getValidChoice(int min, int max) {
    int choice;
    while (true) {
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка ввода. Пожалуйста, введите число: ";
            continue;
        }
        if (choice < min || choice > max) {
            std::cout << "Неверный выбор. Введите число от " << min << " до " << max << ": ";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return choice;
    }
}

std::string getStringInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

int main() {
    try {
        Library library("../data/library_data.txt");
        std::cout << "Данные успешно загружены из файла!\n";
        
        int choice;
        do {
            displayMenu();
            choice = getValidChoice(1, 10);
            
            try {
                switch (choice) {
                    case 1: // Просмотреть все книги
                        std::cout << "\n=== КАТАЛОГ КНИГ ===\n";
                        library.displayAllBooks();
                        break;
                        
                    case 2: // Просмотреть всех пользователей
                        std::cout << "\n=== СПИСОК ПОЛЬЗОВАТЕЛЕЙ ===\n";
                        library.displayAllUsers();
                        break;
                        
                    case 3: { // Добавить новую книгу
                        std::cout << "\n=== ДОБАВЛЕНИЕ КНИГИ ===\n";
                        std::string title = getStringInput("Название книги: ");
                        std::string author = getStringInput("Автор: ");
                        std::string yearStr = getStringInput("Год издания: ");
                        int year = std::stoi(yearStr);
                        std::string isbn = getStringInput("ISBN: ");
                        
                        Book newBook(title, author, year, isbn);
                        library.addBook(newBook);
                        std::cout << "Книга успешно добавлена!\n";
                        break;
                    }
                        
                    case 4: { // Зарегистрировать пользователя
                        std::cout << "\n=== РЕГИСТРАЦИЯ ПОЛЬЗОВАТЕЛЯ ===\n";
                        std::string name = getStringInput("Фамилия пользователя: ");
                        std::string userId = getStringInput("ID пользователя (например, USR_001): ");
                        
                        User newUser(name, userId);
                        library.addUser(newUser);
                        std::cout << "Пользователь успешно зарегистрирован!\n";
                        break;
                    }
                        
                    case 5: { // Выдать книгу пользователю
                        std::cout << "\n=== ВЫДАЧА КНИГИ ===\n";
                        std::string userName = getStringInput("Фамилия пользователя: ");
                        std::string isbn = getStringInput("ISBN книги: ");
                        
                        library.borrowBook(userName, isbn);
                        std::cout << "Книга успешно выдана!\n";
                        break;
                    }
                        
                    case 6: { // Принять книгу от пользователя
                        std::cout << "\n=== ВОЗВРАТ КНИГИ ===\n";
                        std::string isbn = getStringInput("ISBN возвращаемой книги: ");
                        
                        library.returnBook(isbn);
                        std::cout << "Книга успешно возвращена!\n";
                        break;
                    }
                        
                    case 7: { // Поиск книги по ISBN
                        std::cout << "\n=== ПОИСК КНИГИ ===\n";
                        std::string isbn = getStringInput("Введите ISBN для поиска: ");
                        
                        Book* book = library.findBookByISBN(isbn);
                        if (book) {
                            std::cout << "\nКнига найдена:\n";
                            book->displayInfo();
                        } else {
                            std::cout << "Книга с ISBN " << isbn << " не найдена.\n";
                        }
                        break;
                    }
                        
                    case 8: { // Просмотреть профиль пользователя
                        std::cout << "\n=== ПРОФИЛЬ ПОЛЬЗОВАТЕЛЯ ===\n";
                        std::string name = getStringInput("Фамилия пользователя: ");
                        
                        User* user = library.findUserByName(name);
                        if (user) {
                            std::cout << "\nПрофиль пользователя:\n";
                            user->displayProfile();
                        } else {
                            std::cout << "Пользователь " << name << " не найден.\n";
                        }
                        break;
                    }
                        
                    case 9: // Сохранить данные в файл
                        library.saveToFile();
                        std::cout << "Данные успешно сохранены в файл!\n";
                        break;
                        
                    case 10: // Выход
                        std::cout << "Сохранение данных перед выходом...\n";
                        library.saveToFile();
                        std::cout << "До свидания!\n";
                        break;
                }
            } catch (const std::exception& e) {
                std::cerr << "Ошибка: " << e.what() << std::endl;
            }
            
            if (choice != 10) {
                std::cout << "\nНажмите Enter для продолжения...";
                std::cin.get();
            }
            
        } while (choice != 10);
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        return 1;
    }
}