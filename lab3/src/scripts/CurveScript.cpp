#include "CurveScript.hpp"

#include <glm/glm.hpp>
#include <SFML/Window/Mouse.hpp>

#include "data/SvgParser.hpp"
#include "primitives/ThirdDegreeCurve.hpp"
#include "utils/GlmUtils.hpp"

static void handleCurveMesh(ThirdDegreeCurves& curves, ThirdDegreeCurveRenderData& mesh)
{
    mesh = generateMesh(curves);
    mesh.curveMesh.upload();
    mesh.controlPointLinesMesh.upload();
    mesh.controlPointsMesh.upload();
}

static bool handleCurveInteraction(
    ThirdDegreeCurvePath& curvePath,
    ThirdDegreeCurve& curve,
    ControlPoint& draggingPoint,
    const glm::vec2& mousePos,
    bool mousePressed,
    bool mouseReleased)
{
    auto& point_a = curvePath.points[curve.point_a];
    auto& point_b = curvePath.points[curve.point_b];
    auto& point_c = curvePath.points[curve.point_c];
    auto& point_d = curvePath.points[curve.point_d];

    if (mousePressed)
    {
        if (point_a.containsPoint(mousePos))
            draggingPoint = ControlPoint::A;
        else if (point_b.containsPoint(mousePos))
            draggingPoint = ControlPoint::B;
        else if (point_c.containsPoint(mousePos))
            draggingPoint = ControlPoint::C;
        else if (point_d.containsPoint(mousePos))
            draggingPoint = ControlPoint::D;
    }

    bool changed = false;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) &&
        draggingPoint != ControlPoint::None)
    {
        switch (draggingPoint)
        {
        case ControlPoint::A:
            point_a.origin = mousePos;
            changed = true;
            break;
        case ControlPoint::B:
            point_b.origin = mousePos;
            changed = true;
            break;
        case ControlPoint::C:
            point_c.origin = mousePos;
            changed = true;
            break;
        case ControlPoint::D:
            point_d.origin = mousePos;
            changed = true;
            break;
        default:
            break;
        }
    }

    if (mouseReleased)
        draggingPoint = ControlPoint::None;

    return changed;
}

static int isHoverCurvePoint(const ThirdDegreeCurvePath& curvePath, const ThirdDegreeCurve& curve, const glm::vec2& pos,
                             ControlPoint& draggingPoint)
{
    if (draggingPoint != ControlPoint::None)
        return -1;

    auto point_a = curvePath.points[curve.point_a];
    auto point_b = curvePath.points[curve.point_b];
    auto point_c = curvePath.points[curve.point_c];
    auto point_d = curvePath.points[curve.point_d];

    if (point_a.containsPoint(pos))
        draggingPoint = ControlPoint::A;
    else if (point_b.containsPoint(pos))
        draggingPoint = ControlPoint::B;
    else if (point_c.containsPoint(pos))
        draggingPoint = ControlPoint::C;
    else if (point_d.containsPoint(pos))
        draggingPoint = ControlPoint::D;
    else
        return 0;

    return -1;
}

struct CurveHit
{
    size_t pathIndex;
    size_t curveIndex;
};

static std::optional<CurveHit> isHoverCurvePoint(const ThirdDegreeCurves& curves, const glm::vec2& pos,
                                                 ControlPoint& draggingPoint)
{
    for (size_t p = 0; p < curves.paths.size(); ++p)
    {
        const auto& path = curves.paths[p];

        for (size_t c = 0; c < path.curves.size(); ++c)
        {
            const auto& curve = path.curves[c];

            if (isHoverCurvePoint(path, curve, pos, draggingPoint))
                return CurveHit{
                    p,
                    c
                };
        }
    }

    return std::nullopt;
}

CurveScript::CurveScript() :
    handCursor(sf::Cursor::Type::Hand),
    arrowCursor(sf::Cursor::Type::Arrow),
    mousePressed(false),
    mouseReleased(false),
    renderControlPoints(false),
    transform(sf::Transform::Identity)
{
}

void CurveScript::event(const std::optional<sf::Event>& event)
{
    if (event->is<sf::Event::MouseButtonPressed>())
    {
        const auto& e = event->getIf<sf::Event::MouseButtonPressed>();

        if (e->button == sf::Mouse::Button::Left)
            mousePressed = true;
    }

    if (event->is<sf::Event::MouseButtonReleased>())
    {
        const auto& e = event->getIf<sf::Event::MouseButtonReleased>();

        if (e->button == sf::Mouse::Button::Left)
            mouseReleased = true;
    }
}

void CurveScript::run(sf::RenderWindow& window)
{
    auto matrix = euclideanTransformation.getMatrix();
    transform = toSfmlTransform(matrix);
    auto inverse = glm::inverse(matrix);
    bool remesh = false;

    ThirdDegreeCurves& selected_curves =animate ? animationCurves : targetCurves;

    if (animate)
    {
        if (autoAnimate)
        {
            animationIndex += animationStep * animationDirection;

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
        }

        animationIndex = std::clamp(animationIndex, 0.0f, 1.0f);

        remesh = true;

        animateCurve(
            curves,
            targetCurves,
            animationCurves,
            animationIndex
        );
    }

    if (remesh)
    {
        handleCurveMesh(selected_curves, renderData);
    }


    if (!renderControlPoints) return;

    const auto mousePixel = sf::Mouse::getPosition(window);

    glm::vec2 pos = toGlmVec(
        window.mapPixelToCoords(mousePixel)
    );
    const glm::vec3 transformed = inverse * glm::vec3(pos, 1.0f);
    pos = glm::vec2(transformed) / transformed.z;

    auto index = isHoverCurvePoint(selected_curves, pos, draggingPoint);

    if (!index.has_value())
    {
        window.setMouseCursor(arrowCursor);
        return;
    }
    window.setMouseCursor(handCursor);

    auto& path = selected_curves.paths[index->pathIndex];
    auto& curve = path.curves[index->curveIndex];
    if (handleCurveInteraction(path,
                               curve, draggingPoint, pos,
                               mousePressed, mouseReleased))
    {
        handleCurveMesh(selected_curves, renderData);
    }
}

void CurveScript::render(sf::Shader& shader, sf::RenderWindow& window) const
{
    renderData.curveMesh.render(shader, window.getView(), transform);
    if (renderControlPoints)
    {
        renderData.controlPointLinesMesh.render(shader, window.getView(), transform);
        renderData.controlPointsMesh.render(shader, window.getView(), transform);
    }
}

void CurveScript::loadFile(const std::string& path)
{
    curves = std::move(parseSvg(path));
    handleCurveMesh(curves, renderData);
    animationCurves = curves;
    targetCurves = curves;
}

static void updateCurveWeights(ThirdDegreeCurves& curves, float weight_a, float weight_b, float weight_c, float weight_d)
{
    for (auto& path : curves.paths)
    {
        for (auto& curve : path.curves)
        {
            curve.weight_a = weight_a;
            curve.weight_b = weight_b;
            curve.weight_c = weight_c;
            curve.weight_d = weight_d;
        }
    }
}

void CurveScript::updateWeights(float weight_a, float weight_b, float weight_c, float weight_d)
{
    updateCurveWeights(curves, weight_a, weight_b, weight_c, weight_d);
    updateCurveWeights(targetCurves, weight_a, weight_b, weight_c, weight_d);
    updateCurveWeights(animationCurves, weight_a, weight_b, weight_c, weight_d);

    ThirdDegreeCurves& selected_curves = animate ? animationCurves : curves;
    handleCurveMesh(selected_curves, renderData);
}
