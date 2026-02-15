#pragma once
#include <memory>
#include <string>
#include "Product.h"

enum class OrderStatus
{
    Success,
    InsufficientStock,
    ProductNotFound,
    InvalidQuantity
};

class Order
{
private:
    int orderId;
    std::shared_ptr<Product> product;
    int quantity;

public:
    Order(int id,
        std::shared_ptr<Product> product,
        int quantity);

    int getOrderId() const;
    int getProductId() const;
    std::string getProductName() const;
    int getQuantity() const;
    std::shared_ptr<Product> getProduct() const;
};
