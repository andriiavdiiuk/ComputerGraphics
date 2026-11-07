#pragma once
#include <vector>

#include "models/Figure.hpp"
#include "primitives/ThirdDegreeCurve.hpp"
#include "transformations/Transformation.hpp"

struct MenuParams
{
    EuclideanTransformation3D& euclideanTransformation;
    DimetricTransformation3D& dimetricTransformation;
    Figure& figure;
    std::function<void()> figureUpdated;
    EuclideanTransformation3D& euclideanTransformationStart;
    EuclideanTransformation3D& euclideanTransformationTarget;
    bool& animate;
    float& animationSpeed;
};

void drawMenu(MenuParams& params);
