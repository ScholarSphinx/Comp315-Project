#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>

class TextBox
{
public:
	TextBox() = default; //Default constructor for the TextBox class, allowing for the creation of an empty TextBox object that can be initialized later with specific font, size, and position parameters, providing flexibility in how the TextBox is set up and used in the application

	TextBox(const sf::Font& font, //Constructor for the TextBox class, taking a reference to an SFML font, a size for the text box, and a position for the text box, allowing for the initialization of the text box with specific visual properties and placement in the application, ensuring that the text box is properly set up with the desired font and appearance
        sf::Vector2f size,
        sf::Vector2f position)
    {
        m_box.setSize(size);
        m_box.setPosition(position);
        m_box.setFillColor({ 30,30,35 });
        m_box.setOutlineThickness(1.f);
        m_box.setOutlineColor({ 120,120,140 });

		m_text.emplace(font, "", 16); //Initializing the optional SFML Text object with the provided font, an empty string for the initial text, and a character size of 16, allowing for the text to be displayed in the text box with the specified font and size, ensuring that the text box is ready to display user input or other text content as needed in the application
        m_text->setPosition(position + sf::Vector2f{ 8.f, 6.f });
    }

	void handleEvent(const sf::Event& event) //Handling text input events for the TextBox, allowing the user to enter text into the text box and update the displayed text accordingly, ensuring that the TextBox can capture user input and reflect it visually in the application
    {
        if (event.is<sf::Event::TextEntered>())
        {
			auto unicode = event.getIf<sf::Event::TextEntered>()->unicode; //Retrieving the Unicode value of the entered character from the text input event, allowing us to determine which character was entered by the user and update the text box accordingly, ensuring that we can capture and process user input effectively for the TextBox

			if (unicode == 8 && !m_value.empty()) // Handle backspace: if the unicode value is 8 (backspace) and the current text value is not empty, we remove the last character from the text value, allowing the user to correct their input by deleting characters from the text box
                m_value.pop_back();
			else if (unicode >= 32 && unicode < 128) // Only allow printable ASCII characters
                m_value += static_cast<char>(unicode);

			if (m_text) m_text->setString(m_value); // Updating the displayed text in the TextBox to reflect the current value after handling the text input event, ensuring that the visual representation of the text box is consistent with the user's input and provides immediate feedback on the entered text
        }
    }

	const std::string& getValue() const { return m_value; } //Getter for the current value of the TextBox, allowing other parts of the application to retrieve the text that has been entered by the user, providing access to the text box's content for further processing or display elsewhere in the application

	void draw(sf::RenderWindow& window) //Drawing the TextBox on the provided SFML render window, allowing the text box to be visually represented in the application, ensuring that both the background box and the text (if it exists) are rendered correctly for the user to see and interact with
    {
        window.draw(m_box);
        if (m_text) window.draw(*m_text);
    }

private:
	sf::RectangleShape m_box;     //The background box of the TextBox, represented as an SFML RectangleShape, which provides the visual container for the text and can be styled with colors, outlines, and size to create the appearance of a text input field in the application
	std::optional<sf::Text> m_text; //An optional SFML Text object that holds the text to be displayed in the TextBox, allowing for the possibility that the text may not be initialized or displayed at all, providing flexibility in how the TextBox can be used and allowing for dynamic updates to the text content based on user input or other application logic
	std::string m_value; //A string to hold the current value of the TextBox, which is updated based on user input and can be retrieved using the getValue() method, ensuring that the TextBox maintains an internal representation of the text content that can be accessed and manipulated as needed in the application
};
