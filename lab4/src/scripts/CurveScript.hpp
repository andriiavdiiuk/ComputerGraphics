#pragma once
#include <glm/glm.hpp>
#include <primitives/ThirdDegreeCurve.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Cursor.hpp>

#include "transformations/Transformation.hpp"

enum ControlPoint
{
    None,
    A,
    B,
    C,
    D,
};

class CurveScript
{
    ControlPoint draggingPoint = ControlPoint::None;
    ThirdDegreeCurves curves;
    ThirdDegreeCurves targetCurves;
    ThirdDegreeCurves animationCurves;
    ThirdDegreeCurveRenderData renderData;
    sf::Cursor handCursor;
    sf::Cursor arrowCursor;
    sf::Transform transform;
    float animationDirection = 1;
    bool mousePressed;
    bool mouseReleased;


public:
    bool renderControlPoints;
    float animationStep = 0.05;
    float animationIndex = 0;
    bool animate = false;
    bool autoAnimate = false;
    bool changeJointType;

    EuclideanTransformation euclideanTransformation;

    CurveScript();
    void event(const std::optional<sf::Event>& event);
    void run(sf::RenderWindow& window);
    void render(sf::Shader& shader, sf::RenderWindow& window) const;
    void loadFile(const std::string& path);
    void updateWeights(float weight_a, float weight_b, float weight_c, float weight_d);
};

