#pragma once
#include <vector>
#include <string>
#include "../backend/waves/Wave.h"
#include "../Settings.h"
#include <memory>

enum ESlidersChoice : int
{
    FREQUENCY,
    AMPLITUDE,
    PHASE,
    WIDTHLINE,
    HEIGTHGRAPH,
    WAVELENGTH
};

static void ShowMultiSelectCombo(const char* combo_label, std::vector<std::string>& items, std::vector<bool>& selections)
{
    std::string preview_value = "";
    bool any_selected = false;
    for (size_t i = 0; i < items.size(); ++i)
    {
        if (selections[i])
        {
            if (any_selected)
                preview_value += ", ";
            preview_value += items[i];
            any_selected = true;
        }
    }

    if (!any_selected)
        preview_value = "Select items...";

    if (ImGui::BeginCombo(combo_label, preview_value.c_str(), ImGuiComboFlags_NoArrowButton))
    {
        for (size_t i = 0; i < items.size(); ++i)
        {
            const char* item_text = items[i].c_str();
            bool is_selected = selections[i];

            if (ImGui::Selectable(item_text, &is_selected, ImGuiSelectableFlags_DontClosePopups))
            {
                selections[i] = is_selected;
            }
        }

        ImGui::EndCombo();
    }
}

class MenuElements
{
public:
    void DeleteWave(int iIndex = g_Settings.m_iSelectedGraph);

    void DrawMainSettings();
    void DrawConfig();
    void DrawHarmonySettings();
    void DrawGraph();

    void MainWindow();
    void ApplyModernStyle();
private:
    int DotsCount = 1;
    int iWaveLength = 1000;

    uint32_t m_uiChildFlags = ImGuiChildFlags_ResizeY;

    std::vector<std::string> m_vecHarmonicParityNames = { "Все", "Четные", "Нечетные" };
};

inline std::unique_ptr<MenuElements> g_Menu = std::make_unique<MenuElements>();
