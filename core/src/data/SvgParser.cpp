#include "SvgParser.hpp"
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "nanosvg/nanosvg.h"
#include "utils/GlmColors.hpp"

static CurvePoint makeCircle(float x, float y, glm::vec4 color)
{
    return {
        Circle{
            glm::vec2(x, y),
            2, 2, 16, color, true
        },
        CurveJointType::Corner
    };
}

static glm::vec4 parseColor(const unsigned long color)
{
    return {
        ((color      ) & 0xFF) / 255,
        ((color >>  8) & 0xFF) / 255,
        ((color >> 16) & 0xFF) / 255,
        ((color >> 24) & 0xFF) / 255
    };
}

ThirdDegreeCurves parseSvg(const std::string& filePath)
{
    ThirdDegreeCurves result;

    NSVGimage* image =nsvgParseFromFile(filePath.c_str(), "px", 96);

    if (!image)
        return result;

    for (auto shape = image->shapes; shape; shape = shape->next)
    {
        glm::vec4 fillColor = colors::BLACK;

        if (shape->fill.type == NSVG_PAINT_COLOR)
            fillColor = parseColor(shape->fill.color);

        else if (shape->stroke.type == NSVG_PAINT_COLOR)
            fillColor = parseColor(shape->stroke.color);

        fillColor.a = fillColor.a * shape->opacity;


        for (auto path = shape->paths; path; path = path->next)
        {
            if (path->npts < 4)
                continue;

            ThirdDegreeCurvePath svgPath;
            svgPath.enclosed = path->closed != 0;

            const float* first = &path->pts[0];

            constexpr size_t firstAnchor = 0;
            svgPath.points.push_back(
                makeCircle(first[0], first[1], colors::BLACK)
            );

            size_t currentAnchor = firstAnchor;

            for (int i = 0; i < path->npts - 1; i += 3)
            {
                const float* p = &path->pts[i * 2];

                size_t b = svgPath.points.size();
                svgPath.points.push_back(makeCircle(p[2], p[3],colors::GREEN));

                size_t c = svgPath.points.size();
                svgPath.points.push_back(makeCircle(p[4], p[5], colors::GREEN));

                size_t d = svgPath.points.size();
                svgPath.points.push_back(makeCircle(p[6], p[7],colors::BLACK));

                ThirdDegreeCurve curve = {
                    .point_a = currentAnchor,
                    .point_b = b,
                    .point_c = c,
                    .point_d = d,
                    .weight_a = 1,
                    .weight_b = 1,
                    .weight_c = 1,
                    .weight_d = 1,
                };

                curve.color = fillColor;

                svgPath.curves.push_back(curve);

                currentAnchor = d;
            }

            if (svgPath.enclosed && !svgPath.curves.empty())
            {
                svgPath.curves.back().point_d = firstAnchor;
            }

            result.paths.push_back(std::move(svgPath));
        }
    }

    nsvgDelete(image);
    return result;
}