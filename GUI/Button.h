#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

class Button {
public:
    Button() = default;

    Button(const sf::Font& font,
        const std::string& label,
        sf::Vector2f size,
        sf::Vector2f position);

    void setCallback(std::function<void()> cb);
    void handleEvent(const sf::Event& event,
        const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

private:
    sf::RectangleShape m_shape;
    std::optional<sf::Text> m_text;
    std::function<void()> m_callback;
};
