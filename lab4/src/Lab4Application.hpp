#pragma once
#include "Application.hpp"


class Lab4Application : Application
{
public:
    explicit Lab4Application(std::unique_ptr<sf::RenderWindow> window);

    bool run();

private:
    bool running;
};
