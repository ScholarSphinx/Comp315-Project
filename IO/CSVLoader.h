#pragma once
#include "../Core/Inventory.h"
#include <string>

class CSVLoader
{
public:
    static bool load(const std::string& file,
        Inventory& inventory,
        std::string& error);
};
