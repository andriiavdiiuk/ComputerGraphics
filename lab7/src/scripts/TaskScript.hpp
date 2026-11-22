#pragma once

#include <glm/mat4x4.hpp>
#include <SFML/Window/Event.hpp>

#include "graphics/meshes/Mesh.hpp"
#include "Models/IfsFractal.hpp"
#include "transformations/Transformation.hpp"
#include "ScreenMovementScript.hpp"
#include "ui/Menu.hpp"

class TaskScript
{
private:
    sf::Shader shader;
    Mesh2D mesh;
    IfsFractalParams ifs;
    float zoom;
    glm::mat4 projectionView;
    ScreenMovementScript movementScript;
    MenuParams menuParams;

public:
    TaskScript(float aspectRatio);
    void event(const std::optional<sf::Event>& event);
    void run(float deltaTime);
    void render();
};
