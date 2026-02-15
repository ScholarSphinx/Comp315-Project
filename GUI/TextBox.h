#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>

class TextBox
{
public:
    TextBox() = default;

    TextBox(const sf::Font& font,
        sf::Vector2f size,
        sf::Vector2f position)
    {
        m_box.setSize(size);
        m_box.setPosition(position);
        m_box.setFillColor({ 30,30,35 });
        m_box.setOutlineThickness(1.f);
        m_box.setOutlineColor({ 120,120,140 });

        m_text.emplace(font, "", 16);
        m_text->setPosition(position + sf::Vector2f{ 8.f, 6.f });
    }

    void handleEvent(const sf::Event& event)
    {
        if (event.is<sf::Event::TextEntered>())
        {
            auto unicode = event.getIf<sf::Event::TextEntered>()->unicode;

            if (unicode == 8 && !m_value.empty())
                m_value.pop_back();
            else if (unicode >= 32 && unicode < 128)
                m_value += static_cast<char>(unicode);

            if (m_text) m_text->setString(m_value);
        }
    }

    const std::string& getValue() const { return m_value; }

    void draw(sf::RenderWindow& window)
    {
        window.draw(m_box);
        if (m_text) window.draw(*m_text);
    }

private:
    sf::RectangleShape m_box;
    std::optional<sf::Text> m_text;
    std::string m_value;
};
