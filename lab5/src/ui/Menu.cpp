#include "Menu.hpp"

#include <imgui.h>

#include "data/SvgParser.hpp"
#include "utils/UiUtils.hpp"

static void drawEuclideanTransformation3DEditor(EuclideanTransformation3D& params)
{
    bool changed = false;

    if (ImGui::BeginTable("EuclideanMatrix" IMGUI_UNIQUE_ID, 3))
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::PushItemWidth(150.f);
        ImGui::InputFloat(
            "Offset X" IMGUI_UNIQUE_ID,
            &params.offset.x,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::TableSetColumnIndex(1);
        ImGui::PushItemWidth(150.f);
        ImGui::InputFloat(
            "Offset Y" IMGUI_UNIQUE_ID,
            &params.offset.y,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::TableSetColumnIndex(2);
        ImGui::PushItemWidth(150.f);
        ImGui::InputFloat(
            "Offset Z" IMGUI_UNIQUE_ID,
            &params.offset.z,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::PushItemWidth(150.f);
        ImGui::InputFloat(
            "Rotation Point X" IMGUI_UNIQUE_ID,
            &params.rotationPoint.x,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::TableSetColumnIndex(1);
        ImGui::PushItemWidth(150.f);
        ImGui::InputFloat(
            "Rotation Point Y" IMGUI_UNIQUE_ID,
            &params.rotationPoint.y,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::TableSetColumnIndex(2);
        ImGui::PushItemWidth(150.f);
        ImGui::InputFloat(
            "Rotation Point Z" IMGUI_UNIQUE_ID,
            &params.rotationPoint.z,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::PushItemWidth(150.f);
        ImGui::InputFloat(
            "Angle X" IMGUI_UNIQUE_ID,
            &params.rotationDegrees.x,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::TableSetColumnIndex(1);
        ImGui::PushItemWidth(150.f);
        ImGui::InputFloat(
            "Angle Y" IMGUI_UNIQUE_ID,
            &params.rotationDegrees.y,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::TableSetColumnIndex(2);
        ImGui::PushItemWidth(150.f);
        ImGui::InputFloat(
            "Angle Z" IMGUI_UNIQUE_ID,
            &params.rotationDegrees.z,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::EndTable();
    }
}

static void drawDimetricTransformationEditor(MenuParams& params)
{
    ImGui::InputFloat(
        "Angle" IMGUI_UNIQUE_ID,
        &params.dimetricTransformation.rotationAngleDegrees,
        0.5f, 5.0f
    );
}

static void drawFigureEditor(MenuParams& params)
{
    bool changed = false;
    changed |= ImGui::InputFloat(
        "Outer Radius" IMGUI_UNIQUE_ID,
        &params.figure.outerRadius,
        0.5f, 5.0f
    );
    changed |= ImGui::InputFloat(
        "Inner Radius" IMGUI_UNIQUE_ID,
        &params.figure.innerRadius,
        0.5f, 5.0f
    );
    changed |= ImGui::InputFloat(
        "Height" IMGUI_UNIQUE_ID,
        &params.figure.height,
        0.5f, 5.0f
    );

    if (changed)
        params.figureUpdated();
}

static void drawAnimationEditor(MenuParams& params)
{
    if (ImGui::CollapsingHeader("Animation Start" IMGUI_UNIQUE_ID, ImGuiTreeNodeFlags_DefaultOpen))
    {
        drawEuclideanTransformation3DEditor(params.euclideanTransformationStart);
    }
    if (ImGui::CollapsingHeader("Animation Target" IMGUI_UNIQUE_ID, ImGuiTreeNodeFlags_DefaultOpen))
    {
        drawEuclideanTransformation3DEditor(params.euclideanTransformationTarget);
    }
    ImGui::Checkbox("Animate" IMGUI_UNIQUE_ID, &params.animate);
    ImGui::InputFloat(
        "Animation Speed" IMGUI_UNIQUE_ID,
        &params.animationSpeed,
        0.01f, 0.1f
    );
}

void drawMenu(MenuParams& params)
{
    ImGui::Begin("Editor" IMGUI_UNIQUE_ID);

    ImGui::Spacing();
    ImGui::Separator();

    if (ImGui::CollapsingHeader("Figure" IMGUI_UNIQUE_ID, ImGuiTreeNodeFlags_DefaultOpen))
    {
        drawFigureEditor(params);
    }

    if (ImGui::CollapsingHeader("Euclidean" IMGUI_UNIQUE_ID, ImGuiTreeNodeFlags_DefaultOpen))
    {
        drawEuclideanTransformation3DEditor(params.euclideanTransformation);
    }

    if (ImGui::CollapsingHeader("Animation" IMGUI_UNIQUE_ID, ImGuiTreeNodeFlags_DefaultOpen))
    {
        drawAnimationEditor(params);
    }

    if (ImGui::CollapsingHeader("Projection" IMGUI_UNIQUE_ID, ImGuiTreeNodeFlags_DefaultOpen))
    {
        drawDimetricTransformationEditor(params);
    }
    ImGui::End();
}
