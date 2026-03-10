#pragma once
#include <vector>
#include <string>

class MenuElements
{
public:
	MenuElements();

	void MainWindow();
	void ApplyModernStyle();
private:
	std::vector<const char*> m_vecGraphs;

	const char* chGraphsFormula[2] = { "k*sin(x)", "k*cos(x)" };
} inline g_Menu;

