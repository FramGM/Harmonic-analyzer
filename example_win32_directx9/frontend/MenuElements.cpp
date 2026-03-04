#include "MenuElements.h"
#include "../imgui/imgui.h"
#include "../imgui/implot.h"
#include "../Settings.h"
#include "../backend/WaveData.h"
#include <memory>

struct ConfigVars
{
	float m_flFrequency;
	float m_flAmplitude = 1;
	bool m_bFitToAxes = false;
	double m_dlStartTime = 0.0;
	double m_dlDiffTime = 0.0;
	bool m_bRealTime = false;
	float m_flLineWeigth = 1;
};

inline std::unique_ptr<ConfigVars> g_ConfigVars = std::make_unique<ConfigVars>();

void MenuElements::MainWindow()
{
	static const char* chGraphs[] = { "Sin", "Cos", "Sin & Cos" };
	static const char* chGraphsFormula[] = { "k*sin(x)", "k*cos(x)" };

	WaveData data1(0.001, g_ConfigVars.get()->m_flAmplitude, g_ConfigVars.get()->m_flFrequency, 0, g_ConfigVars.get()->m_dlStartTime);
	if (!g_ConfigVars.get()->m_bRealTime)
	{
		data1.m_dlTimeDiff = 0;
		g_ConfigVars.get()->m_dlDiffTime = data1.m_dlTimeDiff;
		g_ConfigVars.get()->m_dlStartTime = ImGui::GetTime();
	}

	ImGui::BeginChild("Main", ImVec2(g_Settings.m_vecWindowSize.x - 35, g_Settings.m_vecWindowSize.y - 55));
	{
		ImGui::SetNextItemWidth(110);
		ImGui::Combo("Функция", &g_Settings.m_iSelectedGraph, chGraphs, IM_ARRAYSIZE(chGraphs));
		ImGui::Checkbox("Авто-масштабирование", &g_ConfigVars.get()->m_bFitToAxes);
		ImGui::SliderFloat("Частота", &g_ConfigVars.get()->m_flFrequency, 0, 120);
		ImGui::SliderFloat("Амплитуда (k)", &g_ConfigVars.get()->m_flAmplitude, 0, 200);
		ImGui::SliderFloat("Толщина графика", &g_ConfigVars.get()->m_flLineWeigth, 0.1f, 10);

		static int DotsCount = 1;
		static int iWaveLength = 1000;

		if (ImGui::SliderInt("Длина волны", &DotsCount, 1, 100))
			iWaveLength = DotsCount * 1000;

		if (g_ConfigVars.get()->m_bFitToAxes)
			ImPlot::SetNextAxesToFit();

		ImPlot::BeginPlot("График");
		{
			ImPlotStyle& pStyle = ImPlot::GetStyle();
			pStyle.LineWeight = g_ConfigVars.get()->m_flLineWeigth;
			switch (g_Settings.m_iSelectedGraph)
			{
			case 0:
				ImPlot::PlotLineG(chGraphsFormula[g_Settings.m_iSelectedGraph], SineWave, &data1, iWaveLength);
				break;
			case 1:
				ImPlot::PlotLineG(chGraphsFormula[g_Settings.m_iSelectedGraph], CosWave, &data1, iWaveLength);
				break;
			case 2:
				ImPlot::PlotLineG(chGraphsFormula[0], SineWave, &data1, iWaveLength);
				ImPlot::PlotLineG(chGraphsFormula[1], CosWave, &data1, iWaveLength);
				break;
			default:
				break;
			}
		}
		ImPlot::EndPlot();

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
