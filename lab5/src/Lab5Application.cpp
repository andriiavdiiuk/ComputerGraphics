#include "Lab5Application.hpp"
#include "primitives/Grid.hpp"
#include <spdlog/spdlog.h>
#include <imgui-SFML.h>
#include <iostream>
#include <glm/glm.hpp>

#include <ui/Menu.hpp>
#include "scripts/TaskScript.hpp"



Lab5Application::Lab5Application(std::unique_ptr<sf::RenderWindow> window_ptr) :
    Application(std::move(window_ptr)),
    running(false)
{
}

bool Lab5Application::run()
{
    running = true;

    sf::Shader meshShader;

    if (!meshShader.loadFromFile(
        "./resources/shaders/mesh3d_shader.vert",
        "./resources/shaders/mesh3d_shader.frag"
    ))
    {
        SPDLOG_ERROR(std::format("Failed to load shader"));
    }
    float aspectRatio =
        static_cast<float>(window->getSize().x) /
        static_cast<float>(window->getSize().y);

    TaskScript script(aspectRatio);

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
        }

        script.run();

        ImGui::SFML::Update(*window, deltaTime);

        script.render(meshShader);

        ImGui::SFML::Render(*window);
        window->display();
    }

    return true;
}
