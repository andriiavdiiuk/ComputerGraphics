#include "Figure.hpp"
#include "Line.hpp"

constexpr Figure default_figure = {
    .center_point = {32, 35},
    .circle_a_point = {12, 60},
    .circle_b_point = {52, 60},
    .circle_c_point = {32, 35},
    .circle_a_radius = 5,
    .circle_b_radius = 5,
    .circle_c_radius = 18,
    .point_a = {0, 10},
    .point_b = {0, 70},
    .point_c = {64, 70},
    .point_d = {64, 10},
    .point_e = {42, 10},
    .point_f = {42, 0},
    .point_g = {20, 0},
    .point_h ={20, 10},
    .color = {0.f, 0.f, 0.f, 1.f},
    .line_width = 4
};

Figure Figure::Default(const glm::vec2& origin, const float scale)
{
    Figure figure = default_figure;
    figure.center_point *= scale;

    figure.circle_a_point *= scale;
    figure.circle_b_point *= scale;
    figure.circle_c_point *= scale;

    figure.circle_a_radius *= scale;
    figure.circle_b_radius *= scale;
    figure.circle_c_radius *= scale;

    figure.point_a *= scale;
    figure.point_b *= scale;
    figure.point_c *= scale;
    figure.point_d *= scale;
    figure.point_e *= scale;
    figure.point_f *= scale;
    figure.point_g *= scale;
    figure.point_h *= scale;

    figure.center_point += origin;
    figure.circle_a_point += origin;
    figure.circle_b_point += origin;
    figure.circle_c_point += origin;

    figure.point_a += origin;
    figure.point_b += origin;
    figure.point_c += origin;
    figure.point_d += origin;
    figure.point_e += origin;
    figure.point_f += origin;
    figure.point_g += origin;
    figure.point_h += origin;

    return figure;
}

FigureDrawData Figure::toFigureDrawData() const
{
    const FigureDrawData data{
        .circle_a = {
            .origin = circle_a_point,
            .radius = circle_a_radius,
            .width = line_width,
            .segments = 32,
            .color = color
        },
        .circle_b = {
            .origin = circle_b_point,
            .radius = circle_b_radius,
            .width = line_width,
            .segments = 32,
            .color = color
        },
        .circle_c = {
            .origin = circle_c_point,
            .radius = circle_c_radius,
            .width = line_width,
            .segments = 32,
            .color = color
        },

        .line_ab = {
            point_a,
            point_b,
            line_width,
            color
        },
        .line_bc = {
            point_b,
            point_c,
            line_width,
            color
        },
        .line_cd = {
            point_c,
            point_d,
            line_width,
            color
        },
        .line_de = {
            point_d,
            point_e,
            line_width,
            color
        },
        .line_ef = {
            point_e,
            point_f,
            line_width,
            color
        },
        .line_fg = {
            point_f,
            point_g,
            line_width,
            color
        },
        .line_gh = {
            point_g,
            point_h,
            line_width,
            color
        },
        .line_ha = {
            point_h,
            point_a,
            line_width,
            color
        },
    };

    return data;
}


std::vector<Mesh> FigureDrawData::toMeshes() const
{
    std::vector<Mesh> meshes;
    meshes.reserve(11);

    meshes.emplace_back(generateMesh(circle_a));
    meshes.emplace_back(generateMesh(circle_b));
    meshes.emplace_back(generateMesh(circle_c));

    meshes.emplace_back(generateMesh(line_ab));
    meshes.emplace_back(generateMesh(line_bc));
    meshes.emplace_back(generateMesh(line_cd));
    meshes.emplace_back(generateMesh(line_de));
    meshes.emplace_back(generateMesh(line_ef));
    meshes.emplace_back(generateMesh(line_fg));
    meshes.emplace_back(generateMesh(line_gh));
    meshes.emplace_back(generateMesh(line_ha));

    return meshes;
}