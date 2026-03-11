#pragma once
#include <vector>
#include <string>
#include "../backend/Wave.h"

class MenuElements
{
public:
	MenuElements();

	void MainWindow();
	void ApplyModernStyle();
private:
	std::vector<std::string> m_vecGraphsName;
	std::vector<Wave> m_vecWaves;
} inline g_Menu;

