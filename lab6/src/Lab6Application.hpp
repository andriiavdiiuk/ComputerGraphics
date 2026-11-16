#pragma once
#include "Application.hpp"


class Lab6Application : Application
{
public:
    explicit Lab6Application(std::unique_ptr<sf::RenderWindow> window);

    bool run();

private:
    bool running;
};
