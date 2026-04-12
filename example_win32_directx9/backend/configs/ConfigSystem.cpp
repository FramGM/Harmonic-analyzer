#include "ConfigSystem.h"
#include <fstream>
#include <Windows.h>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <filesystem>

inline std::vector<std::string> split(std::string s, std::string delimiter) 
{
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) 
	{
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

std::string ConfigSystem::DataPath() 
{
	char la[MAX_PATH];
	if (GetEnvironmentVariableA("LOCALAPPDATA", la, MAX_PATH) == 0)
		return {};
	std::string dir = std::string(la) + "\\HarmonicAnalyzer\\";
	CreateDirectoryA(dir.c_str(), nullptr);
	return dir;
}

ConfigSystem::ConfigSystem() 
{
	Wave _Wave(0.001, 1, 0, 0, 0, 1);
	m_vecWaves.push_back(_Wave);
	m_vecGraphsName.push_back("1; 0 Гц");

	m_vecSliderNames = { "Частота", "Амплитуда", "Фаза", "Толщина графика", "Высота графика", "Длинна волны" };
	m_vecEnableSliders = std::vector<bool>(m_vecSliderNames.size(), false);
	m_strHarmonicParity = "Все";

	RefreshConfig();
}

bool ConfigSystem::LoadConfig(std::string strConfigName)
{
	std::filesystem::path strFilePath = m_strFolderPath + "\\" + strConfigName;

	if (strConfigName.find(".cfg") == std::string::npos)
		strFilePath += ".cfg";

	std::ifstream pFile(strFilePath);
	if (!pFile.is_open() || pFile.bad())
		throw std::exception("Can't open file");

	std::string strElem;

	int iElemCount = 0;
	for (int i = 0; i < 4; i++)
	{
		if (!std::getline(pFile, strElem))
			continue;

		if (i % 2 == 0)
			iElemCount = std::atoi(strElem.c_str());
		else
		{
			std::vector<std::string> vecElem = split(strElem, "|");

			if (i == 1)
				for (int j = 0; j < iElemCount; j++)
					m_vecEnableSliders.at(j) = std::atoi(vecElem.at(j).c_str());
			if (i == 3)
				for (int j = 0; j < iElemCount; j++)
					m_vecFunctions.at(j) = std::atoi(vecElem.at(j).c_str());
		}
	}

	std::getline(pFile, strElem);
	m_iHarmonicParity = std::atoi(strElem.c_str());

	std::getline(pFile, strElem);
	m_strHarmonicParity = strElem;

	std::getline(pFile, strElem);
	m_flLineWeigth = std::atof(strElem.c_str());
}

bool ConfigSystem::SaveConfig(std::string strConfigName)
{
	std::string strFilePath = m_strFolderPath + "\\" + strConfigName;

	if (strConfigName.find(".cfg") == std::string::npos)
		strFilePath += ".cfg";

	if (!std::filesystem::exists(m_strFolderPath))
		std::filesystem::create_directories(m_strFolderPath);

	std::ofstream pFile(strFilePath);
	if (!pFile.good())
		throw std::exception("Can't create file");

	std::string strConfig;
	GetConfigString(strConfig);

	pFile << strConfig;

	RefreshConfig();
	return true;
}

void ConfigSystem::GetConfigString(std::string& strOutput)
{
	strOutput = std::to_string(m_vecEnableSliders.size()) + '\n';
	for (auto bValue : m_vecEnableSliders)
	{
		strOutput += std::to_string((int)bValue) + "|";
	}
	
	strOutput.pop_back();
	strOutput += '\n';

	strOutput += std::to_string(m_vecFunctions.size()) + '\n';
	for (auto bValue : m_vecFunctions)
	{
		strOutput += std::to_string((int)bValue) + "|";
	}

	strOutput.pop_back();
	strOutput += '\n';

	strOutput += std::to_string(m_iHarmonicParity) + '\n';
	strOutput += m_strHarmonicParity + '\n';
	strOutput += std::to_string(m_flLineWeigth);
}

void ConfigSystem::UpdateDisplaySelectedConfig()
{
	if (!m_vecFiles.empty())
	{
		m_iSelectedFile = 0;
		m_strSelectedFile = m_vecFiles.at(0);
	}
	else
	{
		m_iSelectedFile = 0;
		m_strSelectedFile = "";
	}
}

bool ConfigSystem::RemoveConfig(std::string strConfigName)
{
	std::string strFilePath = m_strFolderPath + "\\" + strConfigName;

	if (strConfigName.find(".cfg") == std::string::npos)
		strFilePath += ".cfg";

	std::filesystem::remove(strFilePath);

	RefreshConfig();
	return true;
}

bool ConfigSystem::RefreshConfig()
{
	std::string strFolderName = DataPath();
	m_vecFiles.clear();

	for (const auto& entry : std::filesystem::directory_iterator(strFolderName))
		m_vecFiles.push_back(entry.path().filename().string());

	UpdateDisplaySelectedConfig();
	return true;
}
