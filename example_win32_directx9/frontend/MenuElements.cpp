#include "MenuElements.h"
#include "../imgui/imgui.h"
#include "../imgui/implot.h"
#include "../Settings.h"
#include "../backend/ConfigVars.h"

MenuElements::MenuElements()
{
	Wave _Wave(0.001, 1, 0, 0, 0);
	m_vecWaves.push_back(_Wave);
	m_vecGraphsName.push_back("Сигнал 1");
}

void MenuElements::MainWindow()
{ 
	if (!g_ConfigVars.get()->m_bRealTime)
	{
		for (auto& _Wave : m_vecWaves)
			_Wave.ResetTime();
	}
	else
	{
		for (auto& _Wave : m_vecWaves)
			_Wave.ResumeTime();
	}

	ImGui::BeginChild("Main", ImVec2(g_Settings.m_vecWindowSize.x - 35, g_Settings.m_vecWindowSize.y - 55));
	{
		ImGui::SetNextItemWidth(110);

		if (ImGui::BeginCombo("Сигнал", g_Settings.m_strSelectedGraph.c_str()))
		{
			for (int n = 0; n < m_vecGraphsName.size(); n++)
			{
				bool is_selected = (g_Settings.m_strSelectedGraph == m_vecGraphsName[n]);
				if (ImGui::Selectable(m_vecGraphsName[n].c_str(), is_selected))
				{
					g_Settings.m_strSelectedGraph = m_vecGraphsName[n];
					g_Settings.m_iSelectedGraph = n;
				}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
			
			}

			ImGui::EndCombo();
		}
		ImGui::Checkbox("Авто-масштабирование", &g_ConfigVars.get()->m_bFitToAxes);

		for (int i = 0; i < m_vecWaves.size(); i++)
		{
			if (g_Settings.m_iSelectedGraph == i)
			{
				Wave& _CurWave = m_vecWaves.at(i);
				m_vecGraphsName[i] = std::to_string(_CurWave.GetWave().Freq) + " Гц";
				g_Settings.m_strSelectedGraph = std::to_string(_CurWave.GetWave().Freq) + " Гц";

			}
		}
		ImGui::SliderFloat("Частота", &m_vecWaves.at(g_Settings.m_iSelectedGraph).GetWave().Freq, 0, 120);
		ImGui::SliderFloat("Амплитуда (k)", &m_vecWaves.at(g_Settings.m_iSelectedGraph).GetWave().Amp, 0, 200);

		ImGui::SliderFloat("Толщина графика", &g_ConfigVars.get()->m_flLineWeigth, 0.1f, 10);
		ImGui::SliderFloat("Высота графика", &g_Settings.m_vecGraphSize.y, 100.f, 2140.f);

		static int DotsCount = 1;
		static int iWaveLength = 1000;

		if (ImGui::SliderInt("Длина волны", &DotsCount, 1, 100))
			iWaveLength = DotsCount * 1000;

		if (g_ConfigVars.get()->m_bFitToAxes)
			ImPlot::SetNextAxesToFit();

		g_Settings.m_vecGraphSize.x = ImGui::GetWindowWidth() - 20;
		if (ImPlot::BeginPlot("График", g_Settings.m_vecGraphSize))
		{
			ImPlotStyle& pStyle = ImPlot::GetStyle();
			pStyle.LineWeight = g_ConfigVars.get()->m_flLineWeigth;
			
			for (int i = 0; i < m_vecWaves.size(); i++)
			{
				ImPlot::PlotLineG(m_vecGraphsName.at(i).c_str(), SineWave, &m_vecWaves.at(i).GetWave(), iWaveLength);
			}

			ImPlot::EndPlot();
		}
		if (ImGui::Button("Добавить сигнал"))
		{
			Wave _Wave = Wave(0.001, 1, 0, 0, 0);
			m_vecWaves.push_back(_Wave);

			m_vecGraphsName.push_back(std::string(std::to_string(_Wave.GetWave().Freq) + " Гц"));
		}
		if (ImGui::Button("График в реальном времени"))
			g_ConfigVars.get()->m_bRealTime = !g_ConfigVars.get()->m_bRealTime;
	}
	ImGui::EndChild();
}

void MenuElements::ApplyModernStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();
	
	style.WindowRounding = 8.0f;
	style.ChildRounding = 8.0f;
	style.FrameRounding = 6.0f;
	style.PopupRounding = 8.0f;
	style.ScrollbarRounding = 6.0f;
	style.GrabRounding = 6.0f;
	style.TabRounding = 6.0f;
	
	style.WindowPadding = ImVec2(10.0f, 10.0f);
	style.FramePadding = ImVec2(8.0f, 6.0f);
	style.ItemSpacing = ImVec2(8.0f, 6.0f);
	style.ItemInnerSpacing = ImVec2(6.0f, 4.0f);
	style.TouchExtraPadding = ImVec2(0.0f, 0.0f);
	style.IndentSpacing = 21.0f;
	style.ScrollbarSize = 14.0f;
	style.GrabMinSize = 10.0f;
	
	ImVec4* colors = style.Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.12f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.15f, 0.18f, 0.94f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.10f, 0.12f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.30f, 0.30f, 0.35f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.25f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.30f, 0.30f, 0.35f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.25f, 0.30f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.10f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.30f, 0.50f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.08f, 0.08f, 0.10f, 0.75f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.12f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.45f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.50f, 0.50f, 0.55f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.60f, 0.60f, 0.65f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.40f, 0.80f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.40f, 0.80f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.70f, 0.90f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.25f, 0.50f, 0.75f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.60f, 0.85f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.40f, 0.65f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.30f, 0.50f, 0.70f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.40f, 0.60f, 0.80f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.40f, 0.60f, 0.80f, 1.00f);
	colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.40f, 0.60f, 0.80f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.60f, 0.80f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.40f, 0.60f, 0.80f, 0.20f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.40f, 0.60f, 0.80f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.60f, 0.80f, 0.95f);
}
