#include "TaxableProduct.h"

TaxableProduct::TaxableProduct(int id,
    const std::string& name,
    double price,
    int quantity,
    double taxRate)
    : Product(id, name, price, quantity),
    m_taxRate(taxRate < 0 ? 0 : taxRate)
{
}

double TaxableProduct::calculateFinalPrice() const
{
    return m_price * (1.0 + m_taxRate);
}
