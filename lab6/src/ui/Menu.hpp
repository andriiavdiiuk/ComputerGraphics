#pragma once
#include <vector>

#include "models/GarlicFigure.hpp"
#include "primitives/ThirdDegreeCurve.hpp"
#include "transformations/Transformation.hpp"

struct MenuParams
{
    std::function<void(const std::string&)> onLoadFile;
    EuclideanTransformation& euclideanTransformationCurve;
    EuclideanTransformation& euclideanTransformationCurveStart;
    EuclideanTransformation& euclideanTransformationCurveTarget;
    bool& curveAnimate;
    float& curveAnimationSpeed;
    float& curveScaleFactor;
    EuclideanTransformation3D& euclideanTransformation;
    DimetricTransformation3D& dimetricTransformation;
    GarlicFigure& figure;
    std::function<void()> figureUpdated;
    EuclideanTransformation3D& euclideanTransformationStart;
    EuclideanTransformation3D& euclideanTransformationTarget;
    bool& animate;
    float& animationSpeed;
};

void drawMenu(MenuParams& params);
