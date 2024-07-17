#include <iostream>
#include <vector>
#include <string>

class Product {
protected:
    std::string productName;
    double price;
    int stock;
public:
    Product(std::string name, double pr, int st) : productName(name), price(pr), stock(st) {}
    virtual void display() const = 0;
    virtual double calculateTotalPrice(int quantity) const {
        return price * quantity;
    }
    void reduceStock(int quantity) {
        stock -= quantity;
    }
    int getStock() const {
        return stock;
    }
    std::string getProductName() const {
        return productName;
    }
};

class PerishableProduct : public Product {
    int shelfLife; // in days
public:
    PerishableProduct(std::string name, double pr, int st, int sl) : Product(name, pr, st), shelfLife(sl) {}
    void display() const override {
        std::cout << "Perishable Product: " << productName << ", Price: $" << price << ", Stock: " << stock << ", Shelf Life: " << shelfLife << " days" << std::endl;
    }
};

class NonPerishableProduct : public Product {
    std::string brand;
public:
    NonPerishableProduct(std::string name, double pr, int st, std::string br) : Product(name, pr, st), brand(br) {}
    void display() const override {
        std::cout << "Non-Perishable Product: " << productName << ", Price: $" << price << ", Stock: " << stock << ", Brand: " << brand << std::endl;
    }
};

class Customer {
    std::string name;
    std::vector<Product*> cart;
    std::vector<int> quantities;
public:
    Customer(std::string n) : name(n) {}
    void addToCart(Product* p, int quantity) {
        if (p->getStock() >= quantity) {
            cart.push_back(p);
            quantities.push_back(quantity);
            p->reduceStock(quantity);
        } else {
            std::cout << "Insufficient stock for product: " << p->getProductName() << std::endl;
        }
    }
    void displayCart() const {
        std::cout << "Customer: " << name << "'s Cart:" << std::endl;
        for (size_t i = 0; i < cart.size(); ++i) {
            cart[i]->display();
            std::cout << "Quantity: " << quantities[i] << std::endl;
        }
    }
    double calculateTotalBill() const {
        double total = 0.0;
        for (size_t i = 0; i < cart.size(); ++i) {
            total += cart[i]->calculateTotalPrice(quantities[i]);
        }
        return total;
    }
    std::string getName() const {
        return name;
    }
};

class Bill {
    Customer* customer;
    double totalAmount;
public:
    Bill(Customer* cust) : customer(cust), totalAmount(0) {
        totalAmount = customer->calculateTotalBill();
    }
    void generateBill() const {
        std::cout << "Generating Bill for Customer: " << customer->getName() << std::endl;
        customer->displayCart();
        std::cout << "Total Amount: $" << totalAmount << std::endl;
    }
};

int main() {
    std::vector<Product*> products;
    products.push_back(new PerishableProduct("Milk", 2.5, 50, 7));
    products.push_back(new NonPerishableProduct("Cereal", 3.0, 30, "Kellogg's"));

    std::string customerName;
    std::cout << "Enter customer name: ";
    std::getline(std::cin, customerName);
    Customer customer1(customerName);

    char addMore;
    do {
        std::cout << "Available products:" << std::endl;
        for (size_t i = 0; i < products.size(); ++i) {
            std::cout << i + 1 << ". ";
            products[i]->display();
        }

        int productChoice, quantity;
        std::cout << "Enter product number to add to cart: ";
        std::cin >> productChoice;
        std::cout << "Enter quantity: ";
        std::cin >> quantity;

        if (productChoice > 0 && productChoice <= products.size()) {
            customer1.addToCart(products[productChoice - 1], quantity);
        } else {
            std::cout << "Invalid product choice!" << std::endl;
        }

        std::cout << "Do you want to add more products to cart? (y/n): ";
        std::cin >> addMore;
    } while (addMore == 'y' || addMore == 'Y');

    Bill bill(&customer1);
    bill.generateBill();

    // Clean up dynamically allocated memory
    for (Product* product : products) {
        delete product;
    }

    return 0;
}