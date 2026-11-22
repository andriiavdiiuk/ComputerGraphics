#include "TaskScript.hpp"

#include <iostream>
#include <vector>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <spdlog/spdlog.h>

#include "data/SvgParser.hpp"
#include "primitives/Line3D.hpp"
#include "utils/GlmColors.hpp"
#include "utils/GlmUtils.hpp"

static glm::mat4 makeProjectionView(float aspectRatio)
{
    if (aspectRatio >= 1.0f)
    {
        return glm::ortho(
            -aspectRatio, aspectRatio,
            -1.0f, 1.0f,
            -1.0f, 1.0f
        );
    }

    return glm::ortho(
        -1.0f, 1.0f,
        -1.0f / aspectRatio, 1.0f / aspectRatio,
        -1.0f, 1.0f
    );
}

constexpr std::array<IfsFractalEntry, 3> ifs_params = {
    IfsFractalEntry{.307692, -.531469, -.461538, -.293706, 5.401953, 8.655175, .40},
    IfsFractalEntry{.307692, -.076923, .153846, -.447552, -1.295248, 4.152990, .15},
    IfsFractalEntry{.000000, .545455, .692308, -.195804, -4.893637, 7.269794, .45}
};

constexpr std::array<glm::vec4, 3> ifs_colors{
    colors::RED,
    colors::GREEN,
    colors::BLUE,
};

static void generateFractal(IfsFractalParams& ifsParams, Mesh2D& mesh)
{
    mesh = generateMesh(ifsParams);
    mesh.upload();
}

TaskScript::TaskScript(float aspectRatio) :
    movementScript(0.1, 0.001),
    projectionView(makeProjectionView(aspectRatio)),
    zoom(1),
    ifs(std::vector(
            ifs_params.begin(),
            ifs_params.end()
        ), 1'000'000,
        std::vector(ifs_colors.begin(), ifs_colors.end()
        )
    ),
    menuParams(
        [this]()
        {
            generateFractal(this->ifs, this->mesh);
        },
        ifs
    )
{
    if (!shader.loadFromFile(
        "./lab7_resources/shaders/ifs_shader.vert",
        "./lab7_resources/shaders/ifs_shader.frag"
    ))
    {
        SPDLOG_ERROR(std::format("Failed to load shader"));
    }
    glEnable(GL_PROGRAM_POINT_SIZE);

    generateFractal(ifs, mesh);
}

void TaskScript::event(const std::optional<sf::Event>& event)
{
    movementScript.event(event);
}

void TaskScript::run(float deltaTime)
{
    movementScript.run(deltaTime);
}


void TaskScript::render()
{
    constexpr auto glm_one = glm::mat4(1.0f);
    const auto projectionViewSfml = toSfmlMat(movementScript.screenMatrix * projectionView);
    const auto modelSfml = toSfmlMat(glm_one);

    mesh.setupShader(shader, projectionViewSfml, modelSfml);
    shader.setUniform("pointSize", 1.0f);
    shader.setUniform("zoomLevel", movementScript.zoomLevel);
    mesh.render();
    drawMenu(menuParams);
}
