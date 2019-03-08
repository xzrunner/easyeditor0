#pragma once

#include <rttr/type>

class wxPropertyGrid;
class wxAny;
class wxString;

namespace ee0
{

struct UIMetaInfo;

class WxPropHelper
{
public:
	static void CreateProp(wxPropertyGrid* pg, const UIMetaInfo& info, rttr::instance obj, rttr::property prop,
        std::function<void(const std::string& filepath)> open_file_cb = nullptr);

	static void UpdateProp(const wxString& key, const wxAny& val, const UIMetaInfo& info,
		rttr::instance obj, rttr::property prop);

}; // WxPropHelper

}