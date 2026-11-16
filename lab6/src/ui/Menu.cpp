#include "Menu.hpp"

#include <imgui.h>
#include <nfd.h>
#include <glm/trigonometric.hpp>

#include "data/SvgParser.hpp"
#include "utils/UiUtils.hpp"

static void drawEuclideanTransformation3DEditor(EuclideanTransformation3D& params)
{
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

static void drawEuclideanTransformation2DEditor(EuclideanTransformation& params)
{
    if (ImGui::BeginTable("EuclideanMatrix" IMGUI_UNIQUE_ID, 2))
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

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::PushItemWidth(150.f);
        ImGui::InputFloat(
            "Angle X" IMGUI_UNIQUE_ID,
            &params.rotationDegrees,
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
        "Radius" IMGUI_UNIQUE_ID,
        &params.figure.r,
        0.5f, 5.0f
    );

    if (ImGui::BeginTable(IMGUI_UNIQUE_ID, 3))
    {
        float uStartDegrees = glm::degrees(params.figure.uStart);
        float uEndDegrees = glm::degrees(params.figure.uEnd);
        float uStepDegrees = glm::degrees(params.figure.uStep);

        float vStartDegrees = glm::degrees(params.figure.vStart);
        float vEndDegrees = glm::degrees(params.figure.vEnd);
        float vStepDegrees = glm::degrees(params.figure.vStep);

        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);
        ImGui::PushItemWidth(150.f);
        if (ImGui::InputFloat("U Start" IMGUI_UNIQUE_ID, &uStartDegrees, 1, 5))
        {
            params.figure.uStart = glm::radians(uStartDegrees);
            changed = true;
        }
        ImGui::PopItemWidth();

        ImGui::TableSetColumnIndex(1);
        ImGui::PushItemWidth(150.f);
        if (ImGui::InputFloat("U End" IMGUI_UNIQUE_ID, &uEndDegrees, 1, 5))
        {
            params.figure.uEnd = glm::radians(uEndDegrees);
            changed = true;
        }
        ImGui::PopItemWidth();

        ImGui::TableSetColumnIndex(2);
        ImGui::PushItemWidth(150.f);
        if (ImGui::InputFloat("U Step" IMGUI_UNIQUE_ID, &uStepDegrees, 0.01f, 0.05f))
        {
            params.figure.uStep = glm::radians(uStepDegrees);
            changed = true;
        }
        ImGui::PopItemWidth();

        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);
        ImGui::PushItemWidth(150.f);
        if (ImGui::InputFloat("V Start" IMGUI_UNIQUE_ID, &vStartDegrees, 1, 5))
        {
            params.figure.vStart = glm::radians(vStartDegrees);
            changed = true;
        }
        ImGui::PopItemWidth();

        ImGui::TableSetColumnIndex(1);
        ImGui::PushItemWidth(150.f);
        if (ImGui::InputFloat("V End" IMGUI_UNIQUE_ID, &vEndDegrees, 1, 5))
        {
            params.figure.vEnd = glm::radians(vEndDegrees);
            changed = true;
        }
        ImGui::PopItemWidth();

        ImGui::TableSetColumnIndex(2);
        ImGui::PushItemWidth(150.f);
        if (ImGui::InputFloat("V Step" IMGUI_UNIQUE_ID, &vStepDegrees, 0.01f, 0.05f))
        {
            params.figure.vStep = glm::radians(vStepDegrees);
            changed = true;
        }
        ImGui::PopItemWidth();

        ImGui::EndTable();
    }

    if (changed)
        params.figureUpdated();
}

static void drawFilePicker(MenuParams& params)
{
    if (ImGui::Button("Open SVG texture file", {-1.f, 32.f}))
    {
        nfdu8char_t* path = nullptr;

        const nfdresult_t result = NFD_OpenDialog(&path, nullptr, 0, nullptr);

        if (result == NFD_OKAY)
        {
            const std::string filePath(path);

            params.onLoadFile(filePath);

            NFD_FreePathU8(path);
        }
    }
}

static void drawCurveEditor(MenuParams& params)
{
    drawEuclideanTransformation2DEditor(params.euclideanTransformationCurve);
    ImGui::InputFloat(
        "Scale Factor" IMGUI_UNIQUE_ID,
        &params.curveScaleFactor,
        0.01f, 0.1f
    );
}
static void drawCurveAnimationEditor(MenuParams& params)
{
    if (ImGui::CollapsingHeader("Animation Start" IMGUI_UNIQUE_ID, ImGuiTreeNodeFlags_DefaultOpen))
    {
        drawEuclideanTransformation2DEditor(params.euclideanTransformationCurveStart);
    }
    if (ImGui::CollapsingHeader("Animation Target" IMGUI_UNIQUE_ID, ImGuiTreeNodeFlags_DefaultOpen))
    {
        drawEuclideanTransformation2DEditor(params.euclideanTransformationCurveTarget);
    }
    ImGui::Checkbox("Animate" IMGUI_UNIQUE_ID, &params.curveAnimate);
    ImGui::InputFloat(
        "Animation Speed" IMGUI_UNIQUE_ID,
        &params.curveAnimationSpeed,
        0.01f, 0.1f
    );
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

    if (ImGui::CollapsingHeader("Curve" IMGUI_UNIQUE_ID, ImGuiTreeNodeFlags_DefaultOpen))
    {
        drawFilePicker(params);
        drawCurveEditor(params);
    }

    if (ImGui::CollapsingHeader("Curve Animation" IMGUI_UNIQUE_ID, ImGuiTreeNodeFlags_DefaultOpen))
    {
        drawCurveAnimationEditor(params);
    }

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
