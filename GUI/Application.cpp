#include "Application.h"
#include "../IO/CSVLoader.h"
#include "../Config.h"
#include "../Core/DiscountedProduct.h"
#include "../Core/TaxableProduct.h"
#include "../IO/ProductManager.h"

#include <SFML/Graphics.hpp>
#include <optional>
#include <thread>
#include <cmath>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

Application::Application()
	: m_window(sf::VideoMode({ 1150u,600u }), "Inventory System") //Creating the main application window with a specified size and title, allowing us to display the user interface and interact with the inventory system, ensuring that we have a dedicated space for rendering the GUI components and providing a visually appealing interface for users to engage with the application
{
    m_window.setFramerateLimit(60);

    sf::Image icon;
	if (icon.loadFromFile("Assets/icon.jpeg")) //Loading an icon image from a file to use as the application window's icon, allowing us to customize the appearance of the application in the operating system's taskbar and window decorations, ensuring that we provide a unique and recognizable visual identity for the application
        m_window.setIcon(icon);

	if (!m_font.openFromFile("Assets/font.ttf")) //Loading a font from a file to use for rendering text in the application, allowing us to display product information, metrics, and other textual content in a clear and visually appealing manner throughout the GUI components, ensuring that we have a consistent and attractive font for all text elements in the application. If the font fails to load, we set an error message to inform the user about the issue.
        m_errorMessage = "Font failed to load.";

    std::string error;
	if (!CSVLoader::load("Assets/inventory.csv", m_inventory, error)) //Loading product data from a CSV file into the inventory, parsing each line of the file to create product objects and add them to the inventory, while handling potential errors in file access and data parsing. If the loading process fails, we set an error message to inform the user about the issue, ensuring that we provide feedback on any problems encountered during the loading of the inventory data.
        m_errorMessage = error;

	m_processor = std::make_unique<OrderProcessor>(m_inventory); //Creating an instance of the OrderProcessor class, passing a reference to the inventory, allowing the order processor to access and interact with the inventory data for processing orders, ensuring that the order processing logic can manage stock levels and calculate revenues based on the products available in the inventory

	m_simulateBtn = Button(m_font, "Run Simulation", { 150,40 }, { 40,30 }); //Initializing the "Run Simulation" button with a specified font, label, size, and position, allowing users to interact with the button to trigger the simulation of order processing, ensuring that we have a clear and interactive element in the interface for users to initiate the order processing simulation and view the resulting metrics
	m_simulateBtn.setCallback([this] { //Setting a callback function for the "Run Simulation" button, allowing users to define custom behavior that will be executed when the button is clicked. In this case, we start a new thread to run the order processing simulation, allowing the simulation to run concurrently without blocking the main thread of the application, ensuring that the user interface remains responsive while the simulation is running and allowing users to interact with other elements of the interface during the simulation process.
        std::thread([this] {
			m_processor->simulate(); //Running the order processing simulation in a separate thread, allowing the simulation to execute concurrently without blocking the main thread of the application, ensuring that the user interface remains responsive while the simulation is running and allowing users to interact with other elements of the interface during the simulation process
			}).detach(); //Detaching the thread allows it to run independently without needing to join it back to the main thread, ensuring that we can start the simulation and let it run in the background while the main application continues to function and respond to user interactions without waiting for the simulation to complete.
        });

	m_searchBox = TextBox(m_font, { 300, 30 }, { 200, 80 }); //Initializing the search text box with a specified font, size, and position, allowing users to enter text to search for products in the inventory, ensuring that we have an interactive element in the interface for users to input search queries and filter the displayed products accordingly
}

void Application::run()
{
	while (m_window.isOpen()) //Running the main loop of the application, which will continuously process events, update the application state, and render the user interface until the user decides to close the application. This loop ensures that the application remains responsive and functional while it is in use, allowing users to interact with the inventory, simulate order processing, and view metrics through the GUI components.
    {
        processEvents();
        render();
    }
}

void Application::processEvents()
{
	while (std::optional<sf::Event> e = m_window.pollEvent()) //Polling for user input events such as mouse clicks and keyboard input, allowing the application to respond to user interactions with the GUI components. This loop ensures that users can interact with buttons, text boxes, and other elements of the interface to perform actions such as simulating orders and searching for products in the inventory, providing a responsive and interactive experience for users engaging with the application.
    {
        if (e->is<sf::Event::Closed>()) //Checking if the event is a "Closed" event, which indicates that the user has requested to close the application window. If this event is detected, we call `m_window.close()` to close the application window and terminate the main loop, ensuring that the application exits gracefully when the user decides to close it.
        {
            m_window.close();
        }
		else if (auto* text = e->getIf<sf::Event::TextEntered>()) //Checking if the event is a "TextEntered" event, which indicates that the user has entered text input. If this event is detected, we check if the search box is focused and then process the text input accordingly. This allows users to enter search queries in the search box to filter products in the inventory, providing an interactive way for users to find specific products based on their input.
        {
            if (text->unicode == 8) // Backspace
            {
                if (!m_search.empty())
                    m_search.pop_back();
            }
            else if (text->unicode < 128)
            {
                m_search += static_cast<char>(text->unicode);
            }
        }


        // Key Press Handling

        else if (auto* key = e->getIf<sf::Event::KeyPressed>())
        {

            // A Key = Add Product

            if (key->code == sf::Keyboard::Key::A)
            {
                static int nextId = 1000 + rand() % 9000;

                // Alternate between product types
                std::shared_ptr<Product> product;

                if (nextId % 2 == 0)
                {
                    product = std::make_shared<DiscountedProduct>(
                        nextId,
                        "DiscountedItem",
                        100.0,
                        20,
                        0.10   // 10% discount
                    );
                }
                else
                {
                    product = std::make_shared<TaxableProduct>(
                        nextId,
                        "TaxableItem",
                        100.0,
                        20,
                        0.15   // 15% tax
                    );
                }

                m_inventory.addProduct(product);

                ProductFileManager::addProductToFile(
                    INVENTORY_FILE,
                    product
                );

                nextId++;
            }


            // D Key = Remove Product

            if (key->code == sf::Keyboard::Key::D)
            {
                const auto& products = m_inventory.getProducts();

                if (!products.empty())
                {
                    int idToRemove =
                        products.begin()->first;

                    m_inventory.removeProduct(idToRemove);

                    ProductFileManager::removeProductFromFile(
                        INVENTORY_FILE,
                        idToRemove
                    );
                }
            }


            // R Key = Restock Product

            if (key->code == sf::Keyboard::Key::R)
            {
                const auto& products = m_inventory.getProducts();

                if (!products.empty())
                {
                    int idToRestock =
                        products.begin()->first;

                    m_inventory.increaseStock(
                        idToRestock,
                        10   // restock amount
                    );
                }
            }
        }

		m_searchBox.handleEvent(*e, m_window); //Handling events for the search text box, including mouse clicks for focus and text input, allowing users to enter text to search for products in the inventory. The TextBox handles focus, backspace, character input, and updates its internal value, providing an interactive search functionality in the application.

		m_simulateBtn.handleEvent(*e, m_window); //Handling events for the "Run Simulation" button, allowing the button to respond to user interactions such as mouse clicks. This ensures that when users click the button, the associated callback function is executed to start the order processing simulation, providing an interactive element in the interface for users to engage with and trigger the simulation of order processing based on their input.
    }
}

void Application::render() //Rendering the user interface of the application, including drawing the main window, GUI components such as buttons, text boxes, and charts, and displaying relevant information such as inventory details and order processing metrics. This function ensures that the application provides a visually appealing and informative interface for users to interact with and monitor the state of the inventory and order processing system, allowing users to see the results of their interactions and the current status of the inventory and order processing metrics in real-time.
{
    m_window.clear({ 24,26,32 });

	if (!m_errorMessage.empty()) //Checking if there is an error message to display, which indicates that there was an issue during the initialization of the application, such as failing to load the font or inventory data. If an error message exists, we create a text object with the error message, set its position and color, and draw it on the window before displaying it. This ensures that users are informed about any problems encountered during the setup of the application, providing feedback on issues that may prevent the application from functioning correctly.
    {
        sf::Text error(m_font, m_errorMessage, 20);
        error.setPosition({ 40,40 });
        error.setFillColor(sf::Color::Red);
        m_window.draw(error);
        m_window.display();
        return;
    }

	m_simulateBtn.draw(m_window); //Drawing the "Run Simulation" button on the window, allowing it to be visually represented in the application. This ensures that the button's shape and text are rendered correctly on the screen, enabling users to see and interact with the button in the application, providing a clear and interactive element for users to initiate the order processing simulation.

	sf::Text searchLabel(m_font, "Search Products:", 18); //Creating a text object for the search label, which provides a visual indicator for the search functionality. This label is positioned above the search text box to make it easily visible and accessible for users when they are entering their search queries.
    searchLabel.setPosition({ 42, 82 });
    m_window.draw(searchLabel);

	m_searchBox.draw(m_window); //Drawing the search box on the window, allowing it to be visually represented in the application. This ensures that the search box's shape and any text within it are rendered correctly on the screen, enabling users to see and interact with the search box in the application, providing a clear and interactive element for users to enter their search queries and filter the displayed products accordingly.

	auto products = m_inventory.getProducts(); //Retrieving the products from the inventory, allowing us to access the product data for rendering in the user interface. This provides us with the necessary information about each product, such as its name and stock quantity, which we can use to create visual representations of the products in the application, enabling users to see the details of the products available in the inventory and their current stock levels.
    float x = 40, y = 120;
    int col = 0;

	m_search = m_searchBox.getValue(); //Retrieving the current search query from the text box, allowing us to use it for filtering products in the inventory. This ensures that the search functionality is synchronized with the text box's value, providing a responsive and interactive search experience for users.

	for (auto& [id, p] : products) //Iterating through the products in the inventory, allowing us to create visual representations of each product in the user interface. For each product, we check if it matches the current search query entered by the user, and if it does, we create a card to display the product's name and stock quantity. This loop ensures that only products that match the search criteria are displayed, providing a filtered view of the inventory based on user input, and allowing users to easily find and monitor specific products in the inventory.
    {
        if (!m_search.empty() &&
			p->getName().find(m_search) == std::string::npos)// Checking if the search query is not empty and if the product's name does not contain the search query, allowing us to filter out products that do not match the user's search input. If a product does not match the search criteria, we skip rendering it in the user interface, ensuring that users only see products that are relevant to their search query, providing a more focused and efficient way for users to find specific products in the inventory.
            continue;

		sf::RectangleShape card({ 220,70 }); //Creating a rectangle shape to represent a product card or a bar in the user interface, allowing us to visually display information about each product in a structured and organized manner. The card will contain the product's name and stock quantity, providing users with a clear and concise representation of the products available in the inventory, enabling them to easily monitor stock levels and identify specific products based on their search queries.
        card.setPosition({ x + col * 240.f, y });
        card.setFillColor({ 36,38,45 });
        card.setOutlineThickness(2);
        card.setOutlineColor({ 70,70,80 });
        m_window.draw(card);


        sf::RectangleShape stockIndicator({ 15,64 });
        stockIndicator.setPosition({ x+203 + col * 240.f, y+3 });
        stockIndicator.setFillColor({ 36,38,45 });
        stockIndicator.setOutlineThickness(1);
        stockIndicator.setOutlineColor({ 70,70,80 });
        
		int stockcount = p->getQuantity(); //Retrieving the stock quantity of the product, allowing us to determine how much of the product is currently available in the inventory. This information is crucial for rendering the stock indicator, which visually represents the stock level of the product in the user interface. By checking the stock quantity, we can set the color of the stock indicator to reflect whether the product is out of stock, low on stock, or sufficiently stocked, providing users with a quick and intuitive way to assess the availability of each product in the inventory at a glance.

        if (stockcount == 0)
			stockIndicator.setFillColor({ 200,50,50 }); // Setting the fill color of the stock indicator to red when the stock count is zero, indicating that the product is out of stock. This visual cue allows users to quickly identify products that are unavailable, providing an immediate understanding of the inventory status and helping users to make informed decisions about which products they can order or need to restock.
        else if (stockcount <= 10)
			stockIndicator.setFillColor({ 200,150,50 }); // Setting the fill color of the stock indicator to orange when the stock count is less than or equal to 10, indicating that the product is low on stock. This visual cue allows users to quickly identify products that are running low, providing an immediate understanding of the inventory status and helping users to prioritize which products may need restocking soon or which products they should consider ordering before they run out.
        else if (stockcount <= 20)
			stockIndicator.setFillColor({ 200,200,50 }); // Setting the fill color of the stock indicator to yellow when the stock count is less than or equal to 20, indicating that the product is moderately stocked. This visual cue allows users to quickly identify products that have a reasonable amount of stock available, providing an immediate understanding of the inventory status and helping users to assess which products are sufficiently stocked and which ones may require attention in terms of restocking or ordering.
        else
			stockIndicator.setFillColor({ 50,200,50 }); // Setting the fill color of the stock indicator to green when the stock count is greater than 20, indicating that the product is well-stocked. This visual cue allows users to quickly identify products that have a healthy amount of stock available, providing an immediate understanding of the inventory status and helping users to feel confident about ordering or relying on those products without concern for running out of stock in the near future.
        m_window.draw(stockIndicator);

        sf::Text t(m_font,
			p->getName() + "\nStock: " + std::to_string(p->getQuantity()), 16); //Creating a text object to display the product's name and stock quantity, allowing us to provide detailed information about each product in the user interface. This text will be rendered on top of the product card, giving users a clear and concise representation of the product's details, enabling them to easily monitor stock levels and identify specific products based on their search queries, providing a more informative and engaging user experience when interacting with the inventory system.
        t.setPosition({ x + 10 + col * 240.f, y + 10 });
        m_window.draw(t);

        col++;
        if (col == 3) { col = 0; y += 90; }
    }

	sf::RectangleShape statBack({ 300,430 }); //Creating a rectangle shape to serve as the background for the statistics section of the user interface, allowing us to visually separate and highlight the area where order processing metrics will be displayed. This background provides a clear and organized space for presenting important information about the orders, such as total orders, successful orders, failed orders, and thread count, enabling users to easily monitor the performance of the order processing system and understand the current state of the inventory and order processing metrics at a glance.
    statBack.setPosition({ 800, 120 });
    statBack.setFillColor({ 36,30,43 });
    statBack.setOutlineThickness(2);
    statBack.setOutlineColor({ 70,70,80 });
    m_window.draw(statBack);

	auto& metrics = m_processor->getMetrics(); //Retrieving the metrics from the order processor, allowing us to access the data related to the total number of orders processed, successful orders, failed orders, and thread count. This information is crucial for rendering the statistics section of the user interface, where we will display these metrics in a clear and organized manner. By accessing the metrics, we can provide users with insights into the performance of the order processing system, helping them to understand how well the system is functioning and identify any potential issues or areas for improvement based on the displayed metrics.

    float xStat = 807;
    float yStat = 120;
    int colStat = 0;

    for (int i = 0; i < 4; i++) {
        sf::RectangleShape statCard({ 130,45 });

        if (i == 0) { // Setting the fill color of the stat card to a neutral gray for the "Orders" metric, providing a distinct visual representation for this particular metric in the statistics section of the user interface. This color choice helps to differentiate the "Orders" metric from other metrics such as "Success," "Failed," and "Threads," allowing users to quickly identify and focus on the specific information related to the total number of orders processed, providing a clear and organized presentation of the order processing metrics in the application.
			statCard.setFillColor({ 60,60,60 }); 
        }
        else if (i == 1) { // Setting the fill color of the stat card to green for the "Success" metric, providing a distinct visual representation for this particular metric in the statistics section of the user interface. This color choice helps to differentiate the "Success" metric from other metrics such as "Orders," "Failed," and "Threads," allowing users to quickly identify and focus on the specific information related to the number of successful orders processed, providing a clear and organized presentation of the order processing metrics in the application, and conveying a positive performance indicator for successful order processing.
            xStat += 156;
			statCard.setFillColor({ 10,120,10 });         }
        else if (i == 2) { // Setting the fill color of the stat card to a dark red for the "Threads" metric, providing a distinct visual representation for this particular metric in the statistics section of the user interface. This color choice helps to differentiate the "Threads" metric from other metrics such as "Orders," "Success," and "Failed," allowing users to quickly identify and focus on the specific information related to the number of threads used in the order processing simulation, providing a clear and organized presentation of the order processing metrics in the application, and conveying a sense of technical performance related to multithreading in the order processing system.
            yStat += 55;
			statCard.setFillColor({ 120,10,15 }); 
        }
        else if (i == 3) {// Setting the fill color of the stat card to a very dark color for the "Failed" metric, providing a distinct visual representation for this particular metric in the statistics section of the user interface. This color choice helps to differentiate the "Failed" metric from other metrics such as "Orders," "Success," and "Threads," allowing users to quickly identify and focus on the specific information related to the number of failed orders processed, providing a clear and organized presentation of the order processing metrics in the application, and conveying a sense of caution or concern related to failed order processing, helping users to easily identify areas that may require attention or improvement in the order processing system.
            xStat -=156;
			statCard.setFillColor({ 10,10,15 });         
        };
        statCard.setPosition({ xStat, yStat });
        statCard.setOutlineThickness(1);
        statCard.setOutlineColor({ 70,70,80 });

        m_window.draw(statCard);
    }

    sf::Text orderstat1(m_font,
		"Orders: " + std::to_string(metrics.total.load()), 18); //Creating a text object to display the total number of orders processed, allowing us to provide users with information about the overall activity of the order processing system. This text will be rendered in the statistics section of the user interface, giving users a clear and concise representation of the total orders, enabling them to monitor the performance of the system and understand how many orders have been processed at a glance, providing valuable insights into the workload and efficiency of the order processing system.
   orderstat1.setPosition({ 825,130 });   
    m_window.draw(orderstat1);

    sf::Text successstat2(m_font,
		"\nSuccess: " + std::to_string(metrics.success.load()), 18); //Creating a text object to display the number of successful orders processed, allowing us to provide users with information about the effectiveness of the order processing system. This text will be rendered in the statistics section of the user interface, giving users a clear and concise representation of the successful orders, enabling them to monitor the performance of the system and understand how many orders have been successfully processed at a glance, providing valuable insights into the success rate and efficiency of the order processing system.
   successstat2.setPosition({ 975,110 });    
    m_window.draw(successstat2);

    sf::Text failedstat3(m_font,
		"\nFailed: " + std::to_string(metrics.failed.load()), 18); //Creating a text object to display the number of failed orders processed, allowing us to provide users with information about the challenges or issues faced by the order processing system. This text will be rendered in the statistics section of the user interface, giving users a clear and concise representation of the failed orders, enabling them to monitor the performance of the system and understand how many orders have encountered problems at a glance, providing valuable insights into potential areas for improvement in the order processing system.
    failedstat3.setPosition({ 975,165 });   
    m_window.draw(failedstat3);

    sf::Text threadstat4(m_font,
		"\nThreads: " + std::to_string(THREAD_COUNT), 18); //Creating a text object to display the number of threads used in the order processing simulation, allowing us to provide users with information about the level of concurrency and parallelism in the system. This text will be rendered in the statistics section of the user interface, giving users a clear and concise representation of the thread count, enabling them to understand how many threads are being utilized for processing orders at a glance, providing valuable insights into the performance and efficiency of the order processing system based on its use of multithreading.
    threadstat4.setPosition({ 825,165 });   
    m_window.draw(threadstat4);

    float total = (float)metrics.total.load();
	float ratio = total > 0 ? metrics.success.load() / total : 0.f; //Calculating the success ratio of orders, allowing us to determine the proportion of successful orders relative to the total number of orders processed. This ratio is crucial for rendering the pie chart that visually represents the success and failure rates of the order processing system, providing users with an intuitive understanding of the system's performance at a glance, and helping them to assess the effectiveness of the order processing system based on the visual representation of the success ratio in the pie chart.

	m_chart.draw(m_window, { 950,375 }, 120.f, ratio); //Drawing a pie chart on the window to visually represent the success and failure rates of the order processing system, allowing users to quickly understand the performance of the system at a glance. The chart is positioned at specific coordinates and has a defined radius, with the success ratio determining how much of the chart is filled to indicate successful orders versus failed orders, providing an intuitive and engaging way for users to assess the effectiveness of the order processing system based on the visual representation of the success ratio in the pie chart.

	sf::Text chartLabel(m_font, "Success vs Failed", 18); //Creating a text object to serve as a label for the pie chart, allowing us to provide context and information about what the chart represents. This label will be rendered near the pie chart, giving users a clear understanding of the data being visualized in the chart, enabling them to quickly grasp that the chart is comparing successful orders versus failed orders, providing valuable insights into the performance of the order processing system based on the visual representation of the success ratio in the pie chart.
    chartLabel.setPosition({ 880,505 });
    m_window.draw(chartLabel);

    m_window.display();
}
