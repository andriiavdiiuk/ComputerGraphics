#pragma once
#include <optional>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>


class ScreenMovementScript
{
public:
    static void event(sf::RenderWindow& window, const std::optional<sf::Event>& event);
    static void run(sf::RenderWindow& window, float deltaTime);
};
