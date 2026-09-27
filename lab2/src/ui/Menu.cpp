#include <imgui.h>
#include <glm/geometric.hpp>
#include <ui/Menu.hpp>

#include "transformations/Transformation.hpp"


static void drawFigureEditor(
    FoliumOfDescartes& figure,
    const FoliumOfDescartesDrawData& drawData,
    const std::function<void()>& onFigureUpdated)
{
    bool changed = false;

    changed |= ImGui::InputFloat(
        "a##Figure",
        &figure.a,
        0.05f, 0.5f
    );

    changed |= ImGui::InputFloat(
        "Angle step##Figure",
        &figure.angle_step,
        0.05f, 0.5f
    );


    changed |= ImGui::InputFloat(
        "Angle start##Figure",
        &figure.angle_start,
        0.05f, 0.5f
    );

    changed |= ImGui::InputFloat(
        "Angle end##Figure",
        &figure.angle_end,
        0.05f, 0.5f
    );

    ImGui::SeparatorText("Animation");

    changed |= ImGui::InputFloat(
        "a start##Figure",
        &figure.a_start,
        0.05f,
        0.5f
    );

    changed |= ImGui::InputFloat(
        "a end##Figure",
        &figure.a_end,
        0.05f,
        0.5f
    );

    changed |= ImGui::InputFloat(
        "a step##Figure",
        &figure.a_step,
        0.05f,
        0.5f
    );

    if (ImGui::Button(figure.animation_enabled ? "Stop animation" : "Start animation"))
    {
        figure.animation_enabled = !figure.animation_enabled;
    }

    changed |= ImGui::Checkbox(
        "Asymptote##Figure",
        &figure.include_asymptote
    );


    ImGui::Text(
        "Arc length: %.3f",
        drawData.arcLength
    );

    ImGui::Text(
        "Loop area: %.3f",
        drawData.loopArea
    );


    if (figure.angle_end <= figure.angle_start)
    {
        figure.angle_end = std::min(1000.f, figure.angle_start + figure.angle_step);


        if (figure.angle_end <= figure.angle_start)
            figure.angle_start = std::max(0.f, figure.angle_end - figure.angle_step);

        changed = true;
    }

    figure.angle_step = std::clamp(figure.angle_step, 0.001f, 1000.f);

    figure.a_step = std::clamp(
        figure.a_step,
        0.001f,
        1000.f
    );

    figure.a_start = std::clamp(
        figure.a_start,
        0.f,
        1000.f
    );

    figure.a_end = std::clamp(
        figure.a_end,
        0.f,
        1000.f
    );

    if (figure.a_end <= figure.a_start)
    {
        figure.a_end = std::min(
            1000.f,
            figure.a_start + figure.a_step
        );

        if (figure.a_end <= figure.a_start)
        {
            figure.a_start = std::max(
                0.f,
                figure.a_end - figure.a_step
            );
        }

        changed = true;
    }

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

static void drawCurvePointEditor(
    CurvePointParams& params,
    const CurvePoint& curvePoint,
    const std::function<void()>& onCurvePointUpdated)
{
    bool changed = false;

    changed |= ImGui::InputFloat(
        "Angle##CurvePoint",
        &params.angle,
        0.05f,
        0.5f
    );

    changed |= ImGui::Checkbox(
        "Tangent##CurvePoint",
        &params.includeTangent
    );

    changed |= ImGui::Checkbox(
        "Normal##CurvePoint",
        &params.includeNormal
    );

    changed |= ImGui::InputFloat(
        "Length##CurvePoint",
        &params.length,
        1.f,
        10.f
    );

    params.length = std::max(0.f, params.length);

    ImGui::Separator();

    ImGui::Text(
        "Point: (%.3f, %.3f)",
        curvePoint.point.x,
        curvePoint.point.y
    );

    ImGui::Text(
        "Curvature radius: %.10f",
        curvePoint.curvatureRadius
    );

    if (changed)
        onCurvePointUpdated();
}


void drawMenu(
    EuclideanTransformation& euclidean,
    FoliumOfDescartes& foliumOfDescartes,
    const FoliumOfDescartesDrawData& drawData,
    CurvePointParams& curvePointParams,
    const CurvePoint& curvePoint,
    const std::function<void()>& onCurvePointUpdated,
    const std::function<void()>& onFigureUpdated,
    const std::function<void()>& onEuclideanUpdated
)
{
    ImGui::Begin("Editor##1");

    if (ImGui::Button("Reset parameters"))
    {
        euclidean = EuclideanTransformation::Default;
        foliumOfDescartes = FoliumOfDescartes::Default;
    }

    if (ImGui::CollapsingHeader("Figure##2", ImGuiTreeNodeFlags_DefaultOpen))
    {
        drawFigureEditor(foliumOfDescartes, drawData, onFigureUpdated);
    }

    if (ImGui::CollapsingHeader("Euclidean##3", ImGuiTreeNodeFlags_DefaultOpen))
    {
        drawEuclideanTransformationEditor(euclidean, onEuclideanUpdated);
    }

    if (ImGui::CollapsingHeader("Curve point##3", ImGuiTreeNodeFlags_DefaultOpen))
    {
        drawCurvePointEditor(curvePointParams, curvePoint, onCurvePointUpdated);
    }

    ImGui::End();
}
