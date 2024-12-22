#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

// Базовый класс Товар
class Product {
public:
    Product(const string& name, double price, int quantity, int seller_id)
            : name(name), price(price), quantity(quantity), seller_id(seller_id) {}

    virtual ~Product() = default;

    const string& getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    int getSellerId() const { return seller_id; }

    void reduceQuantity(int amount) { quantity -= amount; }

protected:
    string name;
    double price;
    int quantity;
    int seller_id;
};

// Базовый класс Пользователь
class User {
public:
    User(const string& name, int id) : name(name), id(id) {}
    virtual ~User() = default;

    int getId() const { return id; }
    const string& getName() const { return name; }

protected:
    string name;
    int id;
};

// Класс Продавец
class Seller : public User {
public:
    Seller(const string& name, int id) : User(name, id) {}

    void addProduct(const shared_ptr<Product>& product) {
        products.push_back(product);
    }

private:
    vector<shared_ptr<Product>> products; // Список товаров продавца
};

// Базовый класс Стратегии оплаты
class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    virtual bool pay(double amount, double& balance) const = 0;
    virtual const string& getName() const = 0;
};

// Реализация оплаты наличными
class CashPayment : public PaymentStrategy {
public:
    bool pay(double amount, double& balance) const override {
        if (balance >= amount) {
            balance -= amount;
            return true;
        }
        return false;
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
            balance -= amount;
            return true;
        }
        return false;
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
            balance -= amount;
            return true;
        }
        return false;
    }

    const string& getName() const override { return name; }

private:
    string name = "CryptoPayment";
};

// Класс Покупатель
class Customer : public User {
public:
    Customer(const string& name, int id, double balance)
            : User(name, id), balance(balance) {}

    void setPaymentStrategy(const shared_ptr<PaymentStrategy>& strategy) {
        payment_strategy = strategy;
    }

    bool buyProduct(shared_ptr<Product>& product, int quantity) {
        if (!payment_strategy) {
            cout << "Payment method not set." << endl;
            return false;
        }

        double total_cost = product->getPrice() * quantity;
        if (product->getQuantity() >= quantity && payment_strategy->pay(total_cost, balance)) {
            product->reduceQuantity(quantity);
            cout << "Success: Bought " << product->getName() << " x" << quantity
                 << " for " << total_cost << " using " << payment_strategy->getName() << endl;
            cout << "Remaining balance: " << balance << endl;
            return true;
        }

        cout << "Purchase failed." << endl;
        return false;
    }

private:
    double balance;
    shared_ptr<PaymentStrategy> payment_strategy;
};

// Базовый класс Торговая площадка
class Marketplace {
public:
    virtual ~Marketplace() = default;

    void addSeller(const shared_ptr<Seller>& seller) {
        sellers.push_back(seller);
    }

    void addCustomer(const shared_ptr<Customer>& customer) {
        customers.push_back(customer);
    }

    void addProduct(const shared_ptr<Product>& product) {
        products.push_back(product);
    }

    void printProducts() const {
        cout << "Available products:" << endl;
        for (const auto& product : products) {
            cout << "- " << product->getName() << ", Price: " << product->getPrice()
                 << ", Quantity: " << product->getQuantity() << endl;
        }
    }

    shared_ptr<Product> findProduct(const string& name) const {
        for (const auto& product : products) {
            if (product->getName() == name) {
                return product;
            }
        }
        return nullptr;
    }

private:
    vector<shared_ptr<Seller>> sellers;
    vector<shared_ptr<Customer>> customers;
    vector<shared_ptr<Product>> products;
};

int main() {
    Marketplace marketplace;

    auto seller1 = make_shared<Seller>("John's Store", 1);
    marketplace.addSeller(seller1);

    auto product1 = make_shared<Product>("Laptop", 1000.0, 5, seller1->getId());
    marketplace.addProduct(product1);

    auto product2 = make_shared<Product>("Phone", 500.0, 10, seller1->getId());
    marketplace.addProduct(product2);

    auto customer1 = make_shared<Customer>("Alice", 1, 1500.0);
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