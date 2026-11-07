#pragma once
#include "Application.hpp"


class Lab5Application : Application
{
public:
    explicit Lab5Application(std::unique_ptr<sf::RenderWindow> window);

    bool run();

private:
    bool running;
};
