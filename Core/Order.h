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
	int orderId;    //Unique Order ID
	std::shared_ptr<Product> product;  //Product being ordered
	int quantity;  //Quantity of the product being ordered

public:
    Order(int id,
        std::shared_ptr<Product> product,
        int quantity);
    //Getters
    int getOrderId() const;
    int getProductId() const;
    std::string getProductName() const;
    int getQuantity() const;
    std::shared_ptr<Product> getProduct() const;
};
