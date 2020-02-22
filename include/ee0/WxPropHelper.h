#pragma once

#include <rttr/type>

class wxPropertyGrid;
class wxAny;
class wxString;
class wxEnumProperty;
class wxPGProperty;

namespace ee0
{

struct UIMetaInfo;

class WxPropHelper
{
public:
	static void CreateProp(wxPropertyGrid* pg, const UIMetaInfo& info, rttr::instance obj, rttr::property prop,
        std::function<void(const std::string& filepath)> open_file_cb = nullptr,
        std::function<void(const std::string& code_str)> edit_code_cb = nullptr,
        wxPGProperty* parent = nullptr);

	 static bool UpdateProp(const wxString& key, const wxAny& val, const UIMetaInfo& info,
		rttr::instance obj, rttr::property prop);

    static wxEnumProperty* CreateEnumProp(const std::string& label, rttr::type type, int init_val);
    static rttr::variant QueryEnumPropByLabel(const std::string& label, rttr::type type);

private:
    static rttr::variant StrToVar(const std::string& str, rttr::type type);
    static std::string VarToStr(rttr::variant var, bool wrapped, rttr::type type);

}; // WxPropHelper

}