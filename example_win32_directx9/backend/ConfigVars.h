#pragma once
#include <memory>
#include <string>
#include <vector>

enum EParity : int
{
	ALL_PARITY,
	EVEN_PARITY,
	ODD_PARITY
};

struct ConfigVars
{
	ConfigVars();

	std::vector<std::string> m_vecSliderNames;
	std::vector<bool> m_vecEnableSliders;

	bool m_bFitToAxes = false;
	bool m_bRealTime = false;
	bool m_bShowMaxHeightLine = false;
	bool m_bShowSum = false;
	int m_iHarmonicParity = 0;
	std::string m_strHarmonicParity;
	float m_flLineWeigth = 1;
};

inline std::unique_ptr<ConfigVars> g_ConfigVars = std::make_unique<ConfigVars>();
