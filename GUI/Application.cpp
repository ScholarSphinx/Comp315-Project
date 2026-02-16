#include "Application.h"
#include "../IO/CSVLoader.h"
#include "../Config.h"

#include <SFML/Graphics.hpp>
#include <optional>
#include <thread>
#include <cmath>

Application::Application()
    : m_window(sf::VideoMode({ 1400u,800u }), "Inventory System")
{
    m_window.setFramerateLimit(60);

    sf::Image icon;
    if (icon.loadFromFile("Assets/icon.jpeg"))
        m_window.setIcon(icon);

    if (!m_font.openFromFile("Assets/font.ttf"))
        m_errorMessage = "Font failed to load.";

    std::string error;
    if (!CSVLoader::load("Assets/inventory.csv", m_inventory, error))
        m_errorMessage = error;

    m_processor = std::make_unique<OrderProcessor>(m_inventory);

    m_simulateBtn = Button(m_font, "Run Simulation", { 200,40 }, { 40,30 });
    m_simulateBtn.setCallback([this] {
        std::thread([this] {
            m_processor->simulate();
            }).detach();
        });
}

void Application::run()
{
    while (m_window.isOpen())
    {
        processEvents();
        render();
    }
}

void Application::processEvents()
{
    while (std::optional<sf::Event> e = m_window.pollEvent())
    {
        if (e->is<sf::Event::Closed>())
            m_window.close();

        else if (auto* t = e->getIf<sf::Event::TextEntered>())
        {
            if (t->unicode == 8 && !m_search.empty())
                m_search.pop_back();
            else if (t->unicode < 128)
                m_search += static_cast<char>(t->unicode);
        }

        m_simulateBtn.handleEvent(*e, m_window);
    }
}

void Application::render()
{
    m_window.clear({ 24,26,32 });

    if (!m_errorMessage.empty())
    {
        sf::Text error(m_font, m_errorMessage, 20);
        error.setPosition({ 40,40 });
        error.setFillColor(sf::Color::Red);
        m_window.draw(error);
        m_window.display();
        return;
    }

    m_simulateBtn.draw(m_window);

    sf::Text searchLabel(m_font, "Search: Products" + m_search, 18);
    searchLabel.setPosition({ 40,80 });
    m_window.draw(searchLabel);

    m_searchBox.draw(m_window);

    auto products = m_inventory.getProducts();
    float x = 40, y = 120;
    int col = 0;

    for (auto& [id, p] : products)
    {
        if (!m_search.empty() &&
            p->getName().find(m_search) == std::string::npos)
            continue;

        sf::RectangleShape card({ 220,70 });
        card.setPosition({ x + col * 240.f, y });
        card.setFillColor({ 36,38,45 });
        card.setOutlineThickness(1);
        card.setOutlineColor({ 70,70,80 });
        m_window.draw(card);


        sf::RectangleShape stockIndicator({ 30,70 });
        stockIndicator.setPosition({ x+190 + col * 240.f, y });
        stockIndicator.setFillColor({ 36,38,45 });
        stockIndicator.setOutlineThickness(2);
        stockIndicator.setOutlineColor({ 70,70,80 });
        
		int stockcount = p->getQuantity();

        if (stockcount == 0)
            stockIndicator.setFillColor({ 200,50,50 });
        else if (stockcount <= 10)
            stockIndicator.setFillColor({ 200,150,50 });
        else if (stockcount <= 20)
			stockIndicator.setFillColor({ 200,200,50 });
        else
            stockIndicator.setFillColor({ 50,200,50 });
        m_window.draw(stockIndicator);

        sf::Text t(m_font,
            p->getName() + "\nStock: " + std::to_string(p->getQuantity()), 16);
        t.setPosition({ x + 10 + col * 240.f, y + 10 });
        m_window.draw(t);

        col++;
        if (col == 4) { col = 0; y += 90; }
    }

    auto& metrics = m_processor->getMetrics();

    sf::Text stats(m_font,
        "Orders: " + std::to_string(metrics.total.load()) +
        "\nSuccess: " + std::to_string(metrics.success.load()) +
        "\nFailed: " + std::to_string(metrics.failed.load()) +
        "\nThreads: " + std::to_string(THREAD_COUNT),
        18);

    stats.setPosition({ 1000,120 });
    m_window.draw(stats);

    float total = (float)metrics.total.load();
    float ratio = total > 0 ? metrics.success.load() / total : 0.f;

    m_chart.draw(m_window, { 1100,400 }, 120.f, ratio);

    sf::Text chartLabel(m_font, "Success vs Failed", 18);
    chartLabel.setPosition({ 1030,520 });
    m_window.draw(chartLabel);

    m_window.display();
}
