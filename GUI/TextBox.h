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

	void handleEvent(const sf::Event& event, const sf::RenderWindow& window) //Handling events for the TextBox, including mouse clicks for focus and text input, allowing the user to interact with the text box and enter text when it is focused, ensuring that the TextBox can capture user input and reflect it visually in the application
    {
        if (event.is<sf::Event::MouseButtonPressed>()) //Checking if the event is a mouse button press, allowing us to determine if the user clicked on the text box to focus it, ensuring that the text box only captures text input when it is focused by the user
        {
            auto mouse = sf::Mouse::getPosition(window); //Getting the current position of the mouse cursor relative to the provided render window, allowing us to determine where the user clicked in relation to the text box's position and size, ensuring that we can check if the click occurred within the bounds of the text box to focus it
            if (m_box.getGlobalBounds().contains(sf::Vector2f{ (float)mouse.x, (float)mouse.y })) //Checking if the mouse click position is within the bounds of the text box, allowing us to determine if the user clicked on the text box to focus it, ensuring that we only focus the text box when the click occurred within its area, enabling accurate and responsive interaction with the text box in the application
                m_focused = true;
            else
                m_focused = false; //Unfocusing the text box if the click occurred outside its bounds, ensuring that the text box only captures text input when it is focused, providing a better user experience by preventing accidental text input when interacting with other elements of the interface
        }

        if (m_focused && event.is<sf::Event::TextEntered>()) //Only processing text input events when the text box is focused, ensuring that the text box only captures text input when it is actively being used by the user, preventing accidental text input when interacting with other elements of the interface
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
        // Update outline color based on focus state to provide visual feedback
        if (m_focused)
            m_box.setOutlineColor({ 100, 150, 255 }); //Setting the outline color to blue when the text box is focused, providing visual feedback to the user that the text box is active and ready to receive input, enhancing the user experience by making it clear which element is currently being interacted with
        else
            m_box.setOutlineColor({ 120,120,140 }); //Setting the outline color to gray when the text box is not focused, providing visual feedback to the user that the text box is inactive, ensuring that the text box's appearance reflects its current state and helps users understand which elements are active or inactive in the interface

        window.draw(m_box);
        if (m_text) window.draw(*m_text);
    }

private:
	sf::RectangleShape m_box;     //The background box of the TextBox, represented as an SFML RectangleShape, which provides the visual container for the text and can be styled with colors, outlines, and size to create the appearance of a text input field in the application
	std::optional<sf::Text> m_text; //An optional SFML Text object that holds the text to be displayed in the TextBox, allowing for the possibility that the text may not be initialized or displayed at all, providing flexibility in how the TextBox can be used and allowing for dynamic updates to the text content based on user input or other application logic
	std::string m_value; //A string to hold the current value of the TextBox, which is updated based on user input and can be retrieved using the getValue() method, ensuring that the TextBox maintains an internal representation of the text content that can be accessed and manipulated as needed in the application
	bool m_focused = false; //A boolean flag to track whether the TextBox is currently focused, allowing us to determine when the text box should capture text input events, ensuring that the text box only processes input when it is actively being used by the user, providing a better user experience by preventing accidental text input when interacting with other elements of the interface
};
