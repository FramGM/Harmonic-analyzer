#pragma once
#include <vector>
#include <string>
#include "../backend/Wave.h"
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
    MenuElements();

    void DeleteWave(int iIndex = g_Settings.m_iSelectedGraph);

    void DrawUpperItems();
    void DrawGraph();
    void DrawLowerItems();

    void MainWindow();
    void ApplyModernStyle();
private:
    int DotsCount = 1;
    int iWaveLength = 1000;

    std::vector<std::string> m_vecHarmonicParityNames = { "Все", "Четные", "Нечетные" };
    std::vector<std::string> m_vecGraphsName;
    std::vector<Wave> m_vecWaves;
};

inline std::unique_ptr<MenuElements> g_Menu = std::make_unique<MenuElements>();
