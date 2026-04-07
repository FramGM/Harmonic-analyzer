#include "ConfigSystem.h"
#include <fstream>
#include <Windows.h>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <filesystem>

inline std::vector<std::string> split(std::string s, std::string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

static std::string DataPath() {
	char la[MAX_PATH];
	if (GetEnvironmentVariableA("LOCALAPPDATA", la, MAX_PATH) == 0)
		return {};
	std::string dir = std::string(la) + "\\HarmonicAalyzer\\";
	CreateDirectoryA(dir.c_str(), nullptr);
	return dir;
}

ConfigSystem::ConfigSystem() {
	m_vecSliderNames = { "Частота", "Амплитуда", "Фаза", "Толщина графика", "Высота графика", "Длинна волны" };
	m_vecEnableSliders = std::vector<bool>(m_vecSliderNames.size(), false);
	m_strHarmonicParity = "Все";
}

bool ConfigSystem::LoadConfig(std::string strConfigName)
{
	std::string strFolderPath = DataPath();
	std::string strFilePath = strFolderPath + "\\" + strConfigName;

	if (strConfigName.find(".cfg") == std::string::npos)
		strFilePath += ".cfg";

	std::ifstream pFile(strFilePath);
	if (!pFile.is_open() || pFile.bad())
		throw std::exception("Can't open file");

	std::string strElem;
	while (std::getline(pFile, strElem))
	{
		std::vector<std::string> vecElem = split(strElem, "|");

		int i = 0;
		{
			std::string strElem = vecElem.at(i);

			int iCount = std::atoi(strElem.c_str());
			for (; i + 1 < iCount; i++)
				m_vecEnableSliders.at(i) = std::atoi(vecElem.at(i + 1).c_str());

			strElem = vecElem.at(i);
			iCount = std::atoi(strElem.c_str());
			for (; i + 1 < iCount; i++)
				m_vecFunctions.at(i) = std::atoi(vecElem.at(i + 1).c_str());

			strElem = vecElem.at(i);
			m_iHarmonicParity = std::atoi(strElem.c_str());
			i++;

			strElem = vecElem.at(i);
			m_strHarmonicParity = strElem;
			i++;

			strElem = vecElem.at(i);
			m_flLineWeigth = std::atof(strElem.c_str());
		}
	}
}

bool ConfigSystem::SaveConfig(std::string strConfigName)
{
	std::string strFolderPath = DataPath();
	std::string strFilePath = strFolderPath + "\\" + strConfigName;

	if (strConfigName.find(".cfg") == std::string::npos)
		strFilePath += ".cfg";

	if (!std::filesystem::exists(strFolderPath))
		std::filesystem::create_directories(strFolderPath);

	std::ofstream pFile(strFilePath);
	if (!pFile.good())
		throw std::exception("Can't create file");

	std::string strConfig;
	GetConfigString(strConfig);

	pFile << strConfig;
	return true;
}

void ConfigSystem::GetConfigString(std::string& strOutput)
{
	strOutput = std::to_string(m_vecEnableSliders.size()) + "|";
	for (auto bValue : m_vecEnableSliders)
	{
		strOutput += (int)bValue + "|";
	}

	strOutput += std::to_string(m_vecFunctions.size()) + "|";
	for (auto bValue : m_vecFunctions)
	{
		strOutput += (int)bValue + "|";
	}

	strOutput += std::to_string(m_iHarmonicParity) + "|";
	strOutput += m_strHarmonicParity + "|";
	strOutput += std::to_string(m_flLineWeigth);
}