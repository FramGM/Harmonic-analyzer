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

enum EFunctions : int
{
	EFitToAxes,
	ERealTime,
	EShowMaxHeightLine,
	EShowSum,
	EALLFINCTIONS
};

class ConfigSystem
{
public:
	ConfigSystem();
	bool LoadConfig(std::string strConfigName);
	bool SaveConfig(std::string strConfigName);
	void GetConfigString(std::string& strOutput);

	bool RemoveConfig(std::string strConfigName);
	bool RefreshConfig();

	std::vector<std::string> m_vecSliderNames;
	std::vector<bool> m_vecEnableSliders;
	std::vector<bool> m_vecFunctions = std::vector<bool>(EALLFINCTIONS, false);
	
	int m_iHarmonicParity = 0;
	std::string m_strHarmonicParity;
	float m_flLineWeigth = 1;
};

inline std::unique_ptr<ConfigSystem> g_ConfigSystem = std::make_unique<ConfigSystem>();