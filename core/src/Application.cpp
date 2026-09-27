#include <glad/glad.h>
#include <SFML/OpenGL.hpp>
#include "Application.hpp"
#include <imgui.h>
#include "utils/FileUtils.hpp"
#include "primitives/Grid.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>
#include <imgui-SFML.h>
#include <iostream>

Application::Application(std::unique_ptr<sf::RenderWindow> window_ptr) :
    window(std::move(window_ptr)),
    font("./resources/fonts/OpenSans.ttf")
{

    if (!gladLoadGLLoader(
        reinterpret_cast<GLADloadproc>(sf::Context::getFunction)
    ))
    {
        SPDLOG_ERROR(std::format("Failed to initialize OpenGl loader"));
        throw std::runtime_error("Failed to initialize ImGui");
    }
    IMGUI_CHECKVERSION();

    if (!ImGui::SFML::Init(*window))
    {
        SPDLOG_ERROR(std::format("Failed to initialize ImGui"));
        throw std::runtime_error("Failed to initialize ImGui");
    }

    const ImGuiIO& io = ImGui::GetIO();

    io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF("./resources/fonts/OpenSans.ttf", 20.0f, nullptr, nullptr);

    io.Fonts->Build();


    if (!ImGui::SFML::UpdateFontTexture())
    {
        SPDLOG_ERROR(std::format("Failed to update SFML font texture"));
        throw std::runtime_error("Failed to update SFML font textur");
    }
}

Application::~Application()
{
    ImGui::SFML::Shutdown();
}