#include "DiscountedProduct.h"

DiscountedProduct::DiscountedProduct(int id,
    const std::string& name,
    double price,
    int quantity,
    double discountRate)
    : Product(id, name, price, quantity),
    m_discountRate(discountRate < 0 ? 0 : discountRate)
{
}

double DiscountedProduct::calculateFinalPrice() const
{
    double result = m_price * (1.0 - m_discountRate);
    return result < 0 ? 0 : result;
}
