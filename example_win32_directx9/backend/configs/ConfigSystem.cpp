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

bool ConfigSystem::LoadConfig(std::string strConfigName)
{
	std::string strFolderPath = DataPath();
	std::string strFilePath = strFolderPath + "\\" + strConfigName;

	if (strConfigName.find(".cfg") == std::string::npos)
		strFilePath += ".cfg";

	std::ifstream pFile(strFilePath);
	if (!pFile.is_open() || pFile.bad())
		throw std::exception("Can't open file");



}

bool ConfigSystem::SaveConfig(std::string strConfigName)
{
	std::string strFolderPath = DataPath();
	std::string strFilePath = strFolderPath + "\\" + strConfigName;

	if (strConfigName.find(".cfg") == std::string::npos)
		strFilePath += ".cfg";

	if (!std::filesystem::exists(strFolderPath))
		std::filesystem::create_directories(strFolderPath);

	std::ifstream pReadFile(strFilePath);
	if (!pReadFile.good()) 
		throw std::exception("Can't create file");
	
	std::string strConfig;
	
	
}