#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
using namespace std;

class Sushi {
private:
    string name; // Название суши
    double price; // Цена суши
public:
    Sushi(string n, double p) : name(n), price(p) {}

    string getName() const { // Метод для получения названия суши
        return name;
    }

    double getPrice() const { // Метод для получения цены суши
        return price;
    }
};

class OrderItem {
private:
    Sushi sushi; // Суши
    int quantity; // Количество
public:
    OrderItem(const Sushi& s, int q) : sushi(s), quantity(q) {}

    Sushi getSushi() const { // Метод для получения суши
        return sushi;
    }

    int getQuantity() const { // Метод для получения количества
        return quantity;
    }
};

class Order {
private:
    vector<OrderItem> items; // Вектор для хранения позиций в заказе
public:
    void addItem(const Sushi& sushi, int quantity) { // Метод для добавления позиции в заказ
        if (quantity > 0) {
            items.push_back(OrderItem(sushi, quantity));
        }
    }

    void removeItem(int index) { // Метод для удаления позиции из заказа
        if (index >= 0 && index < static_cast<int>(items.size())) {
            items.erase(items.begin() + index);
        }
    }

    double getTotalPrice() const { // Метод для получения общей стоимости заказа
        double total = 0.0;
        for (const auto& item : items) {
            total += item.getSushi().getPrice() * item.getQuantity();
        }
        return total;
    }

    void displayOrder() const { // Метод для отображения заказа
        cout << "Заказ: \n";
        if (items.empty()) {
            cout << "Нет выбранных суши\n";
        }
        else {
            for (size_t i = 0; i < items.size(); ++i) {
                cout << i + 1 << ". " << items[i].getSushi().getName() << " x " << items[i].getQuantity()
                    << " - " << items[i].getSushi().getPrice() << " гривен\n";
            }
            cout << "Общая стоимость: " << getTotalPrice() << " гривен\n";
        }
    }
};

class DeliveryInfo {
private:
    string address; // Адрес доставки
    int deliveryTime; // Время доставки
    string deliveryMethod; // Метод доставки
public:
    DeliveryInfo(string addr, int time, string method) : address(addr), deliveryTime(time), deliveryMethod(method) {}

    string getAddress() const { // Метод для получения адреса доставки
        return address;
    }

    void setAddress(const string& addr) { // Метод для установки адреса доставки
        address = addr;
    }

    int getDeliveryTime() const { // Метод для получения времени доставки
        return deliveryTime;
    }

    string getDeliveryMethod() const { // Метод для получения метода доставки
        return deliveryMethod;
    }
};

// Функция для отображения меню и выбора суши
void showMenu(const vector<Sushi>& menu, Order& order, double& discount) {
    cout << "Меню:\n";
    for (size_t i = 0; i < menu.size(); ++i) {
        cout << i + 1 << ". " << menu[i].getName() << " - " << menu[i].getPrice() << " гривен\n";
    }

    int choice;
    cout << "Выберите суши для добавления в заказ (введите номер, 0 - завершить выбор): ";
    cin >> choice;
    while (choice != 0) {
        if (choice > 0 && choice <= static_cast<int>(menu.size())) {
            int quantity;
            cout << "Введите количество порций: ";
            cin >> quantity;
            order.addItem(menu[choice - 1], quantity);
        }
        else {
            cout << "Некорректный выбор. Пожалуйста, введите номер из меню.\n";
        }
        cout << "Продолжить выбор (0 - завершить выбор): ";
        cin >> choice;
    }

    // Проверяем промокод
    string promoCode;
    cout << "Введите промокод (если у вас есть): ";
    cin >> promoCode;
    if (promoCode == "SUSHI24") {
        discount = 0.25; // 25% скидка
        cout << "Промокод принят. Ваша скидка: 25%\n";
    }
    else {
        cout << "Неверный промокод.\n";
    }
}

// Функция для удаления позиций из заказа с подтверждением
void removeItemsWithConfirmation(Order& order) {
    char confirm;
    do {
        int index;
        cout << "Введите номер суши для удаления из заказа (или 0 для отмены): ";
        cin >> index;
        if (index != 0) {
            order.removeItem(index - 1);
            cout << "Суши удалены из заказа.\n";
        }
        else {
            break;
        }
        cout << "Хотите удалить еще суши из заказа? (y/n): ";
        cin >> confirm;
    } while (confirm == 'y' || confirm == 'Y');
}

// Функция для размещения заказа
void placeOrder(const vector<Sushi>& menu, Order& order, DeliveryInfo& deliveryInfo, double discount) {
    showMenu(menu, order, discount);

    order.displayOrder();

    removeItemsWithConfirmation(order);

    order.displayOrder();

    // Запрос адреса доставки
    if (deliveryInfo.getDeliveryMethod() == "курьерская доставка") {
        string address;
        cout << "Введите адрес доставки: ";
        cin.ignore();
        getline(cin, address);
        deliveryInfo.setAddress(address);
    }

    srand(time(0)); // Инициализируем генератор случайных чисел
    int randomWaitTime = rand() % 91 + 30; // Генерируем случайное число от 30 до 120
    double totalPrice = order.getTotalPrice() * (1 - discount); // Учитываем скидку
    cout << "Общая стоимость заказа с учетом скидки: " << totalPrice << " гривен\n";
    cout << "Ожидаемое время доставки: " << randomWaitTime << " минут\n";
}

int main() {
    setlocale(LC_ALL, "Russian");

    vector<Sushi> menu = {
        Sushi("Ролл с лососем", 250.0),
        Sushi("Унаги ролл", 350.0),
        Sushi("Филадельфия", 300.0)
    };

    Order order; // Создание объекта заказа
    DeliveryInfo deliveryInfo("", 0, ""); // Создание объекта информации о доставке
    double discount = 0.0; // Изначально скидка отсутствует

    int choice;
    do {
        cout << "\nМеню выбора действий:\n";
        cout << "1. Заказать суши с курьерской доставкой\n";
        cout << "2. Заказать суши с самовывозом\n";
        cout << "3. Выйти\n";
        cout << "Выберите действие: ";
        cin >> choice;

        switch (choice) {
        case 1: // Выбор курьерской доставки
            deliveryInfo = DeliveryInfo("", 0, "курьерская доставка");
            placeOrder(menu, order, deliveryInfo, discount);
            break;
        case 2: // Выбор самовывоза
            deliveryInfo = DeliveryInfo("", 0, "самовывоз");
            placeOrder(menu, order, deliveryInfo, discount);
            break;
        case 3: // Выход из программы
            cout << "Выход из программы.\n";
            break;
        default: // Обработка некорректного ввода
            cout << "Некорректный выбор. Пожалуйста, выберите снова.\n";
            break;
        }
    } while (choice != 3);

    return 0;
}

// Извиняюсь за такой маленький проект просто идей не было какой еще функционал можно было добавить