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

constexpr std::array<Line3D, 3> AXIS_LINES{
    Line3D({-1000, 0, 0}, {1000, 0, 0}, 0.02, colors::RED),
    Line3D({0, -1000, 0}, {0, 1000, 0}, 0.02, colors::GREEN),
    Line3D({0, 0, -1000}, {0, 0, 1000}, 0.02, colors::BLUE),
};

static void updateFigure(Mesh3D& mesh, const GarlicFigure& figure)
{
    mesh = generateMesh(figure.toLines());
    mesh.upload();
}

static void handleCurveMesh(const ThirdDegreeCurves& curves, ThirdDegreeCurveRenderData& data)
{
    data = generateMesh(curves);
    data.curveMesh.upload();
}

static void loadFile(ThirdDegreeCurves& curves, ThirdDegreeCurveRenderData& data, const std::string& path)
{
    curves = std::move(parseSvg(path));
    handleCurveMesh(curves, data);
}


TaskScript::TaskScript(float aspectRatio) :
    figure(3, glm::radians(0.0f), glm::radians(360.0f), 0.1,
           glm::radians(-90.0f), glm::radians(90.0f), 0.1,
           0.01, colors::BLACK),
    mesh(generateMesh(figure.toLines())),
    axisMeshes(generateMesh(std::vector<Line3D>(
        AXIS_LINES.begin(),
        AXIS_LINES.end()
    ))),
    animate(false),
    animationSpeed(0.01f),
    animationIndex(0),
    curveAnimate(false),
    curveAnimationSpeed(0.01f),
    curveAnimationIndex(0),
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
    scaleFactor( glm::pi<float>() / 60 * 0.05),
    menuParams(
        [this](const std::string& path) { loadFile(this->thirdDegreeCurves, this->curveRenderData, path); },
        euclideanTransformationCurve,
        euclideanTransformationCurveStart,
        euclideanTransformationCurveTarget,
        curveAnimate,
        curveAnimationSpeed,
        scaleFactor,
        euclideanTransformation,
        dimetricTransformation,
        figure,
        [this]() { updateFigure(this->mesh, this->figure); },
        euclideanTransformationStart,
        euclideanTransformationTarget,
        animate,
        animationSpeed
    )

{
    if (!meshShader.loadFromFile(
        "./resources/shaders/mesh3d_shader.vert",
        "./resources/shaders/mesh3d_shader.frag"
    ))
    {
        SPDLOG_ERROR(std::format("Failed to load shader"));
    }

    if (!mappingShader.loadFromFile(
        "./lab6_resources/shaders/garlic_shader.vert",
        "./lab6_resources/shaders/garlic_shader.frag"
    ))
    {
        SPDLOG_ERROR(std::format("Failed to load shader"));
    }

    mesh.upload();
    axisMeshes.upload();
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

    if (curveAnimate)
    {
        curveAnimationIndex += curveAnimationSpeed * curveAnimationDirection;

        if (curveAnimationIndex >= 1.0f)
        {
            curveAnimationIndex = 1.0f;
            curveAnimationDirection = -1.0f;
        }
        else if (curveAnimationIndex <= 0.0f)
        {
            curveAnimationIndex = 0.0f;
            curveAnimationDirection = 1.0f;
        }

        euclideanTransformationCurve.offset = glm::mix(
            euclideanTransformationCurveStart.offset,
            euclideanTransformationCurveTarget.offset,
            curveAnimationIndex
        );

        euclideanTransformationCurve.rotationDegrees = glm::mix(
            euclideanTransformationCurveStart.rotationDegrees,
            euclideanTransformationCurveTarget.rotationDegrees,
            curveAnimationIndex
        );

        euclideanTransformationCurve.rotationPoint = glm::mix(
            euclideanTransformationCurveStart.rotationPoint,
            euclideanTransformationCurveStart.rotationPoint,
            curveAnimationIndex
        );
    }
}


void TaskScript::render()
{
    constexpr auto glm_one = glm::mat4(1.0f);
    const auto model = euclideanTransformation.getMatrix();
    const auto projectionView = projection * view * dimetricTransformation.getMatrix();
    const auto projectionViewSfml = toSfmlMat(projectionView);
    const auto modelSfml = toSfmlMat(model);

    axisMeshes.render(meshShader, projectionViewSfml, toSfmlMat(glm_one));
    mesh.render(meshShader, projectionViewSfml, modelSfml);

    const auto curveModel = euclideanTransformationCurve.getMatrix();
    curveRenderData.curveMesh.setupShader(mappingShader, projectionViewSfml, modelSfml);
    mappingShader.setUniform("r", figure.r);
    mappingShader.setUniform("scale", scaleFactor);
    mappingShader.setUniform("localModel", toSfmlMat(curveModel));
    curveRenderData.curveMesh.render();
    drawMenu(menuParams);
}
