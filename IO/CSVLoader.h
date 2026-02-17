#pragma once
#include "../Core/Inventory.h"
#include <string>

class CSVLoader
{
public:
	static bool load(const std::string& file, //Loading product data from a CSV file into the inventory, parsing each line of the file to create product objects and add them to the inventory, while handling potential errors in file access and data parsing, ensuring that the inventory is populated with valid products based on the contents of the CSV file
        Inventory& inventory,
        std::string& error);
};
