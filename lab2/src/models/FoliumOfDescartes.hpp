#pragma once
#include "graphics/meshes/Mesh.hpp"

struct CurvePointParams
{
    float angle;
    bool includeTangent;
    bool includeNormal;
    float length;
};

struct CurvePoint
{
    glm::vec2 point{0,0};
    double curvatureRadius = 0;
    Mesh pointMesh;
    Mesh lineMeshes;
};

struct FoliumOfDescartesDrawData
{
    float arcLength;
    float loopArea;
    Mesh mesh;
};

struct FoliumOfDescartes
{
    float a;
    float angle_start;
    float angle_end;
    float angle_step;
    float line_width;
    glm::vec4 color;

    float a_start;
    float a_end;
    float a_step;
    float a_direction;
    bool animation_enabled;
    bool include_asymptote;
    float asymptote_length;

    static FoliumOfDescartes Default;
    [[nodiscard]] glm::vec2 evaluate(float angle) const;
    [[nodiscard]] FoliumOfDescartesDrawData toMesh() const;
    [[nodiscard]] CurvePoint getCurvePointMeshes(CurvePointParams params) const;
    void animateNext();
};
