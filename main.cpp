#include <iostream>

using namespace std;

struct Node {
    string location;
    float temperature;
    Node* next;
};
enum Role { USER, ADMIN };

Role login() {
    int choice;
    cout << "Выберите роль:\n";
    cout << "1. Администратор\n";
    cout << "2. Пользователь\n";
    cout << "Ваш выбор: ";
    cin >> choice;

    if (choice == 1) return ADMIN;
    else return USER;
}
class TemperatureList {
private:
    Node* head;

public:
    TemperatureList() : head(nullptr) {}

    ~TemperatureList() {
        clear();
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
void adminMenu(TemperatureList& list) {
    int choice;
    string location;
    float temp;

    do {
        cout << "\n--- Меню администратора ---\n";
        cout << "1. Добавить запись\n";
        cout << "2. Удалить запись\n";
        cout << "3. Редактировать запись\n";
        cout << "4. Показать список\n";
        cout << "5. Выйти\n";
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
                cout << "Выход.\n";
                break;
            default:
                cout << "Неверный выбор.\n";
        }

    } while (choice != 5);
}

void userMenu(TemperatureList& list) {
    int choice;
    do {
        cout << "\n--- Меню пользователя ---\n";
        cout << "1. Показать список\n";
        cout << "2. Средняя температура\n";
        cout << "3. Макс/Мин температура\n";
        cout << "4. Выйти\n";
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
                cout << "Выход.\n";
                break;
            default:
                cout << "Неверный выбор.\n";
        }

    } while (choice != 4);
}
int main() {
    TemperatureList list;

    while (true) {
        Role role = login();

        if (role == ADMIN)
            adminMenu(list);
        else
            userMenu(list);

        char again;
        cout << "\nХотите вернуться к выбору роли? (y/n): ";
        cin >> again;
        if (again != 'y' && again != 'Y') {
            cout << "Завершение программы.\n";
            break;
        }
    }

    return 0;
}
