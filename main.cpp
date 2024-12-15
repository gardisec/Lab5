#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

// Товар
class Product {
public:
    Product(const string& name, double price, int quantity, int seller_id)
            : name(name), price(price), quantity(quantity), seller_id(seller_id) {}

    const string& getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    int getSellerId() const { return seller_id; }

    void reduceQuantity(int amount) { quantity -= amount; }

private:
    string name;
    double price;
    int quantity;
    int seller_id;
};

// Продавец
class Seller {
public:
    Seller(const string& name, int id) : name(name), id(id) {}

    int getId() const { return id; }
    const string& getName() const { return name; }

    void addProduct(const shared_ptr<Product>& product) {
        products.push_back(product);
    }

private:
    string name;
    int id;
    vector<shared_ptr<Product>> products; // Список товаров продавца
};

// Абстрактный класс стратегии оплаты
class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    virtual bool pay(double amount, double& balance) const = 0; // Метод оплаты
    virtual const string& getName() const = 0; // Получить имя стратегии оплаты
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

    const string& getName() const override { return name; }

private:
    string name = "CashPayment";
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

    const string& getName() const override { return name; }

private:
    string name = "CardPayment";
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

    const string& getName() const override { return name; }

private:
    string name = "CryptoPayment";
};

// Покупатель
class Customer {
public:
    Customer(const string& name, double balance) : name(name), balance(balance) {}

    void setPaymentStrategy(const shared_ptr<PaymentStrategy>& strategy) {
        payment_strategy = strategy; // Установить метод оплаты
    }

    bool buyProduct(shared_ptr<Product>& product, int quantity) {
        if (!payment_strategy) {
            cout << "Payment method not set." << endl;
            return false;
        }

        double total_cost = product->getPrice() * quantity; // Общая стоимость покупки
        if (product->getQuantity() >= quantity && payment_strategy->pay(total_cost, balance)) {
            product->reduceQuantity(quantity); // Уменьшить количество товара
            cout << "Successful!" << endl;
            cout << "Buy: " << product->getName() << " x" << quantity
                 << " for " << total_cost << " by using " << payment_strategy->getName() << endl;
            cout << "Remaining balance: " << balance << endl;
            return true;
        }
        cout << "Purchase failed." << endl;
        return false; // Покупка не удалась
    }

private:
    string name;
    double balance;
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

    void printProducts() const { // Вывести информацию о товарах
        cout << "Available products:" << endl;
        for (const auto& product : products) {
            cout << "- " << product->getName() << ", Price: " << product->getPrice()
                 << ", Quantity: " << product->getQuantity() << endl;
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
    Marketplace marketplace;

    auto seller1 = make_shared<Seller>("John's Store", 1);
    marketplace.addSeller(seller1);

    auto product1 = make_shared<Product>("Laptop", 1000.0, 5, seller1->getId());
    marketplace.addProduct(product1);

    auto product2 = make_shared<Product>("Phone", 500.0, 10, seller1->getId());
    marketplace.addProduct(product2);

    auto customer1 = make_shared<Customer>("Alice", 1500.0);
    marketplace.addCustomer(customer1);

    customer1->setPaymentStrategy(make_shared<CashPayment>());

    marketplace.printProducts();

    auto selectedProduct = marketplace.findProduct("Laptop");
    if (selectedProduct) {
        customer1->buyProduct(selectedProduct, 1);
    }

    marketplace.printProducts();

    return 0;
}
