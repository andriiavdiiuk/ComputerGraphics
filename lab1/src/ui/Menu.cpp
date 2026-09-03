#include <imgui.h>
#include <glm/geometric.hpp>
#include <ui/Menu.hpp>

#include "transformations/Transformation.hpp"

static bool inputLine(const char* label, glm::vec2& a, glm::vec2& b)
{
    float length = glm::length(b - a);

    if (ImGui::InputFloat(label, &length, 0.5f, 5.0f, "%.2f"))
    {
        resizeLinePoints(a, b, length);

        return true;
    }
    return false;
}

static void drawFigureEditor(
    Figure& figure,
    const std::function<void()>& onFigureUpdated)
{
    bool changed = false;

    ImGui::SeparatorText("Circles");

    changed |= ImGui::InputFloat(
        "Circle A radius##Figure",
        &figure.circle_a_radius,
        0.5f, 5.0f
    );

    changed |= ImGui::InputFloat(
        "Circle B radius##Figure",
        &figure.circle_b_radius,
        0.5f, 5.0f
    );

    changed |= ImGui::InputFloat(
        "Circle C radius##Figure",
        &figure.circle_c_radius,
        0.5f, 0.5f
    );

    ImGui::SeparatorText("Lines");

    changed |= inputLine("Line AB##Figure", figure.point_a, figure.point_b);
    changed |= inputLine("Line BC##Figure", figure.point_b, figure.point_c);
    changed |= inputLine("Line CD##Figure", figure.point_c, figure.point_d);
    changed |= inputLine("Line DE##Figure", figure.point_d, figure.point_e);
    changed |= inputLine("Line EF##Figure", figure.point_e, figure.point_f);
    changed |= inputLine("Line FG##Figure", figure.point_f, figure.point_g);
    changed |= inputLine("Line GH##Figure", figure.point_g, figure.point_h);
    changed |= inputLine("Line HA##Figure", figure.point_h, figure.point_a);

    if (changed)
        onFigureUpdated();
}

static void drawEuclideanTransformationEditor(
    EuclideanTransformation& transformation,
        const std::function<void()>& onEuclideanUpdated)
{
    bool changed = false;

    if (ImGui::BeginTable("EuclideanMatrix", 2))
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::PushItemWidth(150.f);
        changed |= ImGui::InputFloat(
            "Offset X##Euclidean",
            &transformation.offset.x,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::TableSetColumnIndex(1);
        ImGui::PushItemWidth(150.f);
        changed |= ImGui::InputFloat(
            "Offset Y##Euclidean",
            &transformation.offset.y,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::PushItemWidth(150.f);
        changed |= ImGui::InputFloat(
            "Rotation Point X##Euclidean",
            &transformation.rotationPoint.x,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::TableSetColumnIndex(1);
        ImGui::PushItemWidth(150.f);
        changed |= ImGui::InputFloat(
            "Rotation Point Y##Euclidean",
            &transformation.rotationPoint.y,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::PushItemWidth(150.f);
        changed |= ImGui::InputFloat(
            "Angle##Euclidean",
            &transformation.rotationDegrees,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::EndTable();
    }

    if (changed)
        onEuclideanUpdated();
}

static void drawAffineTransformationEditor(
    AffineTransformation& transformation)
{
    bool changed = false;

    if (ImGui::BeginTable("AffineMatrix", 2))
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::PushItemWidth(150.f);
        changed |= ImGui::InputFloat(
            "Xx##Affine",
            &transformation.unitX.x,
            0.05f, 0.05f
        );
        ImGui::PopItemWidth();

        ImGui::TableSetColumnIndex(1);
        ImGui::PushItemWidth(150.f);
        changed |= ImGui::InputFloat(
            "Xy##Affine",
            &transformation.unitX.y,
            0.05f, 0.05f
        );
        ImGui::PopItemWidth();

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::PushItemWidth(150.f);
        changed |= ImGui::InputFloat(
            "Yx##Affine",
            &transformation.unitY.x,
            0.05f, 0.05f
        );
        ImGui::PopItemWidth();

        ImGui::TableSetColumnIndex(1);
        ImGui::PushItemWidth(150.f);
        changed |= ImGui::InputFloat(
            "Yy##Affine",
            &transformation.unitY.y,
            0.05f, 0.05f
        );
        ImGui::PopItemWidth();

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::PushItemWidth(150.f);
        changed |= ImGui::InputFloat(
            "Ox##Affine",
            &transformation.origin.x,
            0.05f, 0.05f
        );
        ImGui::PopItemWidth();

        ImGui::TableSetColumnIndex(1);
        ImGui::PushItemWidth(150.f);
        changed |= ImGui::InputFloat(
            "Oy##Affine",
            &transformation.origin.y,
            0.05f, 0.05f
        );
        ImGui::PopItemWidth();

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::PushItemWidth(150.f);
        changed |= ImGui::InputFloat(
            "Scale X##Affine",
            &transformation.scale.x,
            0.05f, 0.05f
        );
        ImGui::PopItemWidth();

        ImGui::TableSetColumnIndex(1);
        ImGui::PushItemWidth(150.f);
        changed |= ImGui::InputFloat(
            "Scale Y##Affine",
            &transformation.scale.y,
            0.05f, 0.05f
        );
        ImGui::PopItemWidth();

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        changed |= ImGui::Checkbox("Apply symmetry##Affine", &transformation.isApplySymmetry);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::PushItemWidth(150.f);
        changed |= ImGui::InputFloat(
            "Symmetry X##Affine",
            &transformation.symmetryPoint.x,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::TableSetColumnIndex(1);
        ImGui::PushItemWidth(150.f);
        changed |= ImGui::InputFloat(
            "Symmetry Y##Affine",
            &transformation.symmetryPoint.y,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::EndTable();
    }
}

static void drawProjectiveTransformationEditor(
    ProjectiveTransformation& transformation)
{
    bool changed = false;

    if (ImGui::BeginTable("ProjectiveMatrix", 3))
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::PushItemWidth(150.f);
        changed |= ImGui::InputFloat(
            "Xx##Projective",
            &transformation.xAxisEnd.x,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::TableSetColumnIndex(1);
        ImGui::PushItemWidth(150.f);
        changed |= ImGui::InputFloat(
            "Xy##Projective",
            &transformation.xAxisEnd.y,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::TableSetColumnIndex(2);
        ImGui::PushItemWidth(150.f);
        changed |= ImGui::InputFloat(
            "Wx##Projective",
            &transformation.weights.x,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::PushItemWidth(150.f);
        changed |= ImGui::InputFloat(
            "Yx##Projective",
            &transformation.yAxisEnd.x,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::TableSetColumnIndex(1);
        ImGui::PushItemWidth(150.f);
        changed |= ImGui::InputFloat(
            "Yy##Projective",
            &transformation.yAxisEnd.y,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::TableSetColumnIndex(2);
        ImGui::PushItemWidth(150.f);
        changed |= ImGui::InputFloat(
            "Wy##Projective",
            &transformation.weights.y,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::PushItemWidth(150.f);
        changed |= ImGui::InputFloat(
            "Ox##Projective",
            &transformation.origin.x,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::TableSetColumnIndex(1);
        ImGui::PushItemWidth(150.f);
        changed |= ImGui::InputFloat(
            "Oy##Projective",
            &transformation.origin.y,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::TableSetColumnIndex(2);
        ImGui::PushItemWidth(150.f);
        changed |= ImGui::InputFloat(
            "##Projective",
            &transformation.weights.z,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::EndTable();
    }
}


void drawMenu(
    ProjectiveTransformation& projective,
    AffineTransformation& affine,
    EuclideanTransformation& euclidean,
    bool& useProjective,
    bool& useAffine,
    Figure& figure,
    const std::function<void()>& onFigureUpdated,
    const std::function<void()>& onEuclideanUpdated
)
{
    ImGui::Begin("Editor##1");

    if (ImGui::Button("Reset parameters"))
    {
        euclidean = EuclideanTransformation::Default;
        projective = ProjectiveTransformation::Default;
        affine = AffineTransformation::Default;
        figure = Figure::Default({100, 100}, 5);
        onFigureUpdated();
    }

    if (ImGui::CollapsingHeader("Figure##2", ImGuiTreeNodeFlags_DefaultOpen))
    {
        drawFigureEditor(figure, onFigureUpdated);
    }

    if (ImGui::CollapsingHeader("Euclidean##3", ImGuiTreeNodeFlags_DefaultOpen))
    {
        drawEuclideanTransformationEditor(euclidean, onEuclideanUpdated);
    }

    if (ImGui::Checkbox("Affine##4", &useAffine))
    {
        if (useAffine)
        {
            projective = ProjectiveTransformation::Default;
            affine = AffineTransformation::Default;
            useProjective = false;
        }
    }

    if (ImGui::Checkbox("Projective##5", &useProjective))
    {
        if (useProjective)
        {
            projective = ProjectiveTransformation::Default;
            affine = AffineTransformation::Default;
            useAffine = false;
        }
    }

    if (useAffine && ImGui::CollapsingHeader("Affine##6", ImGuiTreeNodeFlags_DefaultOpen))
    {
        drawAffineTransformationEditor(affine);
    }

    if (useProjective && ImGui::CollapsingHeader("Projective##7", ImGuiTreeNodeFlags_DefaultOpen))
    {
        drawProjectiveTransformationEditor(projective);
    }


    ImGui::End();
}
