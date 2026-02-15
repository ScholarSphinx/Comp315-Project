#include "Button.h"

Button::Button(const sf::Font& font,
    const std::string& label,
    sf::Vector2f size,
    sf::Vector2f position)
{
    m_shape.setSize(size);
    m_shape.setPosition(position);
    m_shape.setFillColor({ 60,60,60 });
    m_shape.setOutlineThickness(1.f);
    m_shape.setOutlineColor({ 120,120,140 });

    m_text.emplace(font, label, 16); 
    m_text->setPosition(position + sf::Vector2f{ 10.f, 8.f });
}

void Button::setCallback(std::function<void()> cb) {
    m_callback = cb;
}

void Button::handleEvent(const sf::Event& event,
    const sf::RenderWindow& window)
{
    if (event.is<sf::Event::MouseButtonPressed>())
    {
        auto mouse = sf::Mouse::getPosition(window);
        if (m_shape.getGlobalBounds().contains(
            sf::Vector2f{ (float)mouse.x, (float)mouse.y }))
        {
            if (m_callback) m_callback();
        }
    }
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(m_shape);
    if (m_text) window.draw(*m_text);
}
