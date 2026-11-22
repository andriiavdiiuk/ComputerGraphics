#pragma once
#include "Application.hpp"


class Lab7Application : Application
{
public:
    explicit Lab7Application(std::unique_ptr<sf::RenderWindow> window);

    bool run();

private:
    bool running;
};
