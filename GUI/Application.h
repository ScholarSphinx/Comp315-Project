#pragma once
#include <SFML/Graphics.hpp>
#include "../Core/Inventory.h"
#include "../Concurrency/OrderProcessor.h"
#include "../GUI/Button.h"
#include "../GUI/PieChart.h"
#include "../GUI/TextBox.h"

class Application
{
public:
    Application();
    void run();

private:
    void processEvents();
    void render();

    sf::RenderWindow m_window;
    sf::Font m_font;

    Inventory m_inventory;
    std::unique_ptr<OrderProcessor> m_processor;

    Button m_simulateBtn;
    TextBox m_searchBox;
    PieChart m_chart;

    std::string m_search;
    std::string m_errorMessage;
};
