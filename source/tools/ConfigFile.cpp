#include "ee0/ConfigFile.h"

#include <js/RapidJsonHelper.h>

#include <wx/stdpaths.h>

#include <boost/filesystem.hpp>

namespace ee0
{

CU_SINGLETON_DEFINITION(ConfigFile)

static const char* FILENAME = "config.json";

ConfigFile::ConfigFile()
{
	auto curr_path = boost::filesystem::current_path().string();
	auto cfg_path = boost::filesystem::absolute(FILENAME, curr_path);
	if (boost::filesystem::exists(cfg_path)) {
		LoadFromFile(cfg_path.string());
	}
}

void ConfigFile::LoadFromFile(const std::string& filepath)
{
	rapidjson::Document doc;
	js::RapidJsonHelper::ReadFromFile(filepath.c_str(), doc);

	for (auto& val : doc["font"].GetArray()) 
	{
		std::string name = val["name"].GetString();
		std::string filepath = val["filepath"].GetString();
		m_fonts.push_back(std::make_pair(name, filepath));
	}

	for (auto& val : doc["user_font"].GetArray())
	{
		std::string name = val["name"].GetString();
		std::string filepath = val["filepath"].GetString();
		m_user_fonts.push_back(std::make_pair(name, filepath));
	}
}

}