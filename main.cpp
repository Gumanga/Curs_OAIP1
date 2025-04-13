#include <iostream>

using namespace std;

struct Node {
    string location;
    float temperature;
    Node* next;
};
enum Role { USER, ADMIN, EXIT };

Role login() {
    int choice;

    while (true) {
        cout << "\nВыберите роль:\n";
        cout << "1. Администратор\n";
        cout << "2. Пользователь\n";
        cout << "3. Выйти из программы\n";
        cout << "Ваш выбор: ";
        cin >> choice;

        switch (choice) {
            case 1: return ADMIN;
            case 2: return USER;
            case 3: return EXIT;
            default:
                cout << "Неверный выбор. Попробуйте снова.\n";
        }
    }
}

class TemperatureList {
private:
    Node* head;

public:
    TemperatureList() : head(nullptr) {}

    ~TemperatureList() {
        clear();
    }

void fillBelarusData(TemperatureList& list) {
    list.addEntry("Минск", 14.2);
    list.addEntry("Гродно", 13.8);
    list.addEntry("Брест", 14.5);
    list.addEntry("Гомель", 15.1);
    list.addEntry("Могилёв", 14.7);
    list.addEntry("Витебск", 13.2);
    list.addEntry("Барановичи", 14.4);
    list.addEntry("Полоцк", 13.5);
    list.addEntry("Орша", 14.0);
    list.addEntry("Лида", 13.9);

    cout << "Данные по городам Беларуси успешно добавлены.\n";
}

    // Добавить новую запись
    void addEntry(const string& location, float temp) {
        Node* newNode = new Node{location, temp, nullptr};
        if (!head) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next)
                current = current->next;
            current->next = newNode;
        }
    }

    // Удалить запись по местности
    void removeEntry(const string& location) {
        Node* current = head;
        Node* previous = nullptr;

        while (current) {
            if (current->location == location) {
                if (!previous)
                    head = current->next;
                else
                    previous->next = current->next;

                delete current;
                cout << "Запись удалена.\n";
                return;
            }
            previous = current;
            current = current->next;
        }

        cout << "Местность не найдена.\n";
    }

    // Редактировать температуру по местности
    void editEntry(const string& location, float newTemp) {
        Node* current = head;

        while (current) {
            if (current->location == location) {
                current->temperature = newTemp;
                cout << "Температура обновлена.\n";
                return;
            }
            current = current->next;
        }

        cout << "Местность не найдена.\n";
    }

    // Показать все записи
    void displayList() const {
        Node* current = head;
        if (!current) {
            cout << "Список пуст.\n";
            return;
        }

        cout << "Список температур:\n";
        while (current) {
            cout << "Местность: " << current->location
                 << " | Температура: " << current->temperature << "°C\n";
            current = current->next;
        }
    }

    // Средняя температура
    void averageTemperature() const {
        if (!head) {
            cout << "Список пуст.\n";
            return;
        }

        int count = 0;
        float sum = 0;
        Node* current = head;
        while (current) {
            sum += current->temperature;
            count++;
            current = current->next;
        }

        cout << "Средняя температура: " << sum / count << "°C\n";
    }

    // Максимум и минимум
    void maxMinTemperature() const {
        if (!head) {
            cout << "Список пуст.\n";
            return;
        }

        float maxTemp = head->temperature;
        float minTemp = head->temperature;
        Node* current = head->next;

        while (current) {
            if (current->temperature > maxTemp)
                maxTemp = current->temperature;
            if (current->temperature < minTemp)
                minTemp = current->temperature;
            current = current->next;
        }

        cout << "Макс: " << maxTemp << "°C, Мин: " << minTemp << "°C\n";
    }

    void clear() {
        Node* current = head;
        while (current) {
            Node* toDelete = current;
            current = current->next;
            delete toDelete;
        }
        head = nullptr;
    }
};
bool adminMenu(TemperatureList& list) {
    int choice;
    string location;
    float temp;

    while (true) {
        cout << "\n--- Меню администратора ---\n";
        cout << "1. Добавить запись\n";
        cout << "2. Удалить запись\n";
        cout << "3. Редактировать запись\n";
        cout << "4. Показать список\n";
        cout << "5. Автоматически заполнить города Беларуси\n";
        cout << "6. Вернуться к выбору роли\n";
        cout << "7. Завершить программу\n";
        cout << "Выбор: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Введите местность: ";
                cin >> location;
                cout << "Введите температуру: ";
                cin >> temp;
                list.addEntry(location, temp);
                break;
            case 2:
                list.displayList();
                cout << "Введите местность для удаления: ";
                cin >> location;
                list.removeEntry(location);
                break;
            case 3:
                cout << "Введите местность: ";
                cin >> location;
                cout << "Введите новую температуру: ";
                cin >> temp;
                list.editEntry(location, temp);
                break;
            case 4:
                list.displayList();
                break;
            case 5:
                list.fillBelarusData(list);
                break;
            case 6:
                return true;  // Вернуться к login()
            case 7:
                return false; // Завершить программу
            default:
                cout << "Неверный выбор.\n";
        }
    }
}


bool userMenu(TemperatureList& list) {
    int choice;

    while (true) {
        cout << "\n--- Меню пользователя ---\n";
        cout << "1. Показать список\n";
        cout << "2. Средняя температура\n";
        cout << "3. Макс/Мин температура\n";
        cout << "4. Вернуться к выбору роли\n";
        cout << "5. Завершить программу\n";
        cout << "Выбор: ";
        cin >> choice;

        switch (choice) {
            case 1:
                list.displayList();
                break;
            case 2:
                list.averageTemperature();
                break;
            case 3:
                list.maxMinTemperature();
                break;
            case 4:
                return true;  // к login
            case 5:
                return false; // завершить main
            default:
                cout << "Неверный выбор.\n";
        }
    }
}

int main() {
    TemperatureList list;

    while (true) {
        Role role = login();

        if (role == EXIT) {
            cout << "Программа завершена.\n";
            break;
        }

        if (role == ADMIN) {
            if (!adminMenu(list)) break;
        } else {
            if (!userMenu(list)) break;
        }
    }

    return 0;
}
