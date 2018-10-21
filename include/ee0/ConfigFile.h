#pragma once

#include <cu/cu_macro.h>

#include <rapidjson/document.h>

#include <vector>
#include <string>

namespace ee0
{

class ConfigFile
{
public:
	auto& GetFonts() const { return m_fonts; }
	auto& GetUserFonts() const { return m_user_fonts; }

	bool GetDebugDraw() const { return m_debug_draw; }

private:
	void LoadFromFile(const std::string& filepath);

private:
	std::vector<std::pair<std::string, std::string>> m_fonts;
	std::vector<std::pair<std::string, std::string>> m_user_fonts;

	bool m_debug_draw;

	bool m_draw_stat;

private:
	CU_SINGLETON_DECLARATION(ConfigFile)

}; // ConfigFile

}