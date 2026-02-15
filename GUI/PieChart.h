#pragma once
#include <SFML/Graphics.hpp>

class PieChart {
public:
    void draw(sf::RenderWindow& win,
        sf::Vector2f center,
        float radius,
        float successRatio)
    {
        int points = 60;

        sf::ConvexShape successSlice;
        successSlice.setPointCount(points + 2);
        successSlice.setPoint(0, center);

        float angle = successRatio * 360.f;

        for (int i = 0; i <= points; ++i)
        {
            float a = (-90 + angle * (i / (float)points)) * 3.14159f / 180.f;
            successSlice.setPoint(i + 1,
                center + sf::Vector2f(std::cos(a) * radius,
                    std::sin(a) * radius));
        }

        successSlice.setFillColor({ 80,200,120 });

        sf::CircleShape failCircle(radius);
        failCircle.setFillColor({ 220,90,90 });
        failCircle.setOrigin({ radius,radius });
        failCircle.setPosition(center);

        win.draw(failCircle);
        win.draw(successSlice);
    }
};
