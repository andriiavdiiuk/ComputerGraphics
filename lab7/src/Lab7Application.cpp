#include "Lab7Application.hpp"
#include "primitives/Grid.hpp"
#include <spdlog/spdlog.h>
#include <imgui-SFML.h>

#include "scripts/ScreenMovementScript.hpp"
#include "scripts/TaskScript.hpp"


Lab7Application::Lab7Application(std::unique_ptr<sf::RenderWindow> window_ptr) :
    Application(std::move(window_ptr)),
    running(false)
{
}

bool Lab7Application::run()
{
    running = true;

    sf::Shader meshShader;

    float aspectRatio =static_cast<float>(window->getSize().x) /static_cast<float>(window->getSize().y);

    TaskScript taskScript(aspectRatio);

    sf::Clock deltaClock;

    glEnable(GL_DEPTH_TEST);

    while (running)
    {
        const sf::Time deltaTime = deltaClock.restart();
        window->clear(sf::Color(255, 255, 255, 255));
        glClear(GL_DEPTH_BUFFER_BIT);
        while (const auto event = window->pollEvent())
        {
            ImGui::SFML::ProcessEvent(*window, *event);

            if (event->is<sf::Event::Closed>())
            {
                window->close();
            }

            taskScript.event(event);
        }

        taskScript.run(static_cast<float>(deltaTime.asMilliseconds()));

        ImGui::SFML::Update(*window, deltaTime);

        taskScript.render();

        ImGui::SFML::Render(*window);
        window->display();
    }

    return true;
}
