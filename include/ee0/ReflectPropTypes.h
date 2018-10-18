#pragma once

#include <string>

namespace ee0
{

static const char* UIMetaInfoTag() {
	return "UI_META_INFO";
}

static const char* PropOpenFileTag() {
	return "PROP_OPEN_FILE";
}

struct UIMetaInfo
{
	UIMetaInfo(std::string desc)
		: desc(std::move(desc))
	{
	}

	std::string tips;
	std::string desc;
};

struct PropOpenFile
{
	PropOpenFile(std::string filter)
		: filter(std::move(filter))
	{
	}

	std::string filter;
};

}