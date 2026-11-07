#include "TaskScript.hpp"

#include <iostream>
#include <vector>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#include "primitives/Line3D.hpp"
#include "utils/GlmColors.hpp"
#include "utils/GlmUtils.hpp"

constexpr std::array<Line3D, 3> AXIS_LINES{
    Line3D({-1000, 0, 0}, {1000, 0, 0}, 0.02, colors::RED),
    Line3D({0, -1000, 0}, {0, 1000, 0}, 0.02, colors::GREEN),
    Line3D({0, 0, -1000}, {0, 0, 1000}, 0.02, colors::BLUE),
};

static void updateFigure(Mesh3D& mesh, Figure& figure)
{
    mesh = generateMesh(figure.toLines());
    mesh.upload();
}

TaskScript::TaskScript(float aspectRatio) :
    figure(glm::vec3(0.0f, 0.0f, 0.0f),
           2.0f,
           1.0f,
           2.0f,
           0.02f,
           glm::vec4(colors::BLACK)),
    mesh(generateMesh(figure.toLines())),
    axisMeshes(generateMesh(std::vector<Line3D>(
        AXIS_LINES.begin(),
        AXIS_LINES.end()
    ))),
    animate(false),
    animationSpeed(0.01f),
    animationIndex(0),
    projection(glm::ortho(
        -5.0f * aspectRatio,
        5.0f * aspectRatio,
        -5.0f,
        5.0f,
        0.1f,
        1000.0f
    )),
    view(glm::lookAt(
        glm::vec3(0.0f, 3.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.f, 0.f, 1.f)
    )),
    menuParams(euclideanTransformation,
               dimetricTransformation,
               figure,
               [this]() { updateFigure(this->mesh, this->figure); },
               euclideanTransformationStart,
               euclideanTransformationTarget,
               animate,
               animationSpeed
    )

{
    mesh.upload();
    axisMeshes.upload();
    glm::vec3 objectPosition(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraPosition(30.0f, 30.0f, 30.0f);
}

void TaskScript::run()
{
    if (animate)
    {
        animationIndex += animationSpeed * animationDirection;

        if (animationIndex >= 1.0f)
        {
            animationIndex = 1.0f;
            animationDirection = -1.0f;
        }
        else if (animationIndex <= 0.0f)
        {
            animationIndex = 0.0f;
            animationDirection = 1.0f;
        }

        euclideanTransformation.offset = glm::mix(
            euclideanTransformationStart.offset,
            euclideanTransformationTarget.offset,
            animationIndex
        );

        euclideanTransformation.rotationDegrees = glm::mix(
            euclideanTransformationStart.rotationDegrees,
            euclideanTransformationTarget.rotationDegrees,
            animationIndex
        );

        euclideanTransformation.rotationPoint = glm::mix(
            euclideanTransformationStart.rotationPoint,
            euclideanTransformationTarget.rotationPoint,
            animationIndex
        );
    }
}

void TaskScript::render(sf::Shader& shader)
{
    constexpr auto glm_one = glm::mat4(1.0f);
    const auto model = euclideanTransformation.getMatrix();
    const auto projectionView = projection * view * dimetricTransformation.getMatrix();
    const auto projectionViewSfml = toSfmlMat(projectionView);
    const auto modelSfml = toSfmlMat(model);

    axisMeshes.render(shader, projectionViewSfml, toSfmlMat(glm_one));
    mesh.render(shader, projectionViewSfml, modelSfml);
    drawMenu(menuParams);
}
