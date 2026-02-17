#include "TaxableProduct.h"

TaxableProduct::TaxableProduct(int id, //Unique Product ID
	const std::string& name, //Product name
	double price, //Product price, must be non-negative
	int quantity, //Initial stock quantity, must be non-negative
	double taxRate) //Tax rate as a percentage (e.g., 0.15 for 15% tax), must be non-negative
    : Product(id, name, price, quantity),
	m_taxRate(taxRate < 0 ? 0 : taxRate) //Ensuring the tax rate is not negative, if it is, set it to 0
{
}

double TaxableProduct::calculateFinalPrice() const //Calculates the final price after applying the tax
{
    return m_price * (1.0 + m_taxRate);
}
