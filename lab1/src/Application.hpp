#pragma once
#include <memory>
#include <SFML/Graphics.hpp>

class Application
{
    public:
        Application(std::unique_ptr<sf::RenderWindow> window);
        ~Application();

        bool run();

    private:
        std::unique_ptr<sf::RenderWindow> window;
        bool running;
        sf::Font font;
};
