#pragma once
#include <memory>
#include <vector>
#include <string>
#include "../waves/Wave.h"

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

	bool RemoveConfig(std::string strConfigName);
	bool RefreshConfig();

	std::vector<std::string> m_vecFiles;
	std::string m_strSelectedFile = "";
	int m_iSelectedFile = 0;

	std::vector<std::string> m_vecSliderNames;
	std::vector<bool> m_vecEnableSliders;
	std::vector<bool> m_vecFunctions = std::vector<bool>(EALLFINCTIONS, false);
	
	int m_iHarmonicParity = 0;
	std::string m_strHarmonicParity;
	float m_flLineWeigth = 1;

	std::vector<std::string> m_vecGraphsName;
	std::vector<Wave> m_vecWaves;
private:
	void GetConfigString(std::string& strOutput);
	void UpdateDisplaySelectedConfig();

	std::string DataPath();
	std::string m_strFolderPath = DataPath();
};

inline std::unique_ptr<ConfigSystem> g_ConfigSystem = std::make_unique<ConfigSystem>();
