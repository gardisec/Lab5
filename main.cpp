#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

// Класс описывает товар, который доступен на торговой площадке
class Product {
public:
    Product(const string& name, double price, int quantity, int seller_id)
            : name(name), price(price), quantity(quantity), seller_id(seller_id) {}

    const string& getName() const { return name; } // Получить название товара
    double getPrice() const { return price; } // Получить цену товара
    int getQuantity() const { return quantity; } // Получить количество товара
    int getSellerId() const { return seller_id; } // Получить идентификатор продавца

    void reduceQuantity(int amount) { quantity -= amount; } // Уменьшить количество товара на складе

private:
    string name; // Название товара
    double price; // Цена товара
    int quantity; // Количество товара на складе
    int seller_id; // Идентификатор продавца
};

// Класс описывает продавца на торговой площадке
class Seller {
public:
    Seller(const string& name, int id) : name(name), id(id) {}

    int getId() const { return id; } // Получить идентификатор продавца
    const string& getName() const { return name; } // Получить имя продавца

    void addProduct(const shared_ptr<Product>& product) {
        products.push_back(product); // Добавить товар в список продавца
    }

private:
    string name; // Имя продавца
    int id; // Уникальный идентификатор продавца
    vector<shared_ptr<Product>> products; // Список товаров продавца
};

// Абстрактный класс стратегии оплаты
class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    virtual bool pay(double amount, double& balance) const = 0; // Метод оплаты
};

// Реализация оплаты наличными
class CashPayment : public PaymentStrategy {
public:
    bool pay(double amount, double& balance) const override {
        if (balance >= amount) {
            balance -= amount; // Списать средства с баланса
            return true;
        }
        return false; // Недостаточно средств
    }
};

// Реализация оплаты картой
class CardPayment : public PaymentStrategy {
public:
    bool pay(double amount, double& balance) const override {
        if (balance >= amount) {
            balance -= amount; // Списать средства с баланса
            return true;
        }
        return false; // Недостаточно средств
    }
};

// Реализация оплаты криптовалютой
class CryptoPayment : public PaymentStrategy {
public:
    bool pay(double amount, double& balance) const override {
        if (balance >= amount) {
            balance -= amount; // Списать средства с баланса
            return true;
        }
        return false; // Недостаточно средств
    }
};

// Класс описывает покупателя
class Customer {
public:
    Customer(const string& name, double balance) : name(name), balance(balance) {}

    void setPaymentStrategy(const shared_ptr<PaymentStrategy>& strategy) {
        payment_strategy = strategy; // Установить метод оплаты
    }

    bool buyProduct(shared_ptr<Product>& product, int quantity) {
        if (!payment_strategy) {
            cout << "Payment method not set.\n";
            return false;
        }

        double total_cost = product->getPrice() * quantity; // Общая стоимость покупки
        if (product->getQuantity() >= quantity && payment_strategy->pay(total_cost, balance)) {
            product->reduceQuantity(quantity); // Уменьшить количество товара
            cout << "Purchase successful!\n";
            cout << "Receipt: " << product->getName() << " x" << quantity
                 << " for " << total_cost << " using chosen payment method.\n";
            cout << "Remaining balance: " << balance << "\n";
            return true;
        }
        cout << "Purchase failed.\n";
        return false; // Покупка не удалась
    }

private:
    string name; // Имя покупателя
    double balance; // Баланс покупателя
    shared_ptr<PaymentStrategy> payment_strategy; // Метод оплаты
};

// Класс управляет торговой площадкой
class Marketplace {
public:
    void addSeller(const shared_ptr<Seller>& seller) {
        sellers.push_back(seller); // Добавить продавца
    }

    void addCustomer(const shared_ptr<Customer>& customer) {
        customers.push_back(customer); // Добавить покупателя
    }

    void addProduct(const shared_ptr<Product>& product) {
        products.push_back(product); // Добавить товар
    }

    void listProducts() const {
        cout << "Available products:\n";
        for (const auto& product : products) {
            cout << "- " << product->getName() << ", Price: " << product->getPrice()
                 << ", Quantity: " << product->getQuantity() << "\n"; // Вывести информацию о товарах
        }
    }

    shared_ptr<Product> findProduct(const string& name) const {
        for (const auto& product : products) {
            if (product->getName() == name) {
                return product; // Найти товар по имени
            }
        }
        return nullptr; // Товар не найден
    }

private:
    vector<shared_ptr<Seller>> sellers; // Список продавцов
    vector<shared_ptr<Customer>> customers; // Список покупателей
    vector<shared_ptr<Product>> products; // Список товаров
};

int main() {
    Marketplace marketplace; // Создать торговую площадку

    auto seller1 = make_shared<Seller>("John's Store", 1); // Создать продавца
    marketplace.addSeller(seller1); // Добавить продавца на площадку

    auto product1 = make_shared<Product>("Laptop", 1000.0, 5, seller1->getId()); // Создать товар
    marketplace.addProduct(product1); // Добавить товар на площадку

    auto product2 = make_shared<Product>("Phone", 500.0, 10, seller1->getId()); // Создать товар
    marketplace.addProduct(product2); // Добавить товар на площадку

    auto customer1 = make_shared<Customer>("Alice", 1500.0); // Создать покупателя
    marketplace.addCustomer(customer1); // Добавить покупателя на площадку

    customer1->setPaymentStrategy(make_shared<CashPayment>()); // Установить метод оплаты

    marketplace.listProducts(); // Вывести список доступных товаров

    auto selectedProduct = marketplace.findProduct("Laptop"); // Найти товар по имени
    if (selectedProduct) {
        customer1->buyProduct(selectedProduct, 1); // Покупатель совершает покупку
    }

    marketplace.listProducts(); // Вывести список товаров после покупки

    return 0;
}
