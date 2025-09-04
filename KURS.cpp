#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <windows.h>

using namespace std;

struct Book{
    int number;
    char author[256];
    char title[256];
    int publicationYear;
    char publisher[256];
    int pageCount;
};

void trimSpace(char* str) {
    int i = 0, j = 0;
    bool spaceFound = false;

    while (isspace(str[i])) i++;

    while (str[i]) {
        if (isspace(str[i])) {
            if (!spaceFound) {
                str[j++] = ' ';
                spaceFound = true;
            }
        }
        else {
            str[j++] = str[i];
            spaceFound = false;
        }
        i++;
    }

    if (j > 0 && isspace(str[j - 1])) j--;
    str[j] = '\0';
}

void createFile() {
    string filename;
    cout << "Введите имя файла для создания: ";
    cin >> filename;
    ofstream file(filename, ios::out | ios::binary);
    file.close();
    cout << "Файл успешно создан." << endl;
}

void viewFile(const char* filename) {
    ifstream file(filename, ios::in | ios::binary);

    if (!file) {
        cerr << "Ошибка открытия файла." << endl;
        return;
    }

    cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "| Номер |         Автор          |             Название             | Год |           Издательство          | Страниц |" << endl;
    cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;

    Book book;
    while (file.read((char*)&book, sizeof(Book))) {
        cout << "| " << left << setw(6) << book.number << "|"
            << setw(24) << book.author << "|"
            << setw(34) << book.title << "|"
            << setw(5) << book.publicationYear << "|"
            << setw(33) << book.publisher << "|"
            << setw(9) << book.pageCount << "|" << endl;
        cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
    }


    file.close();
}

void addNewBook() {
    string filename;
    cout << "Введите название файла для добавления: ";
    cin >> filename;
    ofstream file(filename, ios::app | ios::binary);

    Book newBook;
    cout << "Введите номер книги: ";
    while (true) {
        cin >> newBook.number;
        if (cin.fail() || newBook.number < 0 ) {
            cin.clear();
            cin.ignore(256, '\n');
            cout << "Некорректное число." << endl;
        }
        else {
            cin.ignore(256, '\n');
            break;
        }
    }

    cout << "Введите имя автора: ";
    cin.getline(newBook.author, 256);
    trimSpace(newBook.author);

    cout << "Введите название: ";
    cin.getline(newBook.title, 256);
    trimSpace(newBook.title);

    cout << "Введите год издания: ";
    while (true) {
        cin >> newBook.publicationYear;
        if (cin.fail() || newBook.publicationYear < 0 || newBook.publicationYear > 2025){
            cin.clear(); 
            cin.ignore(256, '\n');
            cout << "Некорректный год." << endl;
        }
        else {
            cin.ignore(256, '\n');
            break;
        }
    }

    cout << "Введите издательство: ";
    cin.getline(newBook.publisher, 256);
    trimSpace(newBook.publisher);

    cout << "Введите количество страниц: ";
    while (true) {
        cin >> newBook.pageCount;
        if (cin.fail() || newBook.pageCount < 0) {
            cin.clear(); 
            cin.ignore(256, '\n'); 
            cout << "Некорректное число." << endl;
        }
        else {
            cin.ignore(256, '\n');
            break;
        }
    }

    file.write((char*)&newBook, sizeof(Book));
    file.close();
}

void deleteBook() {
    string filename;
    cout << "Введите имя файла: ";
    cin >> filename;
    viewFile(filename.c_str());

    int bookNumber;
    cout << "Введите номер книги для удаления: ";
    cin >> bookNumber;

    ifstream file(filename, ios::in | ios::binary);
    if (!file) {
        cerr << "Ошибка открытия файла." << endl;
        return;
    }

    ofstream tempFile("temp.bin", ios::out | ios::binary);
    if (!tempFile) {
        cerr << "Ошибка создания временного файла." << endl;
        file.close();
        return;
    }

    Book book;
    bool found = false;
    while (file.read((char*)&book, sizeof(Book))) {
        if (book.number != bookNumber) {
            tempFile.write((char*)&book, sizeof(Book));
        }
        else {
            found = true;
        }
    }

    file.close();
    tempFile.close();

    if (found) {
        remove(filename.c_str());
        rename("temp.bin", filename.c_str());
        cout << "Книга успешно удалена." << endl;
    }
    else {
        remove("temp.bin");
        cout << "Книга с указанным номером не найдена." << endl;
    }
}

void selectionSort(const char* filename) {
    ifstream file(filename, ios::in | ios::binary);
    if (!file) {
        cerr << "Файл не найден или не может быть открыт." << endl;
        return;
    }

    Book books[256];
    int count = 0;

    while (file.read((char*)&books[count], sizeof(Book))) {
        count++;
    }

    file.close();

    for (int i = 0; i < count - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < count; j++) {
            if (strcmp(books[j].author, books[minIndex].author) < 0) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            swap(books[i], books[minIndex]);
        }
    }

    ofstream outFile(filename, ios::out | ios::trunc | ios::binary);
    for (int i = 0; i < count; i++) {
        outFile.write((char*)&books[i], sizeof(Book));
    }
    outFile.close();
}

void insertSort(const char* filename) {
    ifstream file(filename, ios::in | ios::binary);
    if (!file) {
        cerr << "Ошибка открытия файла." << endl;
        return;
    }

    Book books[256];
    int count = 0;

    while (file.read((char*)&books[count], sizeof(Book))) {
        count++;
    }
    file.close();

    // Сортировка вставками по авторам
    for (int i = 1; i < count; i++) {
        Book key = books[i];
        int j = i - 1;

        // Сдвигаем элементы, которые больше ключа
        while (j >= 0 && strcmp(books[j].author, key.author) > 0) {
            books[j + 1] = books[j];
            j = j - 1;
        }
        books[j + 1] = key;
    }

    ofstream outFile(filename, ios::out | ios::trunc | ios::binary);
    for (int i = 0; i < count; i++) {
        outFile.write((char*)&books[i], sizeof(Book));
    }
    outFile.close();
}

void findAuthor() {
    char searchAuthor[256];
    cout << "Введите имя автора для поиска: ";
    cin.ignore();
    cin.getline(searchAuthor, 256);
    trimSpace(searchAuthor);

    string filename;
    cout << "Введите название файла: ";
    cin >> filename;
    insertSort(filename.c_str());
    ifstream file(filename, ios::in | ios::binary);
    if (!file) {
        cerr << "Ошибка открытия файла." << endl;
        return;
    }

    file.seekg(0, ios::end);
    int size = file.tellg();
    int numberBooks = size / sizeof(Book);
    int left = 0;
    int right = numberBooks - 1;
    bool found = false;
    Book book;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        file.seekg(mid * sizeof(Book));
        file.read((char*)&book, sizeof(Book));

        if (strcmp(book.author, searchAuthor) == 0) {
            cout << "Найденные книги автора " << book.author << ":" << endl;
            found = true;
            int position = mid;
            while (position > 0) {
                position--;
                file.seekg(position * sizeof(Book));
                file.read((char*)&book, sizeof(Book));
                if (strcmp(book.author, searchAuthor) != 0) {
                    break;
                }
            }
            position++;
            file.seekg(position * sizeof(Book));
            while (position < numberBooks) {
                file.read((char*)&book, sizeof(Book));
                if (strcmp(book.author, searchAuthor) != 0) {
                    break;
                }
                cout << "- " << book.title << " (" << book.publicationYear << ")" << endl;
                position++;
            }
            break;
        }
        else if (strcmp(book.author, searchAuthor) < 0) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    if (!found) {
        cout << "Книги указанного автора не найдены." << endl;
    }

    file.close();
}

void findYear() {
    int year;
    do {
        cout << "Введите год: ";
        cin >> year;
    } while (year < 0 && year > 2025);

    string filename;
    cout << "Введите имя файла: ";
    cin >> filename;

    ifstream file(filename, ios::in | ios::binary);
    if (!file) {
        cerr << "Ошибка открытия файла." << endl;
        return;
    }

    cout << "Книги, изданные после " << year << " года:" << endl;

    Book book;
    bool found = false;

    while (file.read((char*)&book, sizeof(Book))) {
        if (book.publicationYear > year) {
            cout <<"- " << book.author << ": " << book.title
                << " (" << book.publicationYear << ")" << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "Книги, изданные после " << year << " года, не найдены." << endl;
    }

    file.close();
}

int main() {
    SetConsoleCP(1251);       
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Rus");
    string filename;
    int choice;
    do {
        cout << "Меню управления библиотекой:" << endl;
        cout << "1. Создать новый файл" << endl;
        cout << "2. Просмотреть книги" << endl;
        cout << "3. Добавить новую книгу" << endl;
        cout << "4. Удалить книгу" << endl;
        cout << "5. Найти книги по автору" << endl;
        cout << "6. Показать книги, изданные после указанного года" << endl;
        cout << "7. Сортировать книги по авторам" << endl;
        cout << "0. Выход" << endl;
        cout << "Выберите действие: " << endl;
        cin >> choice;

        switch (choice) {
        case 1:
            createFile();
            cout << endl;    
            break;
        case 2:
            cout << "Введите имя файла: ";
            cin >> filename;
            viewFile(filename.c_str());
            cout << endl;
            break;
        case 3:
            addNewBook();
            cout << endl;
            break;
        case 4:
            deleteBook();
            cout << endl;
            break;
        case 5:
            findAuthor();
            cout << endl;
            break;
        case 6:
            findYear();
            cout << endl;
            break;
        case 7:
            int choicesort;
            cout << "1. Selection Sort" << endl;
            cout << "2. Insertion Sort" << endl;
            cin >> choicesort;
            switch (choicesort) {
            case 1:
                cout << "Введите имя файла для сортировки: ";
                cin >> filename;
                selectionSort(filename.c_str());
                cout << "Записи отсортированы по авторам." << endl;
                break;
            case 2:
                cout << "Введите имя файла для сортировки: ";
                cin >> filename;
                insertSort(filename.c_str());
                cout << "Записи отсортированы по авторам." << endl;
                break;
            default:
                cout << "Неверный выбор. Попробуйте снова." << endl;
            }
            cout << endl;
            break;
        case 0:
            cout << "Завершение работы программы." << endl;
            break;
        default:
            cout << "Неверный выбор. Попробуйте снова." << endl;
            cout << endl;
        }
    } while (choice != 0);
}