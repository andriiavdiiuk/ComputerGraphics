#pragma once
#include "models/FoliumOfDescartes.hpp"
#include "transformations/Transformation.hpp"

void drawMenu(
    EuclideanTransformation& euclidean,
    FoliumOfDescartes& foliumOfDescartes,
    const FoliumOfDescartesDrawData& drawData,
    CurvePointParams& curvePointParams,
    const CurvePoint& curvePoint,
    const std::function<void()>& onCurvePointUpdated,
    const std::function<void()>& onFigureUpdated,
    const std::function<void()>& onEuclideanUpdated
);
