#pragma once
#include <memory>
#include <SFML/Graphics.hpp>

class Application
{
    public:
        explicit Application(std::unique_ptr<sf::RenderWindow> window);
        virtual ~Application();

    protected:
        std::unique_ptr<sf::RenderWindow> window;
        sf::Font font;
};
