#include "Order.h"
#include <stdexcept>

Order::Order(int id,
    std::shared_ptr<Product> product,
    int quantity)
    : orderId(id), product(product), quantity(quantity)
{
    if (!product)
        throw std::invalid_argument("Order created with null product.");

    if (quantity <= 0)
        throw std::invalid_argument("Order quantity must be positive.");
}

int Order::getOrderId() const
{
    return orderId;
}

int Order::getProductId() const
{
    return product->getId();
}

std::string Order::getProductName() const
{
    return product->getName();
}

int Order::getQuantity() const
{
    return quantity;
}

std::shared_ptr<Product> Order::getProduct() const
{
    return product;
}
