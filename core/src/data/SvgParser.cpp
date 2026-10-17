#include "SvgParser.hpp"
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "nanosvg/nanosvg.h"

ThirdDegreeCurves parseSvg(const std::string& filePath)
{
    ThirdDegreeCurves result;

    NSVGimage* image =
        nsvgParseFromFile(filePath.c_str(), "px", 96);

    if (!image)
        return result;

    auto makeCircle = [](float x, float y) {
        return Circle{
            glm::vec2(x, y),
            2, 2, 16, {0, 1, 0, 1}, true
        };
    };

    for (auto shape = image->shapes; shape; shape = shape->next)
    {
        for (auto path = shape->paths; path; path = path->next)
        {
            if (path->npts < 4)
                continue;

            ThirdDegreeCurvePath svgPath;
            svgPath.enclosed = path->closed != 0;

            const float* first = &path->pts[0];

            const size_t firstAnchor = 0;
            svgPath.points.push_back(
                makeCircle(first[0], first[1])
            );

            size_t currentAnchor = firstAnchor;

            for (int i = 0; i < path->npts - 1; i += 3)
            {
                const float* p = &path->pts[i * 2];

                size_t b = svgPath.points.size();
                svgPath.points.push_back(makeCircle(p[2], p[3]));

                size_t c = svgPath.points.size();
                svgPath.points.push_back(makeCircle(p[4], p[5]));

                size_t d = svgPath.points.size();
                svgPath.points.push_back(makeCircle(p[6], p[7]));

                svgPath.curves.push_back({
                    .point_a = currentAnchor,
                    .point_b = b,
                    .point_c = c,
                    .point_d = d,
                    .weight_a = 1,
                    .weight_b = 1,
                    .weight_c = 1,
                    .weight_d = 1
                });

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