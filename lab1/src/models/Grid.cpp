#include <models/Grid.hpp>

static void addGridLines(
    GridDrawData& data,
    const Grid& grid,
    const glm::vec2& point_x_end,
    const glm::vec2& point_y_end
)
{
    // Vertical lines
    for (float i = 0; i < grid.lines_x; i++)
    {
        const float x = grid.point_origin.x + i * grid.spacing_x;

        data.lines.push_back({
            {x, grid.point_origin.y},
            {x, point_y_end.y},
            grid.line_width,
            grid.color
        });
    }

    // Horizontal lines
    for (float i = 0; i < grid.lines_y; i++)
    {
        const float y = grid.point_origin.y + i * grid.spacing_y;

        data.lines.push_back({
            {grid.point_origin.x, y},
            {point_x_end.x, y},
            grid.line_width,
            grid.color
        });
    }
}

static void addAxes(
    GridDrawData& data,
    const Grid& grid,
    const glm::vec2& x_end,
    const glm::vec2& y_end
)
{
    const float axis_width = grid.line_width ;

    // X axis
    data.lines.emplace_back(
        grid.point_origin,
        x_end,
        axis_width,
        grid.color
    );

    const float arrow_x = grid.spacing_x * 0.25f;

    data.lines.push_back({
        x_end,
        {
            x_end.x - arrow_x,
            x_end.y + arrow_x
        },
        axis_width,
        grid.color
    });

    data.lines.push_back({
        x_end,
        {
            x_end.x - arrow_x,
            x_end.y - arrow_x
        },
        axis_width,
        grid.color
    });

    // Y axis
    data.lines.emplace_back(
        grid.point_origin,
        y_end,
        axis_width,
        grid.color
    );

    const float arrow_y = grid.spacing_y * 0.25f;

    data.lines.push_back({
        y_end,
        {
            y_end.x - arrow_y,
            y_end.y - arrow_y
        },
        axis_width,
        grid.color
    });

    data.lines.push_back({
        y_end,
        {
            y_end.x + arrow_y,
            y_end.y - arrow_y
        },
        axis_width,
        grid.color
    });
}

static void addLabels(
    GridDrawData& data,
    const Grid& grid,
    const sf::Font& font,
    const glm::vec2& x_end,
    const glm::vec2& y_end
)
{
    constexpr unsigned int character_size = 14;
    constexpr float offset = -25.f;
    constexpr float origin_offset = -20.f;
    // 0
    sf::Text origin_label(font, "0", character_size);
    origin_label.setPosition({
        grid.point_origin.x + origin_offset,
        grid.point_origin.y + origin_offset
    });
    data.labels.push_back(std::move(origin_label));

    // X spacing
    sf::Text x_spacing_label(
        font,
        std::to_string(static_cast<int>(grid.spacing_x)),
        character_size
    );
    x_spacing_label.setPosition({
        grid.point_origin.x + grid.spacing_x,
        grid.point_origin.y + offset
    });
    data.labels.push_back(std::move(x_spacing_label));

    // Y spacing
    sf::Text y_spacing_label(
        font,
        std::to_string(static_cast<int>(grid.spacing_y)),
        character_size
    );
    y_spacing_label.setPosition({
        grid.point_origin.x + offset,
        grid.point_origin.y + grid.spacing_y
    });
    data.labels.push_back(std::move(y_spacing_label));

    // X
    sf::Text x_label(font, "x", character_size);
    x_label.setPosition({
        x_end.x,
        x_end.y + offset
    });
    data.labels.push_back(std::move(x_label));

    // Y
    sf::Text y_label(font, "y", character_size);
    y_label.setPosition({
        y_end.x + offset,
        y_end.y
    });
    data.labels.push_back(std::move(y_label));

    for (auto& label : data.labels)
    {
        label.setFillColor(sf::Color(
            static_cast<std::uint8_t>(grid.color.r * 255.0f),
            static_cast<std::uint8_t>(grid.color.g * 255.0f),
            static_cast<std::uint8_t>(grid.color.b * 255.0f),
            static_cast<std::uint8_t>(grid.color.a * 255.0f)
        ));


        const auto bounds = label.getLocalBounds();

        label.setOrigin({
            bounds.position.x,
            bounds.position.y + bounds.size.y
        });

        label.setScale({1.f, -1.f});
    }
}

GridDrawData Grid::toGridDrawData() const
{
    GridDrawData data;

    const glm::vec2 point_x_end{
        point_origin.x + spacing_x * lines_x,
        point_origin.y
    };

    const glm::vec2 point_y_end{
        point_origin.x,
        point_origin.y + spacing_y * lines_y
    };

    const glm::vec2 x_end{
        point_x_end.x + spacing_x,
        point_x_end.y
    };

    const glm::vec2 y_end{
        point_y_end.x,
        point_y_end.y + spacing_y
    };

    addGridLines(data, *this, point_x_end, point_y_end);
    addAxes(data, *this, x_end, y_end);
    addLabels(data, *this, font, x_end, y_end);

    return data;
}
