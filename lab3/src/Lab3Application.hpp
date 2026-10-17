#pragma once
#include "Application.hpp"


class Lab3Application : Application
{
public:
    explicit Lab3Application(std::unique_ptr<sf::RenderWindow> window);

    bool run();

private:
    bool running;
};
