#pragma once
#include <string>

class Product //think of other children
{
public://Base class
	Product(int id, //Unique Product ID
		const std::string& name,    //Product name
		double price, //Product price, must be non-negative
		int quantity,
        bool isActive = true); //Initial stock quantity, must be non-negative

    virtual ~Product() = default;
    //getters
    bool isActive() const;
    void setActive(bool status);
    int getId() const;
    std::string getName() const;
    double getPrice() const;
    int getQuantity() const;
	void increaseStock(int amount); //Increases the stock by the specified amount, ensuring that the stock quantity is updated correctly when new inventory is added or when correcting stock levels, allowing for proper inventory management and accurate tracking of available products
	bool reduceStock(int amount); //Reduces the stock by the specified amount, returns true if successful, false if not enough stock or invalid amount

	virtual double calculateFinalPrice() const = 0; //Virtual method used in children classes to calculate the final price of the product, can be overridden for different pricing strategies aka DIscount and Tax

protected:
    int m_id;
    std::string m_name;
    double m_price;
    int m_quantity;
    bool m_isActive; // The "0 or 1" flag for the system
};