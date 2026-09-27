#pragma once
#include "primitives/Circle.hpp"
#include "primitives/Line.hpp"

struct FigureDrawData
{
    Circle circle_a;
    Circle circle_b;
    Circle circle_c;

    Line line_ab;
    Line line_bc;
    Line line_cd;
    Line line_de;
    Line line_ef;
    Line line_fg;
    Line line_gh;
    Line line_ha;

    [[nodiscard]] std::vector<Mesh> toMeshes() const;
};

struct Figure
{
    glm::vec2 center_point;

    glm::vec2 circle_a_point;
    glm::vec2 circle_b_point;
    glm::vec2 circle_c_point;

    float circle_a_radius{};
    float circle_b_radius{};
    float circle_c_radius{};

    glm::vec2 point_a;
    glm::vec2 point_b;
    glm::vec2 point_c;
    glm::vec2 point_d;
    glm::vec2 point_e;
    glm::vec2 point_f;
    glm::vec2 point_g;
    glm::vec2 point_h;

    glm::vec4 color;
    float line_width;
    static Figure Default(const glm::vec2& origin, const float scale);
    [[nodiscard]] FigureDrawData toFigureDrawData() const;
};

