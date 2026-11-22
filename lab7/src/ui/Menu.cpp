#include "Menu.hpp"

#include <imgui.h>
#include "utils/UiUtils.hpp"


static void normalizeProbabilities(MenuParams& params)
{
    float sum = 0.f;

    for (const auto& entry : params.ifsParams.entries)
        sum += std::max(0.f, entry.p);

    if (sum <= 0.f)
        return;

    for (auto& entry : params.ifsParams.entries)
        entry.p = std::max(0.f, entry.p) / sum;
}

void drawMenu(MenuParams& params)
{
    if (!ImGui::Begin("IFS Fractal Editor" IMGUI_UNIQUE_ID))
    {
        ImGui::End();
        return;
    }

    if (ImGui::CollapsingHeader("General" IMGUI_UNIQUE_ID, ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::InputInt("Iterations" IMGUI_UNIQUE_ID, &params.ifsParams.iterations);

        params.ifsParams.iterations = std::max(1, params.ifsParams.iterations);
    }

    ImGui::Separator();

    if (ImGui::CollapsingHeader("Transformations"  IMGUI_UNIQUE_ID, ImGuiTreeNodeFlags_DefaultOpen))
    {
        for (size_t i = 0; i < params.ifsParams.entries.size(); ++i)
        {
            auto& entry = params.ifsParams.entries[i];

            if (i >= params.ifsParams.colors.size())
                params.ifsParams.colors.emplace_back(1.f);

            ImGui::PushID(static_cast<int>(i));

            std::string label = "Transformation " + std::to_string(i + 1);

            if (ImGui::TreeNode(label.c_str()))
            {
                ImGui::PushItemWidth(150.f);

                ImGui::InputFloat("a" IMGUI_UNIQUE_ID, &entry.a, 0.001f, 0.01f);
                ImGui::SameLine();
                ImGui::InputFloat("b" IMGUI_UNIQUE_ID, &entry.b, 0.001f, 0.01f);
                ImGui::SameLine();
                ImGui::InputFloat("c" IMGUI_UNIQUE_ID, &entry.c, 0.001f, 0.01f);
                ImGui::SameLine();
                ImGui::InputFloat("d" IMGUI_UNIQUE_ID, &entry.d, 0.001f, 0.01f);

                ImGui::Separator();

                ImGui::InputFloat("e" IMGUI_UNIQUE_ID, &entry.e, 0.001f, 0.01f);
                ImGui::SameLine();
                ImGui::InputFloat("f" IMGUI_UNIQUE_ID, &entry.f, 0.001f, 0.01f);

                ImGui::Separator();

                ImGui::InputFloat("p" IMGUI_UNIQUE_ID,&entry.p,0.001f, 0.01f);

                ImGui::PopItemWidth();

                entry.p = std::max(0.f, entry.p);

                ImGui::ColorEdit4(
                    "Color",
                    &params.ifsParams.colors[i].x
                );

                if (ImGui::Button("Remove transformation"))
                {
                    params.ifsParams.entries.erase(params.ifsParams.entries.begin() + i);
                    params.ifsParams.colors.erase(params.ifsParams.colors.begin() + i);

                    ImGui::TreePop();
                    ImGui::PopID();
                    break;
                }

                ImGui::TreePop();
            }

            ImGui::PopID();
        }

        if (ImGui::Button("Add transformation" IMGUI_UNIQUE_ID))
        {
            params.ifsParams.entries.push_back({
                0.5f, 0.f,
                0.f, 0.5f,
                0.f, 0.f,
                1.f
            });

            params.ifsParams.colors.emplace_back(
                1.f, 1.f, 1.f, 1.f
            );
        }

        ImGui::SameLine();

        if (ImGui::Button("Normalize probabilities" IMGUI_UNIQUE_ID))
            normalizeProbabilities(params);
    }

    if (ImGui::Button("Apply" IMGUI_UNIQUE_ID))
        params.applyChanges();

    ImGui::End();
}