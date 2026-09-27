#include <glad/glad.h>
#include <SFML/OpenGL.hpp>
#include "Application.hpp"
#include <imgui.h>
#include "utils/FileUtils.hpp"
#include "models/Grid.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>
#include <imgui-SFML.h>
#include <iostream>

#include "models/Figure.hpp"
#include "transformations/Transformation.hpp"
#include "ui/Menu.hpp"


static void resizeView(sf::View& view, const sf::Vector2u size)
{
    view.setSize({
      static_cast<float>(size.x),
      -static_cast<float>(size.y)
  });

    view.setCenter({
        static_cast<float>(size.x) / 2.f - 50.f,
        static_cast<float>(size.y) / 2.f - 50.f,
    });
}

void updateFigureMeshes(const Figure& figure, std::vector<Mesh>& meshes)
{
    meshes = figure.toFigureDrawData().toMeshes();

    for (const auto& mesh : meshes)
    {
        mesh.upload();
    }
}

void updateGridMeshes(const Grid& grid, Mesh& gridMesh, std::vector<sf::Text>& gridLabels)
{
    auto [lines, labels] = grid.toGridDrawData();
    gridMesh = generateMesh(lines);
    gridLabels = std::move(labels);

    gridMesh.upload();
}


Application::Application(std::unique_ptr<sf::RenderWindow> window_ptr) :
    window(std::move(window_ptr)),
    running(false),
    font("./resources/fonts/OpenSans.ttf")
{

    const auto version = gladLoadGLLoader(
        reinterpret_cast<GLADloadproc>(sf::Context::getFunction)
    );
    IMGUI_CHECKVERSION();

    if (!ImGui::SFML::Init(*window))
    {
        SPDLOG_ERROR(std::format("Failed to initialize ImGui"));
        throw std::runtime_error("Failed to initialize ImGui");
    }

    ImGuiIO& io = ImGui::GetIO();

    io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF("./resources/fonts/OpenSans.ttf", 20.0f, nullptr, nullptr);

    io.Fonts->Build();


    ImGui::SFML::UpdateFontTexture();

    sf::View view = window->getDefaultView();
    const auto size = window->getSize();
    resizeView(view, size);
    window->setView(view);
}

Application::~Application()
{
    ImGui::SFML::Shutdown();
}

bool Application::run()
{
    auto view = window->getView();
    const auto size = window->getSize();
    resizeView(view, size);
    window->setView(view);

    running = true;
    sf::Shader meshShader;


    if (!meshShader.loadFromFile(
        "./resources/shaders/mesh_shader.vert",
        "./resources/shaders/mesh_shader.frag"
    ))
    {
        SPDLOG_ERROR(std::format("Failed to load shader"));
    }

    bool useAffine = false;
    bool useProjective = false;
    AffineTransformation affine = AffineTransformation::Default;
    EuclideanTransformation euclidean = EuclideanTransformation::Default;
    ProjectiveTransformation projective = ProjectiveTransformation::Default;
    auto sceneMatrix = glm::mat3(1);
    sf::Transform sfmlTransform;
    sf::Glsl::Mat3 sceneTransform = toSfmlMat3(sceneMatrix);
    sf::Glsl::Mat3 figureTransform = toSfmlMat3(sceneMatrix);
    sf::RenderStates renderStates;
    Mesh rotationPointMesh = euclidean.getRotationPointMesh();
    rotationPointMesh.upload();
    Grid grid = {
        .point_origin = {0, 0},
        .spacing_x = 50,
        .spacing_y = 50,
        .lines_x = 12,
        .lines_y = 12,
        .line_width = 1,
        .color = {0.f, 0.f, 0.f, 1.f},
        .font = font
    };
    auto figure = Figure::Default({100, 100}, 5);

    std::vector<Mesh> figureMeshes;
    Mesh gridMesh;
    std::vector<sf::Text> gridLabels;

    updateFigureMeshes(figure, figureMeshes);
    updateGridMeshes(grid, gridMesh, gridLabels);

    sf::Clock deltaClock;

    while (running)
    {
        if (useAffine)
            sceneMatrix = affine.getMatrix();

        if (useProjective)
            sceneMatrix = projective.getMatrix();

        sfmlTransform = toSfmlTransform(sceneMatrix);
        sceneTransform = toSfmlMat3(sceneMatrix);
        figureTransform = toSfmlMat3(sceneMatrix * euclidean.getMatrix());
        renderStates.transform = sfmlTransform;

        window->clear(sf::Color(255, 255, 255, 255));

        while (const auto event = window->pollEvent())
        {
            ImGui::SFML::ProcessEvent(*window, *event);

            if (event->is<sf::Event::Closed>())
            {
                window->close();
            }
            if (const auto* resized = event->getIf<sf::Event::Resized>())
            {
                auto view = window->getView();
                resizeView(view, resized->size);
                window->setView(view);
            }
        }
        ImGui::SFML::Update(*window, deltaClock.restart());

        drawMenu(projective, affine, euclidean, useProjective, useAffine, figure, [&figure, &figureMeshes]()
        {
            updateFigureMeshes(figure, figureMeshes);
        }, [&rotationPointMesh, &euclidean]()
        {
            rotationPointMesh = euclidean.getRotationPointMesh();
            rotationPointMesh.upload();
        });



        rotationPointMesh.render(meshShader, window->getView(), sceneTransform);
        gridMesh.render(meshShader, window->getView(), sceneTransform);

        for (const auto& mesh : figureMeshes)
        {
            mesh.render(meshShader, window->getView(), figureTransform);
        }

        sf::Shader::bind(nullptr);

        for (const auto& text : gridLabels)
        {
            window->draw(text, renderStates);
        }

        if (useProjective)
        {
            sf::CircleShape point(5.f);
            point.setOrigin({5.f, 5.f});
            point.setFillColor(sf::Color::Red);

            point.setPosition({projective.origin.x, projective.origin.y});
            window->draw(point);

            point.setPosition({projective.xAxisEnd.x, projective.xAxisEnd.y});
            window->draw(point);

            point.setPosition({projective.yAxisEnd.x, projective.yAxisEnd.y});
            window->draw(point);
        }
        ImGui::SFML::Render(*window);
        window->display();
    }

    return true;
}
