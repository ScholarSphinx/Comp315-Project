#pragma once
#include <map>
#include <memory>
#include <mutex>
#include "Product.h"
#include "Order.h"

class Inventory
{
public:
    void addProduct(std::shared_ptr<Product> product);

    std::map<int, std::shared_ptr<Product>> getProducts() const;

    bool processOrder(Order& order);

private:
    std::map<int, std::shared_ptr<Product>> m_products;
    mutable std::mutex m_mutex;
};
