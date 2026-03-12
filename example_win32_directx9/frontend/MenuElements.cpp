#include "MenuElements.h"
#include "../imgui/imgui.h"
#include "../imgui/implot.h"
#include "../backend/ConfigVars.h"
#include <algorithm>

MenuElements::MenuElements()
{
	Wave _Wave(0.001, 1, 0, 0, 0, 1);
	m_vecWaves.push_back(_Wave);
	m_vecGraphsName.push_back("1. 0 Гц");
}

void MenuElements::DeleteWave(int iIndex)
{
	int indexToRemove = iIndex;

	m_vecWaves.erase(m_vecWaves.begin() + indexToRemove);
	m_vecGraphsName.erase(m_vecGraphsName.begin() + indexToRemove);

	//printf("Deleted: %i\n", indexToRemove + 1);

	for (int i = indexToRemove; i < m_vecWaves.size(); i++)
	{
		m_vecWaves[i].GetWave().m_iIndex = i + 1;

		char nameBuffer[64];
		snprintf(nameBuffer, sizeof(nameBuffer), "%i. %g Гц##%i", i + 1, m_vecWaves[i].GetWave().Freq, i + 1);
		m_vecGraphsName[i] = nameBuffer;
	}

	//for (int i = 0; i < m_vecWaves.size(); i++)
	//	printf("Signal: %i\n", m_vecWaves.at(i).GetIndex());

	if (indexToRemove <= g_Settings.m_iSelectedGraph)
	{
		g_Settings.m_iSelectedGraph = std::max(0, g_Settings.m_iSelectedGraph - 1);
	}

	if (!m_vecGraphsName.empty())
	{
		g_Settings.m_strSelectedGraph = m_vecGraphsName[g_Settings.m_iSelectedGraph];
	}
}

void MenuElements::DrawUpperItems()
{
	ImGui::SetNextItemWidth(110);

	if (ImGui::BeginCombo("Гармоника", g_Settings.m_strSelectedGraph.c_str()))
	{
		for (int n = 0; n < m_vecGraphsName.size(); n++)
		{
			bool bDrawHarmonic = false;
			switch (g_ConfigVars.get()->m_iHarmonicParity)
			{
			case ALL_PARITY:
				bDrawHarmonic = true;
				break;
			case EVEN_PARITY:
				if (n % 2 != 0)
					bDrawHarmonic = true;
				break;
			case ODD_PARITY:
				if (n % 2 == 0)
					bDrawHarmonic = true;
				break;
			default:
				break;
			}

			if (!bDrawHarmonic)
				continue;

			bool is_selected = (g_Settings.m_iSelectedGraph == n);
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
	ImGui::SameLine();
	if (ImGui::Button("Добавить гармонику"))
	{
		Wave _Wave = Wave(0.001, 1, 0, 0, 0, m_vecWaves.size() + 1);
		m_vecWaves.push_back(_Wave);

		char nameBuffer[64];
		snprintf(nameBuffer, sizeof(nameBuffer), "%i. %g Гц##%i", m_vecWaves.size(), _Wave.GetWave().Freq, m_vecWaves.size());

		m_vecGraphsName.push_back(nameBuffer);
	}
	ImGui::SameLine();

	if (ImGui::Button("Удалить текущую гармонику") && m_vecWaves.size() > 1)
		DeleteWave();


	ImGui::Checkbox("Авто-масштабирование", &g_ConfigVars.get()->m_bFitToAxes);
	ImGui::Checkbox("Показать макс. высоту", &g_ConfigVars.get()->m_bShowMaxHeightLine);
	ImGui::Checkbox("Линия максимумов", &g_ConfigVars.get()->m_bShowMaxLine);

	if (ImGui::BeginCombo("Индексы гармоник", g_ConfigVars.get()->m_strHarmonicParity.c_str()))
	{
		for (int n = 0; n < m_vecHarmonicParityNames.size(); n++)
		{
			bool is_selected = (g_ConfigVars.get()->m_iHarmonicParity == n);
			if (ImGui::Selectable(m_vecHarmonicParityNames[n].c_str(), is_selected))
			{
				g_ConfigVars.get()->m_strHarmonicParity = m_vecHarmonicParityNames[n];
				g_ConfigVars.get()->m_iHarmonicParity = n;
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	for (int i = 0; i < m_vecWaves.size(); i++)
	{
		if (g_Settings.m_iSelectedGraph == i)
		{
			Wave& _CurWave = m_vecWaves.at(i);
			char freqBuffer[32];
			snprintf(freqBuffer, sizeof(freqBuffer), "%i. %g Гц##%i", i + 1, _CurWave.GetFrequency(), i);
			m_vecGraphsName[i] = freqBuffer;
			g_Settings.m_strSelectedGraph = freqBuffer;
		}
	}

	if (g_Settings.m_iSelectedGraph >= 0 && g_Settings.m_iSelectedGraph < m_vecWaves.size())
	{
		ImGui::SliderFloat("Частота", &m_vecWaves.at(g_Settings.m_iSelectedGraph).GetWave().Freq, 0, 1000, "%.3f");
		ImGui::SliderFloat("Амплитуда (k)", &m_vecWaves.at(g_Settings.m_iSelectedGraph).GetWave().Amp, -200, 200, "%.3f");
	}

	ImGui::SliderFloat("Толщина графика", &g_ConfigVars.get()->m_flLineWeigth, 0.1f, 10);
	ImGui::SliderFloat("Высота графика", &g_Settings.m_vecGraphSize.y, 100.f, 2140.f);

	if (ImGui::SliderInt("Длина волны", &DotsCount, 1, 100))
		iWaveLength = DotsCount * 1000;

	if (g_ConfigVars.get()->m_bFitToAxes)
		ImPlot::SetNextAxesToFit();
}

struct MaxLineData
{
	std::vector<Wave> m_vecWaves;
	double m_dlStartX;
	double m_dlEndX;
	int m_iNumPoints;
};

static ImPlotPoint MaxLine(int idx, void* data)
{
	MaxLineData* maxData = (MaxLineData*)data;

	double x = maxData->m_dlStartX + (maxData->m_dlEndX - maxData->m_dlStartX) * idx / (maxData->m_iNumPoints - 1);

	double maxValue = -std::numeric_limits<double>::infinity();

	for (int i = 0; i < maxData->m_vecWaves.size(); i++)
	{
		bool bDrawHarmonic = false;
		switch (g_ConfigVars.get()->m_iHarmonicParity)
		{
		case ALL_PARITY:
			bDrawHarmonic = true;
			break;
		case EVEN_PARITY:
			if (i % 2 != 0)
				bDrawHarmonic = true;
			break;
		case ODD_PARITY:
			if (i % 2 == 0)
				bDrawHarmonic = true;
			break;
		default:
			break;
		}

		if (!bDrawHarmonic)
			continue;

		Wave wave = maxData->m_vecWaves.at(i);
		WaveData& wd = wave.GetWave();
		double time = wd.m_dlTimeDiff;
		double value = wd.Offset + wd.Amp * sin(2.0 * M_PI * wd.Freq * wd.m_iIndex * x);

		if (value > maxValue)
			maxValue = value;
	}

	return ImPlotPoint(x, maxValue);
}

void MenuElements::DrawGraph()
{
	g_Settings.m_vecGraphSize.x = ImGui::GetWindowWidth() - 20;
	if (ImPlot::BeginPlot("График", g_Settings.m_vecGraphSize))
	{
		ImPlotStyle& pStyle = ImPlot::GetStyle();
		pStyle.LineWeight = g_ConfigVars.get()->m_flLineWeigth;

		for (int i = 0; i < m_vecWaves.size(); i++)
		{
			bool bDrawHarmonic = false;
			switch (g_ConfigVars.get()->m_iHarmonicParity)
			{
			case ALL_PARITY:
				bDrawHarmonic = true;
				break;
			case EVEN_PARITY:
				if (i % 2 != 0)
					bDrawHarmonic = true;
				break;
			case ODD_PARITY:
				if (i % 2 == 0)
					bDrawHarmonic = true;
				break;
			default:
				break;
			}
			if (bDrawHarmonic)
				ImPlot::PlotLineG(m_vecGraphsName.at(i).c_str(), SineWave, &m_vecWaves.at(i).GetWave(), iWaveLength);
		}

		if (g_ConfigVars.get()->m_bShowMaxLine && !m_vecWaves.empty())
		{
			double currentTime = m_vecWaves.at(0).GetWave().m_dlTimeDiff;
			double startX = currentTime;
			double endX = currentTime + iWaveLength * m_vecWaves.at(0).GetWave().X;
			MaxLineData maxData = { m_vecWaves, startX, endX, iWaveLength };

			ImPlot::SetNextLineStyle(ImVec4(1, 0, 0, 1), 2.0f);
			ImPlot::PlotLineG("Максимумы", MaxLine, &maxData, iWaveLength);
		}

		if (ImPlot::IsPlotHovered() && g_ConfigVars.get()->m_bShowMaxHeightLine)
		{
			double mouseX = std::clamp(ImPlot::GetPlotMousePos().x, m_vecWaves.at(g_Settings.m_iSelectedGraph).GetStartPos(), m_vecWaves.at(g_Settings.m_iSelectedGraph).GetStartPos() + DotsCount);
			double mouseY = ImPlot::GetPlotMousePos().y;

			double maxHeight = -std::numeric_limits<double>::infinity();
			double minHeight = std::numeric_limits<double>::infinity();

			for (int i = 0; i < m_vecWaves.size(); i++)
			{
				bool bDrawHarmonic = false;
				switch (g_ConfigVars.get()->m_iHarmonicParity)
				{
				case ALL_PARITY:
					bDrawHarmonic = true;
					break;
				case EVEN_PARITY:
					if (i % 2 != 0)
						bDrawHarmonic = true;
					break;
				case ODD_PARITY:
					if (i % 2 == 0)
						bDrawHarmonic = true;
					break;
				default:
					break;
				}

				if (!bDrawHarmonic)
					continue;

				Wave wave = m_vecWaves.at(i);

				double value = CalculateWaveValue(wave.GetWave(), mouseX);
				maxHeight = std::max(maxHeight, value);
				minHeight = std::min(minHeight, value);
			}

			ImPlot::DragLineX(0, &mouseX, ImVec4(1, 0, 0, 1), 1.0f, ImPlotDragToolFlags_NoInputs);

			ImPlot::Annotation(mouseX, maxHeight, ImVec4(1, 1, 1, 1), ImVec2(5, 5), true, "Макс: %.2f", maxHeight);
		}

		ImPlot::EndPlot();
	}
}

void MenuElements::DrawLowerItems()
{
	if (ImGui::Button("График в реальном времени"))
		g_ConfigVars.get()->m_bRealTime = !g_ConfigVars.get()->m_bRealTime;

	if (ImGui::Button("Сброс параметров"))
	{
		m_vecWaves.clear();
		m_vecGraphsName.clear();

		Wave _Wave(0.001, 1, 0, 0, 0, 1);
		m_vecWaves.push_back(_Wave);
		m_vecGraphsName.push_back("1. 0 Гц");
		g_Settings.m_iSelectedGraph = 0;
		g_Settings.m_strSelectedGraph = "1. 0 Гц";
	}

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
		DrawUpperItems();
		DrawGraph();
		DrawLowerItems();
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
