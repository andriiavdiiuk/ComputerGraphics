#include "Menu.hpp"

#include <imgui.h>
#include <nfd.hpp>
#include <string>

#include "data/SvgParser.hpp"

static void drawFilePicker(MenuParams& params)
{
    if (ImGui::Button("Open file", {-1.f, 32.f}))
    {
        nfdu8char_t* path = nullptr;

        const nfdresult_t result =
            NFD_OpenDialog(&path, nullptr, 0, nullptr);

        if (result == NFD_OKAY)
        {
            const std::string filePath(path);

            params.onLoadFile(filePath);

            NFD_FreePathU8(path);
        }
    }

    ImGui::Checkbox("Animate", &params.toggleAnimation);
    ImGui::Checkbox("Auto Animate", &params.toggleAutoAnimation);
    ImGui::InputFloat("Animation step", &params.animation_step, 0.01f, 0.1f, "%.3f");
    ImGui::InputFloat("Animation index", &params.animation_index, 0.05f, 0.1f, "%.3f");

}

static void drawCurvePointEditor(MenuParams& params)
{
    ImGui::Checkbox("Render control points##ControlPoints",&params.renderControlPoints);

    if (ImGui::BeginTable("Weights", 2, ImGuiTableFlags_SizingStretchSame))
    {
        bool changed = false;
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::TextUnformatted("Weight A");
        ImGui::TableNextColumn();
        ImGui::SetNextItemWidth(-FLT_MIN);
        changed |= ImGui::InputFloat("##weight_a", &params.weight_a, 0.05f, 0.1f, "%.3f");

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::TextUnformatted("Weight B");
        ImGui::TableNextColumn();
        ImGui::SetNextItemWidth(-FLT_MIN);
        changed |=  ImGui::InputFloat("##weight_b", &params.weight_b, 0.05f, 0.1f, "%.3f");

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::TextUnformatted("Weight C");
        ImGui::TableNextColumn();
        ImGui::SetNextItemWidth(-FLT_MIN);
        changed |=  ImGui::InputFloat("##weight_c", &params.weight_c, 0.05f, 0.1f, "%.3f");

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::TextUnformatted("Weight D");
        ImGui::TableNextColumn();
        ImGui::SetNextItemWidth(-FLT_MIN);
        changed |= ImGui::InputFloat("##weight_d", &params.weight_d, 0.05f, 0.1f, "%.3f");

        ImGui::EndTable();

        if (changed)
        {
            params.onWeightsUpdate(params.weight_a, params.weight_b, params.weight_c, params.weight_d);
        }
    }

}


static void drawEuclideanTransformationEditor(MenuParams& params)
{
    bool changed = false;

    if (ImGui::BeginTable("EuclideanMatrix", 2))
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::PushItemWidth(150.f);
        ImGui::InputFloat(
            "Offset X##Euclidean",
            &params.euclideanTransformation.offset.x,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::TableSetColumnIndex(1);
        ImGui::PushItemWidth(150.f);
         ImGui::InputFloat(
            "Offset Y##Euclidean",
            &params.euclideanTransformation.offset.y,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::PushItemWidth(150.f);
        ImGui::InputFloat(
            "Rotation Point X##Euclidean",
            &params.euclideanTransformation.rotationPoint.x,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::TableSetColumnIndex(1);
        ImGui::PushItemWidth(150.f);
        ImGui::InputFloat(
            "Rotation Point Y##Euclidean",
            &params.euclideanTransformation.rotationPoint.y,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::PushItemWidth(150.f);
         ImGui::InputFloat(
            "Angle##Euclidean",
            &params.euclideanTransformation.rotationDegrees,
            0.5f, 5.0f
        );
        ImGui::PopItemWidth();

        ImGui::EndTable();
    }
}


void drawMenu(MenuParams& params)
{
    ImGui::Begin("Editor##1");

    drawFilePicker(params);

    ImGui::Spacing();
    ImGui::Separator();

    drawCurvePointEditor(params);
    drawEuclideanTransformationEditor(params);

    ImGui::End();
}