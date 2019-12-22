#pragma once

#include <rttr/type>

class wxPropertyGrid;
class wxAny;
class wxString;
class wxEnumProperty;

namespace ee0
{

struct UIMetaInfo;

class WxPropHelper
{
public:
	static void CreateProp(wxPropertyGrid* pg, const UIMetaInfo& info, rttr::instance obj, rttr::property prop,
        std::function<void(const std::string& filepath)> open_file_cb = nullptr);

	static bool UpdateProp(const wxString& key, const wxAny& val, const UIMetaInfo& info,
		rttr::instance obj, rttr::property prop);

    static wxEnumProperty* CreateEnumProp(const std::string& label, rttr::type type, int init_val);
    static rttr::variant QueryEnumPropByLabel(const std::string& label, rttr::type type);

}; // WxPropHelper

}