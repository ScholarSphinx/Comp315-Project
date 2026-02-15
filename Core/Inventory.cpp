#include "Inventory.h"

void Inventory::addProduct(std::shared_ptr<Product> product)
{
    if (!product)
        return;

    std::lock_guard<std::mutex> lock(m_inventoryMutex);
    m_products[product->getId()] = product;
}

const std::unordered_map<int, std::shared_ptr<Product>>&
Inventory::getProducts() const
{
    return m_products;
}

OrderStatus Inventory::processOrder(int productId,
    int quantity,
    double& revenueOut)
{
    std::lock_guard<std::mutex> lock(m_inventoryMutex);

    if (quantity <= 0)
        return OrderStatus::InvalidQuantity;

    auto it = m_products.find(productId);
    if (it == m_products.end())
        return OrderStatus::ProductNotFound;

    auto& product = it->second;

    if (product->getQuantity() < quantity)
        return OrderStatus::InsufficientStock;

    product->reduceStock(quantity);

    revenueOut =
        product->calculateFinalPrice() * quantity;

    return OrderStatus::Success;
}
