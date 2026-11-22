#pragma once
#include <optional>
#include <glm/mat4x4.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>


class ScreenMovementScript
{
public:
    float zoomSpeed{0};
    float movementSpeed{0};
    float zoomLevel{1};
    glm::mat4 screenMatrix{1};
    void event(const std::optional<sf::Event>& event);
    void run(float deltaTime);

};
