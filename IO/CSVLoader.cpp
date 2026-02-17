#include "CSVLoader.h"
#include "../Core/TaxableProduct.h"
#include "../Core/DiscountedProduct.h"
#include <fstream>
#include <sstream>

bool CSVLoader::load(const std::string& file, //Loading product data from a CSV file into the inventory, parsing each line of the file to create product objects and add them to the inventory, while handling potential errors in file access and data parsing, ensuring that the inventory is populated with valid products based on the contents of the CSV file
    Inventory& inventory,
    std::string& error)
{
	std::ifstream in(file); //Opening the specified CSV file for reading, using an ifstream to handle file input operations, ensuring that we can read the contents of the file to load product data into the inventory

	if (!in.is_open()) //Checking if the file was successfully opened, if not, we set an error message indicating that the inventory file was not found and return false, ensuring that we handle file access errors gracefully and provide feedback on the issue
    {
        error = "Inventory file not found!";
        return false;
    }

    std::string line;

	while (std::getline(in, line)) //Reading the file line by line, using std::getline to read each line of the CSV file into a string variable, allowing us to process each line of product data individually, ensuring that we can parse and load each product from the file into the inventory
    {
        std::stringstream ss(line);
		std::string token; //Using a stringstream to parse the line of CSV data, allowing us to extract individual fields from the line based on the comma delimiter, ensuring that we can correctly parse the product data from the CSV format and create product objects accordingly

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
			// Parsing the fields from the CSV line, extracting the product ID, name, price, quantity, type, and rate from the line of data, using std::getline to read each field and std::stoi and std::stod to convert the numeric fields to the appropriate types, ensuring that we correctly parse the product data from the CSV format and create product objects accordingly
            if (type == "TAX")
			{// If the product type is "TAX", we create a TaxableProduct object and add it to the inventory, using std::make_shared to create a shared pointer to the product, allowing for polymorphism and proper memory management, ensuring that the product is correctly added to the inventory based on its type
                inventory.addProduct(
                    std::make_shared<TaxableProduct>(
                        id, name, price, qty, rate));
            }
            else if (type == "DISC")
			{// If the product type is "DISC", we create a DiscountedProduct object and add it to the inventory, using std::make_shared to create a shared pointer to the product, allowing for polymorphism and proper memory management, ensuring that the product is correctly added to the inventory based on its type
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

	return true; //Returning true to indicate that the products were successfully loaded into the inventory, allowing the caller to know that the loading process was completed without critical errors, while any issues with individual lines of data are handled gracefully by skipping those lines and continuing with the rest of the file
}
