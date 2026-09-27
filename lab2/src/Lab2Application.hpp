#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "Application.hpp"

class Lab2Application : Application
{
    public:
        explicit Lab2Application(std::unique_ptr<sf::RenderWindow> window);

        bool run();

    private:
        bool running;
};
