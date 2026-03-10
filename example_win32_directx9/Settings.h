#pragma once
#include "imgui/imgui.h"
#include <string>

struct Settings
{
	ImVec2 m_vecWindowSize = ImVec2(1000, 700);
	int m_iSelectedGraph = 0;
	std::string m_strSelectedGraph = "Sin";
	ImVec2 m_vecGraphSize = ImVec2(200, 200);
} inline g_Settings;