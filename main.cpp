#include <iostream>
#include <string>
#include <fstream>
#include <limits>

using namespace std;

struct TemperatureRecord {
    string location;
    string date;
    string time;
    float temperature;
    TemperatureRecord* next;
};

TemperatureRecord* head = nullptr;

// Создание нового узла
TemperatureRecord* createRecord(const string& location, const string& date, const string& time, float temperature) {
    TemperatureRecord* newNode = new TemperatureRecord{ location, date, time, temperature, nullptr };
    return newNode;
}

// Печать одного узла
void printRecord(TemperatureRecord* node) {
    if (node) {
        cout << "Местность: " << node->location << ", Дата: " << node->date
             << ", Время: " << node->time << ", Температура: " << node->temperature << "°C\n";
    }
}

// 1. Вывод всех данных
void printAll() {
    if (!head) {
        cout << "Список пуст.\n";
        return;
    }
    TemperatureRecord* current = head;
    while (current) {
        printRecord(current);
        current = current->next;
    }
}

// 2. Добавление в начало
void addToStart(TemperatureRecord* newNode) {
    newNode->next = head;
    head = newNode;
}

// 3. Добавление в конец
void addToEnd(TemperatureRecord* newNode) {
    if (!head) {
        head = newNode;
        return;
    }
    TemperatureRecord* current = head;
    while (current->next) {
        current = current->next;
    }
    current->next = newNode;
}

// 4. Добавление по алфавиту
void addSortedByLocation(TemperatureRecord* newNode) {
    if (!head || newNode->location < head->location) {
        newNode->next = head;
        head = newNode;
        return;
    }
    TemperatureRecord* current = head;
    while (current->next && current->next->location < newNode->location) {
        current = current->next;
    }
    newNode->next = current->next;
    current->next = newNode;
}

// 5. Добавление после определенного показателя
void addAfter(string loc, TemperatureRecord* newNode) {
    TemperatureRecord* current = head;
    while (current && current->location != loc) {
        current = current->next;
    }
    if (!current) {
        cout << "Местность не найдена.\n";
        delete newNode;
        return;
    }
    newNode->next = current->next;
    current->next = newNode;
}

// 6. Добавление перед определенным показателем
void addBefore(string loc, TemperatureRecord* newNode) {
    if (!head) return;
    if (head->location == loc) {
        newNode->next = head;
        head = newNode;
        return;
    }
    TemperatureRecord* prev = nullptr;
    TemperatureRecord* current = head;
    while (current && current->location != loc) {
        prev = current;
        current = current->next;
    }
    if (!current) {
        cout << "Местность не найдена.\n";
        delete newNode;
        return;
    }
    prev->next = newNode;
    newNode->next = current;
}

// 7. Удаление узла с минимальной температурой
void deleteMinTemperature() {
    if (!head) return;

    TemperatureRecord *minNode = head, *minPrev = nullptr;
    TemperatureRecord *prev = nullptr, *current = head;

    while (current) {
        if (current->temperature < minNode->temperature) {
            minNode = current;
            minPrev = prev;
        }
        prev = current;
        current = current->next;
    }

    if (minNode == head) {
        head = head->next;
    } else {
        minPrev->next = minNode->next;
    }

    delete minNode;
    cout << "Удалён узел с минимальной температурой.\n";
}

// 8. Сохранение в бинарный файл
void saveToFile(const string& filename) {
    ofstream out(filename, ios::binary);
    TemperatureRecord* current = head;
    while (current) {
        size_t len = current->location.size();
        out.write((char*)&len, sizeof(len));
        out.write(current->location.c_str(), len);

        len = current->date.size();
        out.write((char*)&len, sizeof(len));
        out.write(current->date.c_str(), len);

        len = current->time.size();
        out.write((char*)&len, sizeof(len));
        out.write(current->time.c_str(), len);

        out.write((char*)&current->temperature, sizeof(current->temperature));

        current = current->next;
    }
    out.close();
    cout << "Список сохранён в файл.\n";
}

// 9. Загрузка из файла и добавление в начало
void loadFromFile(const string& filename) {
    ifstream in(filename, ios::binary);
    if (!in) {
        cout << "Ошибка открытия файла.\n";
        return;
    }

    while (in.peek() != EOF) {
        size_t len;
        string location, date, time;
        float temperature;

        in.read((char*)&len, sizeof(len));
        location.resize(len);
        in.read(&location[0], len);

        in.read((char*)&len, sizeof(len));
        date.resize(len);
        in.read(&date[0], len);

        in.read((char*)&len, sizeof(len));
        time.resize(len);
        in.read(&time[0], len);

        in.read((char*)&temperature, sizeof(temperature));

        addToStart(createRecord(location, date, time, temperature));
    }
    in.close();
    cout << "Загрузка завершена.\n";
}

// 10. Сведения о макс/мин температуре
void showMaxMin() {
    if (!head) {
        cout << "Список пуст.\n";
        return;
    }

    TemperatureRecord* maxNode = head;
    TemperatureRecord* minNode = head;
    TemperatureRecord* current = head;

    while (current) {
        if (current->temperature > maxNode->temperature)
            maxNode = current;
        if (current->temperature < minNode->temperature)
            minNode = current;
        current = current->next;
    }

    cout << "Максимальная температура:\n";
    printRecord(maxNode);
    cout << "Минимальная температура:\n";
    printRecord(minNode);
}

// 11. Поиск по местности и диапазону времени
void searchByLocationAndTime(const string& location, const string& startDate, const string& endDate) {
    TemperatureRecord* current = head;
    bool found = false;
    while (current) {
        if (current->location == location && current->date >= startDate && current->date <= endDate) {
            printRecord(current);
            found = true;
        }
        current = current->next;
    }
    if (!found)
        cout << "Нет данных по указанному запросу.\n";
}

// Меню
void showMenu() {
    cout << "\nМеню:\n"
         << "1. Показать все записи\n"
         << "2. Добавить в начало\n"
         << "3. Добавить в конец\n"
         << "4. Добавить по алфавиту\n"
         << "5. Добавить после местности\n"
         << "6. Добавить перед местностью\n"
         << "7. Удалить с минимальной температурой\n"
         << "8. Сохранить в файл\n"
         << "9. Загрузить из файла\n"
         << "10. Показать макс/мин температуру\n"
         << "11. Поиск по местности и дате\n"
         << "0. Выход\n";
}

// Ввод записи
TemperatureRecord* inputRecord() {
    string loc, date, time;
    float temp;
    cout << "Введите местность: ";
    cin >> loc;
    cout << "Введите дату (гггг-мм-дд): ";
    cin >> date;
    cout << "Введите время (чч:мм): ";
    cin >> time;
    cout << "Введите температуру: ";
    cin >> temp;
    return createRecord(loc, date, time, temp);
}

// Главная функция
int main() {
    setlocale(LC_ALL, "ru");
    int choice;
    string location, startDate, endDate;

    do {
        showMenu();
        cout << "Выбор: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // очистка ввода

        switch (choice) {
            case 1: printAll(); break;
            case 2: addToStart(inputRecord()); break;
            case 3: addToEnd(inputRecord()); break;
            case 4: addSortedByLocation(inputRecord()); break;
            case 5:
                cout << "После какой местности вставить? ";
                cin >> location;
                addAfter(location, inputRecord());
                break;
            case 6:
                cout << "Перед какой местностью вставить? ";
                cin >> location;
                addBefore(location, inputRecord());
                break;
            case 7: deleteMinTemperature(); break;
            case 8: saveToFile("data.bin"); break;
            case 9: loadFromFile("data.bin"); break;
            case 10: showMaxMin(); break;
            case 11:
                cout << "Введите местность: ";
                cin >> location;
                cout << "Введите начальную дату (гггг-мм-дд): ";
                cin >> startDate;
                cout << "Введите конечную дату (гггг-мм-дд): ";
                cin >> endDate;
                searchByLocationAndTime(location, startDate, endDate);
                break;
            case 0: cout << "Выход.\n"; break;
            default: cout << "Неверный выбор!\n"; break;
        }
    } while (choice != 0);

    return 0;
}
