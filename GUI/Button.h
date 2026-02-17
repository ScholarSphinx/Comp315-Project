#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

class Button {
public:
	Button() = default; //Default constructor for the Button class, allowing for the creation of an uninitialized button object that can be configured later, providing flexibility in how and when the button is set up in the application

	Button(const sf::Font& font, //Constructor for the Button class, taking a reference to a font, a label string, size, and position as parameters, allowing for the initialization of the button's visual appearance and text label, ensuring that the button is properly configured with the desired font, label, size, and position when it is created in the application
        const std::string& label,
        sf::Vector2f size,
        sf::Vector2f position);

	void setCallback(std::function<void()> cb); //Setting a callback function for the button, allowing users to define custom behavior that will be executed when the button is clicked, enabling interactive functionality for the button in the application
	void handleEvent(const sf::Event& event, //Handling events for the button, allowing it to respond to user interactions such as mouse clicks, ensuring that the button can trigger its callback function when the appropriate events occur, enabling interactive behavior for the button in the application
        const sf::RenderWindow& window); 
	void draw(sf::RenderWindow& window); //Drawing the button on the provided render window, allowing it to be visually represented in the application, ensuring that the button's shape and text are rendered correctly on the screen, enabling users to see and interact with the button in the application

private:
	sf::RectangleShape m_shape; //Using a RectangleShape to represent the visual appearance of the button, allowing for easy customization of the button's size, color, and position, ensuring that the button can be visually distinct and appropriately styled in the application
	std::optional<sf::Text> m_text; //Using an optional Text object to represent the label of the button allows for flexibility in whether the button has a label or not, enabling the creation of both labeled and unlabeled buttons in the application, while still providing the ability to display text on the button when needed
	std::function<void()> m_callback; //Using a std::function to store the callback allows for flexibility in the type of callable that can be used (function pointers, lambda expressions, functors), enabling users to define their own custom behavior for the button when it is clicked, without being limited to a specific function signature or type
};
