#pragma once
#include <memory>
#include <vector>
#include <string>

enum EParity : int
{
	ALL_PARITY,
	EVEN_PARITY,
	ODD_PARITY
};

class ConfigSystem
{
public:
	bool LoadConfig(std::string strConfigName);
	bool SaveConfig(std::string strConfigName);

	bool RemoveConfig(std::string strConfigName);
	bool CreateConfig(std::string strConfigName);
	bool RefreshConfig();

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

inline std::unique_ptr<ConfigSystem> g_ConfigSystem = std::make_unique<ConfigSystem>();