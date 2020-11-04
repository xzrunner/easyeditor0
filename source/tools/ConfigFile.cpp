#include "ee0/ConfigFile.h"
#include "ee0/AssetsMap.h"

#include <js/RapidJsonHelper.h>

#include <wx/stdpaths.h>

#include <boost/filesystem.hpp>

namespace ee0
{

CU_SINGLETON_DEFINITION(ConfigFile)

static const char* FILENAME = "config.json";

ConfigFile::ConfigFile()
	: m_debug_draw(false)
	, m_draw_stat(false)
//	, m_use_opengl(false)
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

	auto& dir = boost::filesystem::path(filepath).parent_path();
	for (auto& val : doc["font"].GetArray())
	{
		std::string name = val["name"].GetString();
		std::string filepath = val["filepath"].GetString();
		auto full_path = boost::filesystem::absolute(filepath, dir);
		m_fonts.push_back(std::make_pair(name, full_path.string()));
	}

	for (auto& val : doc["user_font"].GetArray())
	{
		std::string name = val["name"].GetString();
		std::string filepath = val["filepath"].GetString();
		auto full_path = boost::filesystem::absolute(filepath, dir);
		m_user_fonts.push_back(std::make_pair(name, full_path.string()));
	}

    if (doc.HasMember("asset_dirs")) {
        for (auto& dir : doc["asset_dirs"].GetArray()) {
            AssetsMap::Instance()->LoadDirWithUnity(dir.GetString());
        }
    }

	if (doc.HasMember("debug_draw")) {
		m_debug_draw = doc["debug_draw"].GetBool();
	}
	if (doc.HasMember("draw_stat")) {
		m_draw_stat = doc["draw_stat"].GetBool();
	}
}

}