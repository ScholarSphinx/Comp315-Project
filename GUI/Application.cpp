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
#include <algorithm>
#include <cctype>
#include <SFML/System.hpp>
#include <stdexcept>

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
    // In your main entry point or Controller
    std::string targetFile;

    if (EDIT_MODE) {
        targetFile = EDITED_INVENTORY_FILE;
    }
    else {
        targetFile = INVENTORY_FILE; // Your standard read-only/base file
    }

	if (!CSVLoader::load(targetFile, m_inventory, error)) //Loading product data from a CSV file into the inventory, parsing each line of the file to create product objects and add them to the inventory, while handling potential errors in file access and data parsing. If the loading process fails, we set an error message to inform the user about the issue, ensuring that we provide feedback on any problems encountered during the loading of the inventory data.
        m_errorMessage = error;

	m_processor = std::make_unique<OrderProcessor>(m_inventory); //Creating an instance of the OrderProcessor class, passing a reference to the inventory, allowing the order processor to access and interact with the inventory data for processing orders, ensuring that the order processing logic can manage stock levels and calculate revenues based on the products available in the inventory

	m_simulateBtn = Button(m_font, "Run Simulation", { 150,40 }, { 40,30 }); //Initializing the "Run Simulation" button with a specified font, label, size, and position, allowing users to interact with the button to trigger the simulation of order processing, ensuring that we have a clear and interactive element in the interface for users to initiate the order processing simulation and view the resulting metrics
	m_simulateBtn.setCallback([this] { //Setting a callback function for the "Run Simulation" button, allowing users to define custom behavior that will be executed when the button is clicked. In this case, we start a new thread to run the order processing simulation, allowing the simulation to run concurrently without blocking the main thread of the application, ensuring that the user interface remains responsive while the simulation is running and allowing users to interact with other elements of the interface during the simulation process.
        std::thread([this] {
			m_processor->simulate(); //Running the order processing simulation in a separate thread, allowing the simulation to execute concurrently without blocking the main thread of the application, ensuring that the user interface remains responsive while the simulation is running and allowing users to interact with other elements of the interface during the simulation process
			}).detach(); //Detaching the thread allows it to run independently without needing to join it back to the main thread, ensuring that we can start the simulation and let it run in the background while the main application continues to function and respond to user interactions without waiting for the simulation to complete.
        });

	m_searchBox = TextBox(m_font, { 300, 30 }, { 200, 80 }); //Initializing the search text box with a specified font, size, and position, allowing users to enter text to search for products in the inventory, ensuring that we have an interactive element in the interface for users to input search queries and filter the displayed products accordingly

    // Main Menu Button
    m_filterMenuBtn = Button(m_font, "Filter/Sort", { 120, 30 }, { 520, 80 });
    m_filterMenuBtn.setCallback([this] {
        m_isMenuOpen = !m_isMenuOpen; // Toggle the dropdown
        });

    // Dropdown Option 1: Price Low to High
    m_sortPriceBtn = Button(m_font, "Price: Asc", { 120, 30 }, { 520, 110 });
    m_sortPriceBtn.setCallback([this] {
        m_currentSort = SortCriteria::PriceAsc;
        m_viewedProducts = m_inventory.getSortedProducts(SortCriteria::PriceAsc);
        m_isMenuOpen = false;
        });

    // Dropdown Option 2: Price High to Low
    m_sortPriceDescBtn = Button(m_font, "Price: Desc", { 120, 30 }, { 520, 140 });
    m_sortPriceDescBtn.setCallback([this] {
        m_currentSort = SortCriteria::PriceDesc;
        m_viewedProducts = m_inventory.getSortedProducts(SortCriteria::PriceDesc);
        m_isMenuOpen = false;
        });

    // Dropdown Option 3: Stock Low to High
    m_sortQtyAscBtn = Button(m_font, "Qty: Asc", { 120, 30 }, { 520, 170 });
    m_sortQtyAscBtn.setCallback([this] {
        m_currentSort = SortCriteria::QtyAsc;
        m_viewedProducts = m_inventory.getSortedProducts(SortCriteria::QtyAsc);
        m_isMenuOpen = false;
        });

    // Dropdown Option 4: Stock High to Low
    m_sortQtyBtn = Button(m_font, "Qty: Desc", { 120, 30 }, { 520, 200 }); // Next slot at 200
    m_sortQtyBtn.setCallback([this] {
        m_currentSort = SortCriteria::QtyDesc;
        m_viewedProducts = m_inventory.getSortedProducts(SortCriteria::QtyDesc);
        m_isMenuOpen = false;
        });

    m_sortNameAscBtn = Button(m_font, "Name: Asc", { 120, 30 }, { 520, 230 });
    m_sortNameAscBtn.setCallback([this] {
        m_currentSort = SortCriteria::NameAsc;
        m_viewedProducts = m_inventory.getSortedProducts(SortCriteria::NameAsc);
        m_isMenuOpen = false;
    });

    m_sortNameDescBtn = Button(m_font, "Name: Desc", { 120, 30 }, { 520, 260 });
    m_sortNameDescBtn.setCallback([this] {
        m_currentSort = SortCriteria::NameDesc;
        m_viewedProducts = m_inventory.getSortedProducts(SortCriteria::NameDesc);
        m_isMenuOpen = false;
    });

    m_sortIdAscBtn = Button(m_font, "ID: Asc", { 120, 30 }, { 520, 290 });
    m_sortIdAscBtn.setCallback([this] {
        m_currentSort = SortCriteria::IDAsc;
        m_viewedProducts = m_inventory.getSortedProducts(SortCriteria::IDAsc);
        m_isMenuOpen = false;
        });

    m_sortIdDescBtn = Button(m_font, "ID: Desc", { 120, 30 }, { 520, 320 });
    m_sortIdDescBtn.setCallback([this] {
        m_currentSort = SortCriteria::IDDesc;
        m_viewedProducts = m_inventory.getSortedProducts(SortCriteria::IDDesc);
        m_isMenuOpen = false;
        });

    m_filterTaxBtn = Button(m_font, "Taxable", { 120, 30 }, { 520, 350 });
    m_filterTaxBtn.setCallback([this] {
        m_currentSort = SortCriteria::Tax;
        m_viewedProducts = m_inventory.getSortedProducts(SortCriteria::Tax);
        m_isMenuOpen = false;
        });

    m_filterDiscBtn = Button(m_font, "Discountable", { 120, 30 }, { 520, 380 });
    m_filterDiscBtn.setCallback([this] {
        m_currentSort = SortCriteria::Disc;
        m_viewedProducts = m_inventory.getSortedProducts(SortCriteria::Disc);
        m_isMenuOpen = false;
        });
    // Initial load so the screen isn't blank
    m_viewedProducts = m_inventory.getSortedProducts(SortCriteria::QtyDesc);

    m_viewedProducts = m_inventory.getSortedProducts(m_currentSort);

    m_showAddProductBtn = Button(m_font, "Add Product", { 120, 30 }, { 800, 80 });
    m_showAddProductBtn.setCallback([this] {
        m_isRemoveProductPanelOpen = false;
        m_isAddProductPanelOpen = true;
        m_selectedAddType = "TAX";
        m_addProductError.clear();
        m_addNameBox.clear();
        m_addStockBox.clear();
        m_addPriceBox.clear();
        });

    m_addTypeTaxBtn = Button(m_font, "TAX", { 230, 30 }, { 835, 202 });
    m_addTypeTaxBtn.setCallback([this] {
        m_selectedAddType = "TAX";
        });

    m_addTypeDiscBtn = Button(m_font, "DISC", { 230, 30 }, { 835, 236 });
    m_addTypeDiscBtn.setCallback([this] {
        m_selectedAddType = "DISC";
        });

    m_addNameBox = TextBox(m_font, { 230, 30 }, { 835, 314 });
    m_addStockBox = TextBox(m_font, { 230, 30 }, { 835, 384 });
    m_addPriceBox = TextBox(m_font, { 230, 30 }, { 835, 454 });

    m_submitAddProductBtn = Button(m_font, "Add Product", { 150, 36 }, { 875, 500 });
    m_submitAddProductBtn.setCallback([this] {
        submitAddProductForm();
        });

    m_closeOverlayBtn = Button(m_font, "X", { 32, 28 }, { 1060, 128 });
    m_closeOverlayBtn.setCallback([this] {
        m_isAddProductPanelOpen = false;
        m_isRemoveProductPanelOpen = false;
        m_addProductError.clear();
        m_removeProductError.clear();
        });

    m_showRemoveProductBtn = Button(m_font, "Remove Product", { 140, 30 }, { 960, 80 });
    m_showRemoveProductBtn.setCallback([this] {
        m_isAddProductPanelOpen = false;
        m_isRemoveProductPanelOpen = true;
        m_removeProductError.clear();
        m_removeIdBox.clear();
        });

    m_removeIdBox = TextBox(m_font, { 230, 30 }, { 835, 250 });
    m_submitRemoveProductBtn = Button(m_font, "Remove Product", { 170, 36 }, { 865, 330 });
    m_submitRemoveProductBtn.setCallback([this] {
        submitRemoveProductForm();
        });

	m_restockBtn = Button(m_font, "Restock Low Inventory", { 200, 30 }, { 850, 560 });
    m_restockBtn.setCallback([this] {
        m_inventory.restockAll(MEDIUM_STOCK_THRESHOLD, RESTOCK_AMOUNT, RESTOCK_THRESHOLD);
        m_viewedProducts = m_inventory.getSortedProducts(m_currentSort);
		});

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
    

		m_searchBox.handleEvent(*e, m_window); //Handling events for the search text box, including mouse clicks for focus and text input, allowing users to enter text to search for products in the inventory. The TextBox handles focus, backspace, character input, and updates its internal value, providing an interactive search functionality in the application.
        if (m_isAddProductPanelOpen)
        {
            m_addNameBox.handleEvent(*e, m_window);
            m_addStockBox.handleEvent(*e, m_window);
            m_addPriceBox.handleEvent(*e, m_window);
            m_addTypeTaxBtn.handleEvent(*e, m_window);
            m_addTypeDiscBtn.handleEvent(*e, m_window);
        }
        if (m_isRemoveProductPanelOpen)
        {
            m_removeIdBox.handleEvent(*e, m_window);
        }

		m_simulateBtn.handleEvent(*e, m_window); //Handling events for the "Run Simulation" button, allowing the button to respond to user interactions such as mouse clicks. This ensures that when users click the button, the associated callback function is executed to start the order processing simulation, providing an interactive element in the interface for users to engage with and trigger the simulation of order processing based on their input.
        m_showAddProductBtn.handleEvent(*e, m_window);
        m_showRemoveProductBtn.handleEvent(*e, m_window);
		m_restockBtn.handleEvent(*e, m_window);
        if (m_isAddProductPanelOpen)
        {
            m_submitAddProductBtn.handleEvent(*e, m_window);
        }
        if (m_isRemoveProductPanelOpen)
        {
            m_submitRemoveProductBtn.handleEvent(*e, m_window);
        }
        if (m_isAddProductPanelOpen || m_isRemoveProductPanelOpen)
        {
            m_closeOverlayBtn.handleEvent(*e, m_window);
        }
        if (!m_isAddProductPanelOpen && !m_isRemoveProductPanelOpen)
        {
            m_filterMenuBtn.handleEvent(*e, m_window); //Handle click for opening/closing the dropdown menu.
            if (m_isMenuOpen)
            {
                m_sortPriceBtn.handleEvent(*e, m_window);
                m_sortPriceDescBtn.handleEvent(*e, m_window);
                m_sortQtyAscBtn.handleEvent(*e, m_window);
                m_sortQtyBtn.handleEvent(*e, m_window);
				m_sortNameAscBtn.handleEvent(*e, m_window);
				m_sortNameDescBtn.handleEvent(*e, m_window);
				m_sortIdAscBtn.handleEvent(*e, m_window);
				m_sortIdDescBtn.handleEvent(*e, m_window);
                m_filterTaxBtn.handleEvent(*e, m_window);
                m_filterDiscBtn.handleEvent(*e, m_window);
            }
        }
    }
}

void Application::submitAddProductForm()
{
    const std::string productName = m_addNameBox.getValue();
    if (productName.empty())
    {
        m_addProductError = "Name is required.";
        return;
    }

    int quantity = 0;
    double price = 0.0;
    try
    {
        quantity = std::stoi(m_addStockBox.getValue());
        price = std::stod(m_addPriceBox.getValue());
    }
    catch (const std::exception&)
    {
        m_addProductError = "Stock and price must be valid numbers.";
        return;
    }

    if (quantity < 0 || price < 0.0)
    {
        m_addProductError = "Stock and price must be non-negative.";
        return;
    }

    const int nextId = ProductFileManager::calculateNextId(INVENTORY_FILE);
    const std::string type = m_selectedAddType;
    const double rate = (type == "DISC") ? DISC_RATE : TAX_RATE;

    std::shared_ptr<Product> product;
    if (type == "DISC")
    {
        product = std::make_shared<DiscountedProduct>(nextId, productName, price, quantity, rate);
    }
    else
    {
        product = std::make_shared<TaxableProduct>(nextId, productName, price, quantity, rate);
    }
    m_inventory.addProduct(product);
    ProductFileManager::addProductToFile(INVENTORY_FILE, product, type, rate);
    ProductFileManager::addProductToFile(EDITED_INVENTORY_FILE, product, type, rate);

    m_viewedProducts = m_inventory.getSortedProducts(m_currentSort);
    m_addProductError.clear();
    m_selectedAddType = "TAX";
    m_addNameBox.clear();
    m_addStockBox.clear();
    m_addPriceBox.clear();
    m_isAddProductPanelOpen = false;
}

void Application::submitRemoveProductForm()
{
    int productId = 0;
    try
    {
        productId = std::stoi(m_removeIdBox.getValue());
    }
    catch (const std::exception&)
    {
        m_removeProductError = "Product ID must be a valid number.";
        return;
    }

    if (productId <= 0)
    {
        m_removeProductError = "Product ID must be greater than 0.";
        return;
    }

    // Use existing inventory remove/deactivate implementation.
    m_inventory.deactivateProduct(productId);
    m_viewedProducts = m_inventory.getSortedProducts(m_currentSort);
    m_removeProductError.clear();
    m_removeIdBox.clear();
    m_isRemoveProductPanelOpen = false;
}

std::string toLower(std::string data) {
    std::transform(data.begin(), data.end(), data.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return data;
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

	auto& products = m_viewedProducts; //Render from the currently sorted/filtered view so dropdown selection is reflected on screen.
    float x = 40, y = 120;
    int col = 0;



    std::string searchLower = toLower(m_searchBox.getValue());//Retrieving the current search query from the text box, allowing us to use it for filtering products in the inventory. This ensures that the search functionality is synchronized with the text box's value, providing a responsive and interactive search experience for users.

	for (auto& p : products) //Iterating through the products in the current view list, allowing us to create visual representations of each product in the user interface. For each product, we check if it matches the current search query entered by the user, and if it does, we create a card to display the product's name and stock quantity. This loop ensures that only products that match the search criteria are displayed, providing a filtered view of the inventory based on user input, and allowing users to easily find and monitor specific products in the inventory.
    {
        if (!p->isActive()) { // Assuming isActive() returns the boolean/int status
            continue;
        }

        std::string nameLower = toLower(p->getName()); //should sort case-sensitivity issue

        if (!searchLower.empty() && nameLower.find(searchLower) == std::string::npos)// Checking if the search query is not empty and if the product's name does not contain the search query, allowing us to filter out products that do not match the user's search input. If a product does not match the search criteria, we skip rendering it in the user interface, ensuring that users only see products that are relevant to their search query, providing a more focused and efficient way for users to find specific products in the inventory.
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

        if (stockcount == NO_STOCK_THRESHOLD)
			stockIndicator.setFillColor({ 200,50,50 }); // Setting the fill color of the stock indicator to red when the stock count is zero, indicating that the product is out of stock. This visual cue allows users to quickly identify products that are unavailable, providing an immediate understanding of the inventory status and helping users to make informed decisions about which products they can order or need to restock.
        else if (stockcount <= LOW_STOCK_THRESHOLD)
			stockIndicator.setFillColor({ 200,150,50 }); // Setting the fill color of the stock indicator to orange when the stock count is less than or equal to 10, indicating that the product is low on stock. This visual cue allows users to quickly identify products that are running low, providing an immediate understanding of the inventory status and helping users to prioritize which products may need restocking soon or which products they should consider ordering before they run out.
        else if (stockcount <= MEDIUM_STOCK_THRESHOLD)
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

    //Draw the dropdown so it floats on top of everything
    m_showAddProductBtn.draw(m_window);
    m_showRemoveProductBtn.draw(m_window);
    m_filterMenuBtn.draw(m_window);
	m_restockBtn.draw(m_window);

    if (m_isMenuOpen) {
        m_sortPriceBtn.draw(m_window);
        m_sortPriceDescBtn.draw(m_window);
        m_sortQtyAscBtn.draw(m_window);
        m_sortQtyBtn.draw(m_window);
        m_sortNameAscBtn.draw(m_window);
		m_sortNameDescBtn.draw(m_window);
		m_sortIdAscBtn.draw(m_window);
		m_sortIdDescBtn.draw(m_window);
        m_filterDiscBtn.draw(m_window);
        m_filterTaxBtn.draw(m_window);
    }

    if (m_isAddProductPanelOpen)
    {
        sf::RectangleShape addPanel({ 300,430 });
        addPanel.setPosition({ 800,120 });
        addPanel.setFillColor({ 28,30,38 });
        addPanel.setOutlineThickness(2);
        addPanel.setOutlineColor({ 90,90,110 });
        m_window.draw(addPanel);

        sf::Text panelTitle(m_font, "Add Product", 20);
        panelTitle.setPosition({ 885, 145 });
        m_window.draw(panelTitle);

        sf::Text typeLabel(m_font, "Type", 16);
        typeLabel.setPosition({ 835, 180 });
        m_window.draw(typeLabel);
        m_addTypeTaxBtn.draw(m_window);
        m_addTypeDiscBtn.draw(m_window);

        sf::Text typeState(m_font, "Selected: " + m_selectedAddType, 14);
        typeState.setPosition({ 835, 270 });
        typeState.setFillColor({ 200, 220, 255 });
        m_window.draw(typeState);

        sf::Text nameLabel(m_font, "Name", 16);
        nameLabel.setPosition({ 835, 292 });
        m_window.draw(nameLabel);
        m_addNameBox.draw(m_window);

        sf::Text stockLabel(m_font, "Stock", 16);
        stockLabel.setPosition({ 835, 362 });
        m_window.draw(stockLabel);
        m_addStockBox.draw(m_window);

        sf::Text priceLabel(m_font, "Price", 16);
        priceLabel.setPosition({ 835, 432 });
        m_window.draw(priceLabel);
        m_addPriceBox.draw(m_window);

        if (!m_addProductError.empty())
        {
            sf::Text errorText(m_font, m_addProductError, 14);
            errorText.setPosition({ 815, 486 });
            errorText.setFillColor({ 230, 90, 90 });
            m_window.draw(errorText);
        }

        m_submitAddProductBtn.draw(m_window);
        m_closeOverlayBtn.draw(m_window);
    }
    if (m_isRemoveProductPanelOpen)
    {
        sf::RectangleShape removePanel({ 300,430 });
        removePanel.setPosition({ 800,120 });
        removePanel.setFillColor({ 28,30,38 });
        removePanel.setOutlineThickness(2);
        removePanel.setOutlineColor({ 90,90,110 });
        m_window.draw(removePanel);

        sf::Text panelTitle(m_font, "Remove Product", 20);
        panelTitle.setPosition({ 862, 145 });
        m_window.draw(panelTitle);

        sf::Text idLabel(m_font, "Product ID", 16);
        idLabel.setPosition({ 835, 225 });
        m_window.draw(idLabel);
        m_removeIdBox.draw(m_window);

        if (!m_removeProductError.empty())
        {
            sf::Text errorText(m_font, m_removeProductError, 14);
            errorText.setPosition({ 815, 295 });
            errorText.setFillColor({ 230, 90, 90 });
            m_window.draw(errorText);
        }

        m_submitRemoveProductBtn.draw(m_window);
        m_closeOverlayBtn.draw(m_window);
    }
    m_window.display();
}
