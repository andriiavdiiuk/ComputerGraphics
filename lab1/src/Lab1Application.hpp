#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "Application.hpp"

class Lab1Application : Application
{
    public:
        explicit Lab1Application(std::unique_ptr<sf::RenderWindow> window);

        bool run();

    private:
        bool running;
};
