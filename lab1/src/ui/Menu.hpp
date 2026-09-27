#pragma once
#include "models/Figure.hpp"
#include "primitives/Grid.hpp"
#include "transformations/Transformation.hpp"

void drawMenu(
    ProjectiveTransformation& projective,
    AffineTransformation& affine,
    EuclideanTransformation& euclidean,
    bool& useProjective,
    bool& useAffine,
    Figure& figure,
    const std::function<void()>& onFigureUpdated,
    const std::function<void()>& onEuclideanUpdated
);
