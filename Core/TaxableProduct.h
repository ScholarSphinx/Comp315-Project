#pragma once
#include "Product.h"

class TaxableProduct : public Product
{
public:
    TaxableProduct(int id,
        const std::string& name,
        double price,
        int quantity,
        double taxRate);

    double calculateFinalPrice() const override;

private:
    double m_taxRate;
};
