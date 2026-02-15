#pragma once
#include <string>

class Product
{
public:
    Product(int id,
        const std::string& name,
        double price,
        int quantity);

    virtual ~Product() = default;

    int getId() const;
    std::string getName() const;
    double getPrice() const;
    int getQuantity() const;

    bool reduceStock(int amount);

    virtual double calculateFinalPrice() const = 0;

protected:
    int m_id;
    std::string m_name;
    double m_price;
    int m_quantity;
};