#pragma once
#include <memory>

struct ConfigVars
{
	bool m_bFitToAxes = false;
	bool m_bRealTime = false;
	bool m_bShowMaxHeightLine = false;
	bool m_bShowMaxLine = false;
	float m_flLineWeigth = 1;
};

inline std::unique_ptr<ConfigVars> g_ConfigVars = std::make_unique<ConfigVars>();
