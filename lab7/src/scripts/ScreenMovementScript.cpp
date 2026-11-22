#include "ScreenMovementScript.hpp"

#include <glm/ext/matrix_transform.hpp>

void ScreenMovementScript::event(const std::optional<sf::Event>& event)
{
    if (const auto* wheel = event->getIf<sf::Event::MouseWheelScrolled>())
    {
        if (wheel->wheel == sf::Mouse::Wheel::Vertical)
        {
            float zoomFactor = 1.f + wheel->delta * zoomSpeed;
            zoomLevel *= zoomFactor;
            screenMatrix = glm::scale(screenMatrix,glm::vec3(zoomFactor, zoomFactor, 1.f));
        }
    }
}

void ScreenMovementScript::run(float deltaTime)
{
    glm::vec2 movement(0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        movement.y -= movementSpeed * deltaTime;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        movement.y += movementSpeed * deltaTime;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        movement.x -= movementSpeed * deltaTime;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        movement.x += movementSpeed * deltaTime;

    float zoom = glm::length(glm::vec2(
        screenMatrix[0][0],
        screenMatrix[0][1]
    ));

    movement /= zoom;

    screenMatrix = glm::translate(
        screenMatrix,
        glm::vec3(movement, 0.f)
    );
}