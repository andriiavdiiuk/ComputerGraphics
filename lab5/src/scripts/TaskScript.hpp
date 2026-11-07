#pragma once

#include <glm/mat4x4.hpp>
#include "ui/Menu.hpp"
#include "graphics/meshes/Mesh.hpp"
#include "transformations/Transformation.hpp"
#include "models/Figure.hpp"

class TaskScript
{
private:
    Figure figure;
    Mesh3D mesh;
    Mesh3D axisMeshes;
    glm::mat4 view;
    glm::mat4 projection;
    MenuParams menuParams;
    EuclideanTransformation3D euclideanTransformation;
    DimetricTransformation3D dimetricTransformation;

    EuclideanTransformation3D euclideanTransformationStart;
    EuclideanTransformation3D euclideanTransformationTarget;
    bool animate;
    float animationDirection = 1;
    float animationSpeed;
    float animationIndex;
public:
    TaskScript(float aspectRatio);
    void run();
    void render(sf::Shader& shader);
};
