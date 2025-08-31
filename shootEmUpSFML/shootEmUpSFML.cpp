#include "SFML/Graphics.hpp"
#include<optional>

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 800 }), "Window Title");
    sf::CircleShape shape(200.f, 100);
    shape.setFillColor(sf::Color(204, 77, 5));
    shape.setPosition({ 200, 200 });

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())

        {

            if (event->is<sf::Event::Closed>())

            {

                window.close();

            }

        }

        window.clear(sf::Color(18, 33, 43)); 
        window.draw(shape);
        window.display();
    }

    return 0;
}