#include "Product.h"

Product::Product(int id,
                 const std::string& name,
                 double price,
                 int quantity)
    : m_id(id),
      m_name(name),
      m_price(price < 0 ? 0 : price),
      m_quantity(quantity < 0 ? 0 : quantity)
{
}

int Product::getId() const
{
    return m_id;
}

std::string Product::getName() const
{
    return m_name;
}

double Product::getPrice() const
{
    return m_price;
}

int Product::getQuantity() const
{
    return m_quantity;
}

bool Product::reduceStock(int amount)
{
    if (amount <= 0)
        return false;

    if (m_quantity < amount)
        return false;

    m_quantity -= amount;
    return true;
}
