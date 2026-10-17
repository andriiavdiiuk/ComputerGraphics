#pragma once
#include <vector>

#include "primitives/ThirdDegreeCurve.hpp"
#include "transformations/Transformation.hpp"

struct MenuParams
{
    std::function<void(const std::string&)> onLoadFile;
    std::function<void(float, float, float, float)> onWeightsUpdate;
    bool& renderControlPoints;
    bool& toggleAnimation;
    bool& toggleAutoAnimation;
    float& animation_step;
    float& animation_index;
    EuclideanTransformation& euclideanTransformation;

    float weight_a = 1;
    float weight_b = 1;
    float weight_c = 1;
    float weight_d = 1;
};

void drawMenu(MenuParams& params);
