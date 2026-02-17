#include "GUI/Application.h"

int main()
{
	Application app; //Creating an instance of the Application class, which initializes the application and sets up the necessary components for running the application, ensuring that we have a ready-to-use application object to start the main loop and handle user interactions
    app.run();
	return 0; //Starting the main loop of the application by calling the run method, which will keep the application running and responsive to user interactions until the user decides to close the application, ensuring that the application remains active and functional while it is in use
}
