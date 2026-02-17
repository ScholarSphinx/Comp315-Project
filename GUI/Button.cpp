#include "Button.h"

Button::Button(const sf::Font& font, //Constructor for the Button class, taking a reference to a font, a label string, size, and position as parameters, allowing for the initialization of the button's visual appearance and text label, ensuring that the button is properly configured with the desired font, label, size, and position when it is created in the application
    const std::string& label,
    sf::Vector2f size,
    sf::Vector2f position)
{
    m_shape.setSize(size);
    m_shape.setPosition(position);
    m_shape.setFillColor({ 60,60,60 });
    m_shape.setOutlineThickness(1.f);
    m_shape.setOutlineColor({ 120,120,140 });

	m_text.emplace(font, label, 16);  //Emplacing a new sf::Text object into the optional m_text member variable, initializing it with the provided font, label string, and character size of 16, allowing for the button to display a text label using the specified font and size, ensuring that the button's label is properly configured and ready to be drawn on the screen
    m_text->setPosition(position + sf::Vector2f{ 10.f, 8.f });
}

void Button::setCallback(std::function<void()> cb) {
	m_callback = cb; //Setting the callback function for the button by assigning the provided std::function to the m_callback member variable, allowing users to define custom behavior that will be executed when the button is clicked, enabling interactive functionality for the button in the application
}

void Button::handleEvent(const sf::Event& event, //Handling events for the button, allowing it to respond to user interactions such as mouse clicks, ensuring that the button can trigger its callback function when the appropriate events occur, enabling interactive behavior for the button in the application
    const sf::RenderWindow& window)
{
	if (event.is<sf::Event::MouseButtonPressed>()) //Checking if the event is a mouse button press event, allowing the button to respond specifically to mouse clicks, ensuring that the button's interactive behavior is triggered only when the user clicks on it, enabling users to interact with the button in the application
    {
		auto mouse = sf::Mouse::getPosition(window); //Getting the current position of the mouse cursor relative to the provided render window, allowing us to determine where the user clicked in relation to the button's position and size, ensuring that we can check if the click occurred within the bounds of the button to trigger the appropriate response
		if (m_shape.getGlobalBounds().contains(//Checking if the mouse click position is within the bounds of the button's shape, allowing us to determine if the user clicked on the button, ensuring that we only trigger the button's callback function when the click occurred within the button's area, enabling accurate and responsive interaction with the button in the application
            sf::Vector2f{ (float)mouse.x, (float)mouse.y }))
        {
			if (m_callback) m_callback(); //If the mouse click occurred within the button's bounds and a callback function is set, we call the callback function to execute the defined behavior for the button click, allowing users to interact with the button and trigger custom functionality in the application when they click on it
        }
    }
}

void Button::draw(sf::RenderWindow& window) { //Drawing the button on the provided render window, allowing it to be visually represented in the application, ensuring that the button's shape and text are rendered correctly on the screen, enabling users to see and interact with the button in the application
    window.draw(m_shape);
    if (m_text) window.draw(*m_text);
}
