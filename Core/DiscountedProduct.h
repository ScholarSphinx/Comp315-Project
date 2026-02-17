#pragma once
#include "Product.h"

class DiscountedProduct : public Product
{
public:
	DiscountedProduct(int id, //Unique Product ID
		const std::string& name, //Product name
		double price, //Product price, must be non-negative
		int quantity, //Initial stock quantity, must be non-negative
		double discountRate); //Discount rate as a percentage (e.g., 0.2 for 20% discount), must be between 0 and 1

	double calculateFinalPrice() const override; //Override the calculateFinalPrice method to apply the discount to the base price and return the final price, ensuring it does not go below 0

private:
    double m_discountRate; 
};
