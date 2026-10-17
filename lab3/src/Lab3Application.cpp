#include "Lab3Application.hpp"
#include "primitives/Grid.hpp"
#include <spdlog/spdlog.h>
#include <imgui-SFML.h>
#include <iostream>
#include <glm/ext/scalar_constants.hpp>
#include "scripts/CurveScript.hpp"
#include "scripts/ScreenMovementScript.hpp"
#include "ui/Menu.hpp"

Lab3Application::Lab3Application(std::unique_ptr<sf::RenderWindow> window_ptr) :
    Application(std::move(window_ptr)),
    running(false)
{
}

bool Lab3Application::run()
{
    running = true;

    sf::Shader meshShader;

    if (!meshShader.loadFromFile(
        "./resources/shaders/mesh_shader.vert",
        "./resources/shaders/mesh_shader.frag"
    ))
    {
        SPDLOG_ERROR(std::format("Failed to load shader"));
    }

    CurveScript curveScript;
    MenuParams params {
        [&curveScript](const std::string& path)
        {
            curveScript.loadFile(path);
        },
        [&curveScript](const float weight_a, const float weight_b, const float weight_c, const float weight_d)
        {
            curveScript.updateWeights(weight_a, weight_b, weight_c, weight_d);
        },
        curveScript.renderControlPoints,
        curveScript.animate,
        curveScript.autoAnimate,
        curveScript.animationStep,
        curveScript.animationIndex,
        curveScript.euclideanTransformation,

    };

    sf::Clock deltaClock;

    while (running)
    {
        const float deltaTime = static_cast<float>(deltaClock.restart().asMilliseconds());
        window->clear(sf::Color(255, 255, 255, 255));

        while (const auto event = window->pollEvent())
        {
            ImGui::SFML::ProcessEvent(*window, *event);

            if (event->is<sf::Event::Closed>())
            {
                window->close();
            }

            ScreenMovementScript::event(*window, event);

            curveScript.event(event);
        }

        ScreenMovementScript::run(*window, deltaTime);
        curveScript.run(*window);

        ImGui::SFML::Update(*window, deltaClock.restart());

        drawMenu(params);
        curveScript.render(meshShader, *window);

        ImGui::SFML::Render(*window);
        window->display();
    }

    return true;
}
