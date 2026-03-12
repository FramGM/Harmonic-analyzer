#pragma once
#include <memory>

enum EParity : int
{
	ALL_PARITY,
	EVEN_PARITY,
	ODD_PARITY
};

struct ConfigVars
{
	bool m_bFitToAxes = false;
	bool m_bRealTime = false;
	bool m_bShowMaxHeightLine = false;
	bool m_bShowMaxLine = false;
	int m_iHarmonicParity = 0;
	std::string m_strHarmonicParity = "";
	float m_flLineWeigth = 1;
};

inline std::unique_ptr<ConfigVars> g_ConfigVars = std::make_unique<ConfigVars>();
