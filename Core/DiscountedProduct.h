#pragma once
#include "Product.h"

class DiscountedProduct : public Product
{
public:
    DiscountedProduct(int id,
        const std::string& name,
        double price,
        int quantity,
        double discountRate);

    double calculateFinalPrice() const override;

private:
    double m_discountRate;
};
