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
	Application(); //Constructor for the Application class, responsible for initializing the application, setting up the main window, loading resources such as fonts, and preparing the inventory and order processor for use in the application, ensuring that all necessary components are ready for the application to run smoothly and handle user interactions effectively
	void run(); //Running the main loop of the application, which will continuously process events, update the application state, and render the user interface until the user decides to close the application, ensuring that the application remains responsive and functional while it is in use, allowing users to interact with the inventory, simulate order processing, and view metrics through the GUI components

private:
	void processEvents(); //Processing user input events such as mouse clicks and keyboard input, allowing the application to respond to user interactions with the GUI components, ensuring that users can interact with buttons, text boxes, and other elements of the interface to perform actions such as simulating orders and searching for products in the inventory
	void render(); //Rendering the user interface of the application, including drawing the main window, GUI components such as buttons, text boxes, and charts, and displaying relevant information such as inventory details and order processing metrics, ensuring that the application provides a visually appealing and informative interface for users to interact with and monitor the state of the inventory and order processing system

	sf::RenderWindow m_window; //Using an SFML RenderWindow to create the main application window, allowing for rendering of graphics and handling of user input events, ensuring that we have a dedicated window for displaying the application's user interface and responding to user interactions effectively
	sf::Font m_font; //Using an SFML Font to load and manage font resources for rendering text in the application, allowing for consistent and visually appealing text display in the GUI components, ensuring that we can display product information, metrics, and other textual content in a clear and readable manner throughout the application

	Inventory m_inventory; //Using an Inventory object to manage the collection of products available in the application, allowing for storage, retrieval, and management of product information and stock levels, ensuring that we have a centralized data structure for handling the inventory of products that can be displayed and interacted with in the application
	std::unique_ptr<OrderProcessor> m_processor; //Using a unique_ptr to manage the OrderProcessor allows for dynamic allocation and automatic cleanup of the order processor instance, ensuring that we can create and manage the order processor as needed while maintaining proper memory management and avoiding memory leaks in the application

	Button m_simulateBtn; //Using a Button object to represent the "Simulate Orders" button in the GUI, allowing users to interact with the button to trigger the simulation of order processing, ensuring that we have a clear and interactive element in the interface for users to initiate the order processing simulation and view the resulting metrics
	TextBox m_searchBox; //Using a TextBox object to represent the search box in the GUI, allowing users to enter text to search for products in the inventory, ensuring that we have an interactive element in the interface for users to input search queries and retrieve relevant product information from the inventory based on their input
	PieChart m_chart; //Using a PieChart object to visually represent the metrics of order processing, such as the proportion of successful and failed orders, allowing users to quickly and easily understand the performance of the order processing system through a visual representation, ensuring that we provide an informative and engaging way for users to monitor the outcomes of their simulated orders and the overall performance of the system

	std::string m_search; //Using a string to store the current search query entered by the user in the search box, allowing us to keep track of the user's input and use it to filter and display relevant products from the inventory based on the search criteria, ensuring that we can provide a responsive and interactive search functionality in the application
	std::string m_errorMessage; //Using a string to store any error messages that may occur during the application's operation, such as issues with loading the inventory or processing orders, allowing us to provide feedback to the user about any problems that arise and ensure that we can communicate important information about the application's state and any issues that need attention, enhancing the user experience by keeping them informed about the application's status and any errors that may occur

	// --- Sort & Filter ---
	SortCriteria m_currentSort = SortCriteria::QtyDesc; // Default sort
	bool m_isMenuOpen = false;                        // Tracks if the dropdown is visible

	// The  list used for rendering
	std::vector<std::shared_ptr<Product>> m_viewedProducts;

	// UI Elements
	Button m_filterMenuBtn;
	Button m_sortPriceDescBtn;
	Button m_sortQtyBtn;
	Button m_sortPriceBtn;
	Button m_sortQtyAscBtn;

	// Optional: A background for the dropdown to make it look like a real menu
	sf::RectangleShape m_menuBackground;
};


