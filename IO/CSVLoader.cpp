#include "CSVLoader.h"
#include "../Core/TaxableProduct.h"
#include "../Core/DiscountedProduct.h"
#include <fstream>
#include <sstream>

bool CSVLoader::load(const std::string& file,
    Inventory& inventory,
    std::string& error)
{
    std::ifstream in(file);

    if (!in.is_open())
    {
        error = "Inventory file not found!";
        return false;
    }

    std::string line;

    while (std::getline(in, line))
    {
        std::stringstream ss(line);
        std::string token;

        try
        {
            int id, qty;
            std::string name, type;
            double price, rate;

            std::getline(ss, token, ','); id = std::stoi(token);
            std::getline(ss, name, ',');
            std::getline(ss, token, ','); price = std::stod(token);
            std::getline(ss, token, ','); qty = std::stoi(token);
            std::getline(ss, type, ',');
            std::getline(ss, token, ','); rate = std::stod(token);

            if (type == "TAX")
            {
                inventory.addProduct(
                    std::make_shared<TaxableProduct>(
                        id, name, price, qty, rate));
            }
            else if (type == "DISC")
            {
                inventory.addProduct(
                    std::make_shared<DiscountedProduct>(
                        id, name, price, qty, rate));
            }
        }
        catch (...)
        {
            continue; // skip bad lines safely
        }
    }

    return true;
}
