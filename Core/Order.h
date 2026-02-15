#pragma once

enum class OrderStatus
{
    SUCCESS,
    FAILED
};

class Order
{
public:
    Order(int id, int productId, int quantity)
        : orderId(id),
        productId(productId),
        quantity(quantity),
        status(OrderStatus::FAILED) {
    }

    int orderId;
    int productId;
    int quantity;
    OrderStatus status;
};
