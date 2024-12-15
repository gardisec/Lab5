class Product:
    def __init__(self, name, price, quantity, seller_id):
        self.name = name
        self.price = price
        self.quantity = quantity
        self.seller_id = seller_id

    def reduce_quantity(self, amount):
        self.quantity -= amount


class Seller:
    def __init__(self, name, seller_id):
        self.name = name
        self.id = seller_id
        self.products = []

    def add_product(self, product):
        self.products.append(product)


class PaymentStrategy:
    def pay(self, amount, balance):
        raise NotImplementedError("PaymentStrategy.pay() must be implemented")

    def get_name(self):
        raise NotImplementedError("PaymentStrategy.get_name() must be implemented")


class CashPayment(PaymentStrategy):
    def pay(self, amount, balance):
        if balance >= amount:
            return True, balance - amount
        return False, balance

    def get_name(self):
        return "CashPayment"


class CardPayment(PaymentStrategy):
    def pay(self, amount, balance):
        if balance >= amount:
            return True, balance - amount
        return False, balance

    def get_name(self):
        return "CardPayment"


class CryptoPayment(PaymentStrategy):
    def pay(self, amount, balance):
        if balance >= amount:
            return True, balance - amount
        return False, balance

    def get_name(self):
        return "CryptoPayment"


class Customer:
    def __init__(self, name, balance):
        self.name = name
        self.balance = balance
        self.payment_strategy = None

    def set_payment_strategy(self, strategy):
        self.payment_strategy = strategy

    def buy_product(self, product, quantity):
        if not self.payment_strategy:
            print("Payment method not set.")
            return False

        total_cost = product.price * quantity
        if product.quantity >= quantity:
            success, new_balance = self.payment_strategy.pay(total_cost, self.balance)
            if success:
                product.reduce_quantity(quantity)
                self.balance = new_balance
                print(f"Successful purchase of {product.name} x{quantity} for {total_cost} using {self.payment_strategy.get_name()}.")
                print(f"Remaining balance: {self.balance}")
                return True
        print("Purchase failed.")
        return False


class Marketplace:
    def __init__(self):
        self.sellers = []
        self.products = []
        self.customers = []

    def add_seller(self, seller):
        self.sellers.append(seller)

    def add_customer(self, customer):
        self.customers.append(customer)

    def add_product(self, product):
        self.products.append(product)

    def list_products(self):
        print("Available products:")
        for product in self.products:
            print(f"- {product.name}, Price: {product.price}, Quantity: {product.quantity}")

    def find_product(self, name):
        for product in self.products:
            if product.name == name:
                return product
        return None


if __name__ == "__main__":
    marketplace = Marketplace()

    seller1 = Seller("John's Store", 1)
    marketplace.add_seller(seller1)

    product1 = Product("Laptop", 1000.0, 5, seller1.id)
    product2 = Product("Phone", 500.0, 10, seller1.id)

    marketplace.add_product(product1)
    marketplace.add_product(product2)

    customer1 = Customer("Alice", 1500.0)
    marketplace.add_customer(customer1)

    customer1.set_payment_strategy(CashPayment())

    marketplace.list_products()

    selected_product = marketplace.find_product("Laptop")
    if selected_product:
        customer1.buy_product(selected_product, 1)

    marketplace.list_products()
