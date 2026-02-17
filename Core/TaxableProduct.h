#pragma once
#include "Product.h"

class TaxableProduct : public Product
{
public:
	TaxableProduct(int id, //Unique Product ID
		const std::string& name, //Product name
		double price, //Product price, must be non-negative
		int quantity, //Initial stock quantity, must be non-negative
		double taxRate); //Tax rate as a percentage (e.g., 0.15 for 15% tax), must be non-negative

	double calculateFinalPrice() const override; //Override the calculateFinalPrice method to apply the tax to the base price and return the final price

private:
    double m_taxRate;
};
