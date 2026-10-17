#include "ScreenMovementScript.hpp"

static void setZoom(sf::RenderWindow& window, float zoomFactor)
{
    auto view = window.getView();

    view.zoom(zoomFactor);

    window.setView(view);
}

void ScreenMovementScript::event(sf::RenderWindow& window, const std::optional<sf::Event>& event)
{
    if (const auto* wheel = event->getIf<sf::Event::MouseWheelScrolled>())
    {
        if (wheel->wheel == sf::Mouse::Wheel::Vertical)
        {
            setZoom(window, 1.f - wheel->delta * 0.1f);
        }
    }
}

void ScreenMovementScript::run(sf::RenderWindow& window, float deltaTime)
{
    constexpr float moveSpeed = 1.f;

    sf::Vector2f movement{0.f, 0.f};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        movement.y -= moveSpeed * deltaTime;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        movement.y += moveSpeed * deltaTime;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        movement.x -= moveSpeed * deltaTime;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        movement.x += moveSpeed * deltaTime;
    auto view = window.getView();
    view.move(movement);
    window.setView(view);
}
