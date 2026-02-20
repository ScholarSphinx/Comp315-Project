#include "Order.h"
#include <stdexcept>

Order::Order(int id,
    std::shared_ptr<Product> product,
    int quantity)
    : orderId(id), product(product), quantity(quantity)
{//Creating the object and validating the input parameters
    if (!product)
        throw std::invalid_argument("Order created with null product.");

    if (quantity <= 0)
        throw std::invalid_argument("Order quantity must be positive.");
}//throws for errors in the input parameters

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
void Inventory::removeProduct(int productId)
{
    std::lock_guard<std::mutex> lock(m_inventoryMutex);
    m_products.erase(productId);
}

void Inventory::increaseStock(int productId, int amount)
{
    std::lock_guard<std::mutex> lock(m_inventoryMutex);

    auto it = m_products.find(productId);
    if (it != m_products.end())
        it->second->increaseStock(amount);
}
