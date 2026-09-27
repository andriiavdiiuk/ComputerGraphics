#pragma once

#include <vector>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Line.hpp"
#include "graphics/meshes/Mesh.hpp"

struct GridDrawData
{
    std::vector<Line> lines;
    std::vector<sf::Text> labels;
};

class Grid
{
public:
    glm::vec2 point_origin;
    float spacing_x;
    float spacing_y;
    float lines_x;
    float lines_y;

    float line_width;
    glm::vec4 color;
    sf::Font font;
    [[nodiscard]] GridDrawData toGridDrawData() const;
};
