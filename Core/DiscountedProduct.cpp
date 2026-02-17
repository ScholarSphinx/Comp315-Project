#include "DiscountedProduct.h"

DiscountedProduct::DiscountedProduct(int id, //Unique Product ID
	const std::string& name, //Product name
	double price, //Product price, must be non-negative
	int quantity, //Initial stock quantity, must be non-negative
	double discountRate) //Discount rate as a percentage (e.g., 0.2 for 20% discount), must be between 0 and 1
    : Product(id, name, price, quantity),
	m_discountRate(discountRate < 0 ? 0 : discountRate) //Ensuring the discount rate is not negative, if it is, set it to 0
{
}

double DiscountedProduct::calculateFinalPrice() const //Calculates the final price after applying the discount, ensuring it does not go below 0
{
	double result = m_price * (1.0 - m_discountRate); // Calculate the discounted price
    return result < 0 ? 0 : result;
}
