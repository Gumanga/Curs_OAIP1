#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <regex>

using namespace std;

struct TemperatureRecord {
    string location;
    string date;
    string time;
    float temperature;
    TemperatureRecord* next;
};

TemperatureRecord* head = nullptr;

int readIntSafe(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        } else {
            cout << "Неверный ввод. Попробуйте снова.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

float readFloatSafe(const string& prompt) {
    float value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        } else {
            cout << "Неверный ввод. Попробуйте снова.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

string readNonEmptyString(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (!input.empty()) return input;
        cout << "Поле не может быть пустым. Повторите ввод.\n";
    }
}

bool isValidDate(const string& date) {
    regex pattern(R"((\d{4})-(\d{2})-(\d{2}))");
    smatch match;
    if (!regex_match(date, match, pattern)) {
        cout << "Неверный формат даты. Используйте ГГГГ-ММ-ДД.\n";
        return false;
    }
    int month = stoi(match[2]);
    int day = stoi(match[3]);
    if (month < 1 || month > 12 || day < 1 || day > 31) {
        cout << "Неверные значения дня или месяца.\n";
        return false;
    }
    return true;
}

bool isValidTime(const string& time) {
    regex pattern(R"(\d{2}:\d{2})");
    if (!regex_match(time, pattern)) {
        cout << "Неверный формат времени. Используйте ЧЧ:ММ.\n";
        return false;
    }
    return true;
}

TemperatureRecord* createRecord(const string& location, const string& date, const string& time, float temperature) {
    return new TemperatureRecord{ location, date, time, temperature, nullptr };
}

TemperatureRecord* inputRecord() {
    string loc = readNonEmptyString("Введите местность: ");
    string date;
    do {
        date = readNonEmptyString("Введите дату (гггг-мм-дд): ");
    } while (!isValidDate(date));

    string time;
    do {
        time = readNonEmptyString("Введите время (чч:мм): ");
    } while (!isValidTime(time));

    float temp = readFloatSafe("Введите температуру: ");
    return createRecord(loc, date, time, temp);
}

void printRecord(TemperatureRecord* node) {
    if (node) {
        cout << "Местность: " << node->location
             << ", Дата: " << node->date
             << ", Время: " << node->time
             << ", Температура: " << node->temperature << "°C\n";
    }
}

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

void addToStart(TemperatureRecord* newNode) {
    newNode->next = head;
    head = newNode;
}

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

void addAfter(const string& loc, TemperatureRecord* newNode) {
    TemperatureRecord* current = head;
    while (current && current->location != loc) {
        current = current->next;
    }
    if (!current) {
        cout << "Местность не найдена. Возврат в меню.\n";
        delete newNode;
        return;
    }
    newNode->next = current->next;
    current->next = newNode;
}

void addBefore(const string& loc, TemperatureRecord* newNode) {
    if (!head) {
        cout << "Местность не найдена. Возврат в меню.\n";
        delete newNode;
        return;
    }
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
        cout << "Местность не найдена. Возврат в меню.\n";
        delete newNode;
        return;
    }
    prev->next = newNode;
    newNode->next = current;
}

void deleteMinTemperature() {
    if (!head) {
        cout << "Список пуст.\n";
        return;
    }

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

void saveToFile(const string& filename) {
    ofstream out(filename, ios::binary);
    if (!out) {
        cout << "Ошибка при открытии файла для записи.\n";
        return;
    }
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

void loadFromFile(const string& filename) {
    ifstream in(filename, ios::binary);
    if (!in) {
        cout << "Ошибка открытия файла.\n";
        return;
    }

    while (head) {
        TemperatureRecord* temp = head;
        head = head->next;
        delete temp;
    }

    TemperatureRecord* tail = nullptr;

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

        TemperatureRecord* newNode = createRecord(location, date, time, temperature);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    in.close();
    cout << "Загрузка завершена.\n";
}

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

// 📌 Вынесенная логика для case 5
void handleAddAfter() {
    string location = readNonEmptyString("После какой местности вставить? ");
    TemperatureRecord* current = head;
    while (current && current->location != location) {
        current = current->next;
    }
    if (!current) {
        cout << "Местность не найдена. Возврат в меню.\n";
        return;
    }
    TemperatureRecord* newNode = inputRecord();
    addAfter(location, newNode);
}

// 📌 Вынесенная логика для case 6
void handleAddBefore() {
    string location = readNonEmptyString("Перед какой местности вставить? ");
    TemperatureRecord* current = head;
    while (current && current->location != location) {
        current = current->next;
    }
    if (!current) {
        cout << "Местность не найдена. Возврат в меню.\n";
        return;
    }
    TemperatureRecord* newNode = inputRecord();
    addBefore(location, newNode);
}

// 📌 Вынесенная логика для case 11
void searchByLocationAndDateRange() {
    string location = readNonEmptyString("Введите местность: ");
    string startDate, endDate;

    do {
        startDate = readNonEmptyString("Введите начальную дату (гггг-мм-дд): ");
    } while (!isValidDate(startDate));

    do {
        endDate = readNonEmptyString("Введите конечную дату (гггг-мм-дд): ");
    } while (!isValidDate(endDate));

    searchByLocationAndTime(location, startDate, endDate);
}

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

int main() {
    setlocale(LC_ALL, "ru");
    int choice;

    do {
        showMenu();
        choice = readIntSafe("Выбор: ");

        switch (choice) {
            case 1: printAll(); break;
            case 2: addToStart(inputRecord()); break;
            case 3: addToEnd(inputRecord()); break;
            case 4: addSortedByLocation(inputRecord()); break;
            case 5: handleAddAfter(); break;
            case 6: handleAddBefore(); break;
            case 7: deleteMinTemperature(); break;
            case 8: saveToFile("data.bin"); break;
            case 9: loadFromFile("data.bin"); break;
            case 10: showMaxMin(); break;
            case 11: searchByLocationAndDateRange(); break;
            case 0: cout << "Выход.\n"; break;
            default: cout << "Неверный выбор!\n"; break;
        }

    } while (choice != 0);

    while (head) {
        TemperatureRecord* temp = head;
        head = head->next;
        delete temp;
    }

    return 0;
}
