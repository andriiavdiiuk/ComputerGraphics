#include "Lab2Application.hpp"
#include "primitives/Grid.hpp"
#include <spdlog/spdlog.h>
#include <imgui-SFML.h>
#include <glm/ext/scalar_constants.hpp>
#include <glm/matrix.hpp>
#include "models/FoliumOfDescartes.hpp"
#include "transformations/Transformation.hpp"
#include "ui/Menu.hpp"


static void resizeView(sf::View& view, const sf::Vector2u size)
{
    view.setSize({
        static_cast<float>(size.x),
        -static_cast<float>(size.y)
    });

    view.setCenter({0.f, 0.f});
}

static void updateGridMeshes(const Grid& grid, Mesh& gridMesh, std::vector<sf::Text>& gridLabels)
{
    auto [lines, labels] = grid.toGridDrawData();
    gridMesh = generateMesh(lines);
    gridLabels = std::move(labels);

    gridMesh.upload();
}

static void updateFigureMesh(const FoliumOfDescartes& figure, FoliumOfDescartesDrawData& drawData)
{
    drawData = figure.toMesh();
    drawData.mesh.upload();
}

static void updateRotationPoint(const EuclideanTransformation& euclidean, Mesh& mesh)
{
    mesh = euclidean.getRotationPointMesh(0.1,0.1);
    mesh.upload();
}

static void updateCurvePoint(const FoliumOfDescartes& figure, const CurvePointParams& params, CurvePoint& curvePoint)
{
    curvePoint = figure.getCurvePointMeshes(params);
    curvePoint.pointMesh.upload();
    curvePoint.lineMeshes.upload();
}

Lab2Application::Lab2Application(std::unique_ptr<sf::RenderWindow> window_ptr) :
    Application(std::move(window_ptr)),
    running(false)
{
    sf::View view = window->getDefaultView();
    const auto size = window->getSize();
    resizeView(view, size);
    window->setView(view);
}

bool Lab2Application::run()
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

    AffineTransformation affine = AffineTransformation::Default;
    affine.scale = {50, 50};
    EuclideanTransformation euclidean = EuclideanTransformation::Default;

    auto sceneMatrix = glm::mat3(1);
    sf::Transform sfmlGridTransform;
    sf::Transform sfmlFigureTransform;
    sf::RenderStates renderStates;

    Grid grid = {
        .point_origin = {0, 0},
        .spacing_x = 1,
        .spacing_y = 1,
        .lines_x = 8,
        .lines_y = 8,
        .negative_lines_x = 8,
        .negative_lines_y = 8,
        .line_width = 0.05,
        .color = {0.f, 0.f, 0.f, 1.f},
        .font = font
    };
    CurvePointParams curvePointParams = {
        .length = 3,
    };
    CurvePoint curvePoint;
    Mesh rotationPointMesh;
    FoliumOfDescartes foliumOfDescartes = FoliumOfDescartes::Default;
    FoliumOfDescartesDrawData foliumOfDescartesData = foliumOfDescartes.toMesh();
    Mesh gridMesh;
    std::vector<sf::Text> gridLabels;


    updateGridMeshes(grid, gridMesh, gridLabels);
    updateFigureMesh(foliumOfDescartes, foliumOfDescartesData);
    updateRotationPoint(euclidean, rotationPointMesh);

    sf::Clock deltaClock;

    while (running)
    {
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
                view = window->getView();
                resizeView(view, resized->size);
                window->setView(view);
            }
        }
        ImGui::SFML::Update(*window, deltaClock.restart());

        sceneMatrix = affine.getMatrix() * euclidean.getMatrix();
        sfmlFigureTransform = toSfmlTransform(sceneMatrix);
        sfmlGridTransform = toSfmlTransform(affine.getMatrix());
        renderStates.transform = sfmlGridTransform;

        if (foliumOfDescartes.animation_enabled)
        {
            foliumOfDescartes.animateNext();
            updateFigureMesh(foliumOfDescartes, foliumOfDescartesData);
            updateCurvePoint(foliumOfDescartes, curvePointParams, curvePoint);
        }


        drawMenu(
            euclidean,
            foliumOfDescartes,
            foliumOfDescartesData,
            curvePointParams,
            curvePoint,
            [&foliumOfDescartes, &curvePointParams, &curvePoint]
            {
                updateCurvePoint(foliumOfDescartes, curvePointParams, curvePoint);
            },
            [&foliumOfDescartes, &foliumOfDescartesData,  &curvePointParams, &curvePoint]
            {
                updateFigureMesh(foliumOfDescartes, foliumOfDescartesData);
                updateCurvePoint(foliumOfDescartes, curvePointParams, curvePoint);
            },
            [&euclidean, &rotationPointMesh]
            {
                updateRotationPoint(euclidean, rotationPointMesh);
            }
        );


        gridMesh.render(meshShader, window->getView(), sfmlGridTransform);
        foliumOfDescartesData.mesh.render(meshShader, window->getView(), sfmlFigureTransform);
        rotationPointMesh.render(meshShader, window->getView(), sfmlGridTransform);
        curvePoint.pointMesh.render(meshShader, window->getView(), sfmlFigureTransform);
        curvePoint.lineMeshes.render(meshShader, window->getView(), sfmlFigureTransform);


        ImGui::SFML::Render(*window);
        window->display();
    }

    return true;
}
