#pragma once
#include <vector>
#include <string>
#include "../backend/Wave.h"

class MenuElements
{
public:
	MenuElements();

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
} inline g_Menu;

