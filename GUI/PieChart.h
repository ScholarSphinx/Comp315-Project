#pragma once
#include <SFML/Graphics.hpp>

class PieChart {
public:
	void draw(sf::RenderWindow& win, //Drawing a pie chart to represent the success ratio of orders, using SFML's graphics capabilities to create a visual representation of the success and failure rates, allowing users to quickly understand the performance of the order processing system through a graphical interface
        sf::Vector2f center,
        float radius,
        float successRatio)
    {
		int points = 60; //Number of points to approximate the pie slice, providing a smoother appearance for the pie chart by increasing the number of points used to define the shape of the success slice, ensuring that the visual representation of the success ratio is more accurate and visually appealing

        sf::ConvexShape successSlice;
        successSlice.setPointCount(points + 2);
		successSlice.setPoint(0, center); //Setting the first point of the success slice to the center of the pie chart, allowing the slice to be drawn as a sector originating from the center, ensuring that the pie chart visually represents the proportion of successful orders relative to the total number of orders by filling in the area corresponding to the success ratio

		float angle = successRatio * 360.f; //Calculating the angle for the success slice based on the success ratio, allowing the pie chart to visually represent the proportion of successful orders relative to the total number of orders, ensuring that users can easily interpret the success ratio through the size of the success slice in the pie chart

        for (int i = 0; i <= points; ++i)
        {
			float a = (-90 + angle * (i / (float)points)) * 3.14159f / 180.f; //Calculating the angle for each point of the success slice, starting from -90 degrees (the top of the circle) and progressing clockwise based on the success ratio, allowing the pie chart to visually represent the proportion of successful orders by filling in the area corresponding to the success ratio, ensuring that users can easily interpret the success ratio through the size of the success slice in the pie chart
            successSlice.setPoint(i + 1,
				center + sf::Vector2f(std::cos(a) * radius, //Calculating the position of each point on the circumference of the pie chart for the success slice, using cosine and sine functions to determine the x and y coordinates based on the angle and radius, allowing the pie chart to visually represent the proportion of successful orders by filling in the area corresponding to the success ratio, ensuring that users can easily interpret the success ratio through the size of the success slice in the pie chart
                    std::sin(a) * radius));
        }

		successSlice.setFillColor({ 80,200,120 }); //Setting the fill color of the success slice to a greenish color, visually representing successful orders in the pie chart, allowing users to quickly identify the portion of the pie chart that corresponds to successful orders, ensuring that the visual representation of the success ratio is clear and easily distinguishable from the failure portion of the pie chart

		sf::CircleShape failCircle(radius); //Creating a circle shape to represent the failure portion of the pie chart, using a simple circle to visually represent the proportion of failed orders by filling in the area corresponding to the failure ratio, allowing users to quickly identify the portion of the pie chart that corresponds to failed orders, ensuring that the visual representation of the success ratio is clear and easily distinguishable from the failure portion of the pie chart
        failCircle.setFillColor({ 220,90,90 });
        failCircle.setOrigin({ radius,radius });
        failCircle.setPosition(center);

        win.draw(failCircle);
        win.draw(successSlice);
    }
};
