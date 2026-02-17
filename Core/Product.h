#pragma once
#include <string>

class Product //think of other children
{
public://Base class
	Product(int id, //Unique Product ID
		const std::string& name,    //Product name
		double price, //Product price, must be non-negative
		int quantity); //Initial stock quantity, must be non-negative

    virtual ~Product() = default;
    //getters
    int getId() const;
    std::string getName() const;
    double getPrice() const;
    int getQuantity() const;

	bool reduceStock(int amount); //Reduces the stock by the specified amount, returns true if successful, false if not enough stock or invalid amount

	virtual double calculateFinalPrice() const = 0; //Virtual method used in children classes to calculate the final price of the product, can be overridden for different pricing strategies aka DIscount and Tax

protected:
    int m_id;
    std::string m_name;
    double m_price;
    int m_quantity;
};