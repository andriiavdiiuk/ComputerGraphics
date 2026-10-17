#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/ContextSettings.hpp>

#include "Lab3Application.hpp"
#include "data/SvgParser.hpp"

int main()
{
    sf::ContextSettings settings;
    settings.majorVersion = 4;
    settings.minorVersion = 6;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.attributeFlags = sf::ContextSettings::Attribute::Debug;
    settings.antiAliasingLevel = 8;

    auto window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode({1280, 720}),
        "Computer Graphics lab 3",
        sf::Style::Default,
        sf::State::Windowed,
        settings
    );

    auto app = Lab3Application(std::move(window));
    return app.run();
}
