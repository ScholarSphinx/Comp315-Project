#include "Inventory.h"

void Inventory::addProduct(std::shared_ptr<Product> product)
{
    if (!product) return;

    std::lock_guard<std::mutex> lock(m_mutex);
    m_products[product->getId()] = product;
}

std::map<int, std::shared_ptr<Product>> Inventory::getProducts() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_products;
}

bool Inventory::processOrder(Order& order)
{
    if (order.quantity <= 0)
    {
        order.status = OrderStatus::FAILED;
        return false;
    }

    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_products.find(order.productId);
    if (it == m_products.end())
    {
        order.status = OrderStatus::FAILED;
        return false;
    }

    bool success = it->second->reduceStock(order.quantity);

    order.status = success ? OrderStatus::SUCCESS
        : OrderStatus::FAILED;

    return success;
}
