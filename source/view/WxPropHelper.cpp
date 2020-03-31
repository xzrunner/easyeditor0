#include "ee0/WxPropHelper.h"
#include "ee0/ReflectPropTypes.h"
#include "ee0/WxOpenFileProp.h"
#include "ee0/WxEditCodeProp.h"

#include <SM_Vector.h>
#include <cpputil/StringHelper.h>
#include <painting0/Color.h>

#include <wx/propgrid/propgrid.h>

#include <cctype>

namespace
{

std::string UnderscoreToCamelCase(const std::string& str)
{
    std::vector<std::string> tokens;
    cpputil::StringHelper::Split(str, "_", tokens);

    std::string ret;
    for (auto& sub : tokens)
    {
        if (sub.empty()) {
            continue;
        }
        sub[0] = std::toupper(sub[0]);
        if (!ret.empty()) {
            ret += " ";
        }
        ret += sub;
    }
    return ret;
}

}

namespace ee0
{

void WxPropHelper::CreateProp(wxPropertyGrid* pg, const UIMetaInfo& info, rttr::instance obj, rttr::property prop,
                              std::function<void(const std::string& filepath)> open_file_cb,
                              std::function<void(const std::string& code_str)> edit_code_cb,
                              wxPGProperty* parent)
{
	auto type = prop.get_type();
	if (type == rttr::type::get<bool>())
	{
		auto b = prop.get_value(obj).get_value<bool>();
        auto c_prop = new wxBoolProperty(info.desc, wxPG_LABEL, b);
        if (parent) {
            pg->AppendIn(parent, c_prop);
        } else {
            pg->Append(c_prop);
        }
		pg->SetPropertyAttribute(c_prop, wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
	}
	else if (type == rttr::type::get<sm::bvec2>())
	{
		auto b = prop.get_value(obj).get_value<sm::bvec2>();
        auto x_prop = new wxFloatProperty("X", wxPG_LABEL, b.x);
        auto y_prop = new wxFloatProperty("Y", wxPG_LABEL, b.y);
        if (parent) {
            pg->AppendIn(parent, x_prop);
            pg->AppendIn(parent, y_prop);
        } else {
            pg->Append(x_prop);
            pg->Append(y_prop);
        }
		pg->SetPropertyAttribute(x_prop, wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
		pg->SetPropertyAttribute(y_prop, wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
	}
    else if (type == rttr::type::get<int>())
    {
        auto v = prop.get_value(obj).get_value<int>();
        auto c_prop = new wxIntProperty(info.desc, wxPG_LABEL, v);
        if (parent) {
            pg->AppendIn(parent, c_prop);
        } else {
            pg->Append(c_prop);
        }
    }
    else if (type == rttr::type::get<unsigned int>())
    {
        auto v = prop.get_value(obj).get_value<unsigned int>();
        auto c_prop = new wxUIntProperty(info.desc, wxPG_LABEL, v);
        if (parent) {
            pg->AppendIn(parent, c_prop);
        } else {
            pg->Append(c_prop);
        }
    }
	else if (type == rttr::type::get<float>())
	{
		auto v = prop.get_value(obj).get_value<float>();
        auto c_prop = new wxFloatProperty(info.desc, wxPG_LABEL, v);
        if (parent) {
            pg->AppendIn(parent, c_prop);
        } else {
            pg->Append(c_prop);
        }
	}
	else if (type == rttr::type::get<sm::vec2>())
	{
		auto v = prop.get_value(obj).get_value<sm::vec2>();
        auto c_prop = new wxStringProperty(info.desc, wxPG_LABEL, wxT("<composed>"));
        if (parent) {
            pg->AppendIn(parent, c_prop);
        } else {
            pg->Append(c_prop);
        }
        c_prop->SetExpanded(false);
        pg->AppendIn(c_prop, new wxFloatProperty(wxT("X"), wxPG_LABEL, v.x));
        pg->AppendIn(c_prop, new wxFloatProperty(wxT("Y"), wxPG_LABEL, v.y));
	}
	else if (type == rttr::type::get<sm::vec3>())
	{
		auto v = prop.get_value(obj).get_value<sm::vec3>();
        auto c_prop = new wxStringProperty(info.desc, wxPG_LABEL, wxT("<composed>"));
        if (parent) {
            pg->AppendIn(parent, c_prop);
        } else {
            pg->Append(c_prop);
        }
        c_prop->SetExpanded(false);
        pg->AppendIn(c_prop, new wxFloatProperty(wxT("X"), wxPG_LABEL, v.x));
        pg->AppendIn(c_prop, new wxFloatProperty(wxT("Y"), wxPG_LABEL, v.y));
        pg->AppendIn(c_prop, new wxFloatProperty(wxT("Z"), wxPG_LABEL, v.z));
	}
	else if (type == rttr::type::get<sm::vec4>())
	{
		auto v = prop.get_value(obj).get_value<sm::vec4>();
        auto c_prop = new wxStringProperty(info.desc, wxPG_LABEL, wxT("<composed>"));
        if (parent) {
            pg->AppendIn(parent, c_prop);
        } else {
            pg->Append(c_prop);
        }
        c_prop->SetExpanded(false);
        pg->AppendIn(c_prop, new wxFloatProperty(wxT("X"), wxPG_LABEL, v.x));
        pg->AppendIn(c_prop, new wxFloatProperty(wxT("Y"), wxPG_LABEL, v.y));
        pg->AppendIn(c_prop, new wxFloatProperty(wxT("Z"), wxPG_LABEL, v.z));
        pg->AppendIn(c_prop, new wxFloatProperty(wxT("W"), wxPG_LABEL, v.w));
	}
	else if (type == rttr::type::get<const char*>()
	      || type == rttr::type::get<std::string>())
	{
		bool is_char = type == rttr::type::get<const char*>();
		std::string str = (type == rttr::type::get<const char*>()) ?
			prop.get_value(obj).get_value<const char*>() :
			prop.get_value(obj).to_string();

        // open file
		auto open_file_obj = prop.get_metadata(PropOpenFileTag());
		if (open_file_obj.is_valid())
		{
			auto open_file = open_file_obj.get_value<PropOpenFile>();
			auto c_prop = new WxOpenFileProp(info.desc, wxPG_LABEL, str);
			c_prop->SetFilter(open_file.filter);
			std::string prop_name = prop.get_name().to_string();
			c_prop->SetCallback([=](const std::string& filepath)
			{
				if (type == rttr::type::get<const char*>()) {
					prop.set_value(obj, filepath.c_str());
				} else {
					prop.set_value(obj, filepath);
				}
                if (open_file_cb) {
                    open_file_cb(filepath);
                }
			});

            if (parent) {
                pg->AppendIn(parent, c_prop);
            } else {
                pg->Append(c_prop);
            }

            return;
		}

        // cdoe edit
        if (prop.get_metadata(PropEditCodeTag()).is_valid())
        {
            auto c_prop = new WxEditCodeProp(info.desc, wxPG_LABEL, str);
            c_prop->SetCallback([=](const std::string& code_str)
            {
                if (type == rttr::type::get<const char*>()) {
                    prop.set_value(obj, code_str.c_str());
                } else {
                    prop.set_value(obj, code_str);
                }
                if (edit_code_cb) {
                    edit_code_cb(code_str);
                }
            });

            if (parent) {
                pg->AppendIn(parent, c_prop);
            } else {
                pg->Append(c_prop);
            }

            return;
        }

        // string
        str = cpputil::StringHelper::UTF8ToGBK(str.c_str());
        if (prop.get_metadata(PropLongStringTag()).is_valid())
        {
            auto c_prop = new wxLongStringProperty(info.desc, wxPG_LABEL, str);
            if (parent) {
                pg->AppendIn(parent, c_prop);
            } else {
                pg->Append(c_prop);
            }
        }
        else
        {
            auto c_prop = new wxStringProperty(info.desc, wxPG_LABEL, str);
            if (parent) {
                pg->AppendIn(parent, c_prop);
            } else {
                pg->Append(c_prop);
            }
        }
	}
    else if (type == rttr::type::get<pt0::Color>())
    {
        auto col = prop.get_value(obj).get_value<pt0::Color>();
        auto c_prop = new wxStringProperty(info.desc, wxPG_LABEL, wxT("<composed>"));
        if (parent) {
            pg->AppendIn(parent, c_prop);
        } else {
            pg->Append(c_prop);
        }
        c_prop->SetExpanded(false);
        pg->AppendIn(c_prop, new wxUIntProperty(wxT("R"), wxPG_LABEL, col.r));
        pg->AppendIn(c_prop, new wxUIntProperty(wxT("G"), wxPG_LABEL, col.g));
        pg->AppendIn(c_prop, new wxUIntProperty(wxT("B"), wxPG_LABEL, col.b));
        pg->AppendIn(c_prop, new wxUIntProperty(wxT("A"), wxPG_LABEL, col.a));
    }
    else if (type.is_enumeration())
    {
        auto c_prop = CreateEnumProp(info.desc, type, prop.get_value(obj).get_value<int>());
        if (parent) {
            pg->AppendIn(parent, c_prop);
        } else {
            pg->Append(c_prop);
        }
    }
    else if (type.is_sequential_container())
    {
        wxPGProperty* c_prop = nullptr;

        auto var = prop.get_value(obj);
        auto view = var.create_sequential_view();
        auto val_type = view.get_value_type();
        if (val_type.is_enumeration())
        {
            wxArrayString labels;
            wxArrayInt values;

            auto enum_vals = val_type.get_enumeration().get_values();
            labels.resize(enum_vals.size());
            values.resize(enum_vals.size());
            for (auto& v : enum_vals)
            {
                auto idx = v.to_int();
                auto desc = val_type.get_enumeration().get_metadata(idx);
                assert(desc.is_valid());
                labels[idx] = desc.to_string();
                values[idx] = 1 << idx;
            }

            int value = 0;
            for (size_t i = 0, n = view.get_size(); i < n; ++i) {
                value |= (1 << view.get_value(i).to_int());
            }

            c_prop = new wxFlagsProperty(info.desc, wxPG_LABEL, labels, values, value);
        }
        else
        {
            wxArrayString str_array;

            const auto num = view.get_size();
            str_array.reserve(num);
            for (size_t i = 0, n = num; i < n; ++i) {
                str_array.push_back(VarToStr(view.get_value(i),
                    true, view.get_value_type()));
            }

            c_prop = new wxArrayStringProperty(info.desc, wxPG_LABEL, str_array);
        }

        if (parent) {
            pg->AppendIn(parent, c_prop);
        } else {
            pg->Append(c_prop);
        }
    }
    else if (type.is_associative_container())
    {
    }
    else // composed
    {
        auto props = type.get_properties();
        if (!props.empty())
        {
            auto c_obj = prop.get_value(obj);

            wxPGProperty* p_prop = pg->Append(new wxStringProperty(info.desc, wxPG_LABEL, wxT("<composed>")));
            p_prop->SetExpanded(false);
            for (auto& c_prop : props) {
                auto desc = UnderscoreToCamelCase(c_prop.get_name().to_string());
                CreateProp(pg, UIMetaInfo(desc), c_obj, c_prop, open_file_cb, edit_code_cb, p_prop);
            }
        }
    }
}

bool WxPropHelper::UpdateProp(const wxString& key, const wxAny& val, const UIMetaInfo& info,
	                          rttr::instance obj, rttr::property prop)
{
    bool dirty = true;
	auto type = prop.get_type();
	if (type == rttr::type::get<bool>() && key == info.desc)
	{
		prop.set_value(obj, wxANY_AS(val, bool));
	}
	else if (type == rttr::type::get<sm::bvec2>() && (key == "X" || key == "Y"))
	{
		auto b = prop.get_value(obj).get_value<sm::bvec2>();
		if (key == "X") {
			b.x = wxANY_AS(val, bool);
		} else if (key == "Y") {
			b.y = wxANY_AS(val, bool);
		}
		prop.set_value(obj, b);
	}
    else if (type == rttr::type::get<int>() && key == info.desc)
    {
        prop.set_value(obj, wxANY_AS(val, int));
    }
    else if (type == rttr::type::get<unsigned int>() && key == info.desc)
    {
        //// fixme: wx runtime error
        //size_t val = wxANY_AS(val, size_t);
        size_t v = wxANY_AS(val, int);
        prop.set_value(obj, v);
    }
	else if (type == rttr::type::get<float>() && key == info.desc)
	{
		prop.set_value(obj, wxANY_AS(val, float));
	}
	else if (type == rttr::type::get<sm::vec2>() && key == info.desc)
	{
        std::vector<std::string> tokens;
        auto str = wxANY_AS(val, wxString).ToStdString();
        cpputil::StringHelper::Split(str, ";", tokens);
        assert(tokens.size() == 2);

		auto v = prop.get_value(obj).get_value<sm::vec2>();
        v.x = std::stof(tokens[0]);
        v.y = std::stof(tokens[1]);

		prop.set_value(obj, v);
	}
	else if (type == rttr::type::get<sm::vec3>() && key == info.desc)
	{
        std::vector<std::string> tokens;
        auto str = wxANY_AS(val, wxString).ToStdString();
        cpputil::StringHelper::Split(str, ";", tokens);
        assert(tokens.size() == 3);

        auto v = prop.get_value(obj).get_value<sm::vec3>();
        v.x = std::stof(tokens[0]);
        v.y = std::stof(tokens[1]);
        v.z = std::stof(tokens[2]);

		prop.set_value(obj, v);
	}
	else if (type == rttr::type::get<sm::vec4>() && key == info.desc)
	{
        std::vector<std::string> tokens;
        auto str = wxANY_AS(val, wxString).ToStdString();
        cpputil::StringHelper::Split(str, ";", tokens);
        assert(tokens.size() == 4);

		auto v = prop.get_value(obj).get_value<sm::vec4>();
        v.x = std::stof(tokens[0]);
        v.y = std::stof(tokens[1]);
        v.z = std::stof(tokens[2]);
        v.w = std::stof(tokens[3]);

		prop.set_value(obj, v);
	}
	else if (type == rttr::type::get<const char*>() && key == info.desc)
	{
        auto str = wxANY_AS(val, wxString).ToStdString();
        str = cpputil::StringHelper::GBKToUTF8(str.c_str());
		prop.set_value(obj, str.c_str());
	}
	else if (type == rttr::type::get<std::string>() && key == info.desc)
	{
        auto str = wxANY_AS(val, wxString).ToStdString();
        str = cpputil::StringHelper::GBKToUTF8(str.c_str());
        prop.set_value(obj, str);
	}
    else if (type == rttr::type::get<pt0::Color>() && key == info.desc)
    {
        std::vector<std::string> tokens;
        auto str = wxANY_AS(val, wxString).ToStdString();
        cpputil::StringHelper::Split(str, ";", tokens);
        assert(tokens.size() == 4);

        auto col = prop.get_value(obj).get_value<pt0::Color>();
        col.r = std::stoi(tokens[0]);
        col.g = std::stoi(tokens[1]);
        col.b = std::stoi(tokens[2]);
        col.a = std::stoi(tokens[3]);

		prop.set_value(obj, col);
    }
    else if (type.is_enumeration() && key == info.desc)
    {
        if (val.CheckType<int>())
        {
            auto t = val.GetType();
            auto idx = wxANY_AS(val, int);
            auto vars = type.get_enumeration().get_values();
            assert(idx >= 0 && idx < static_cast<int>(vars.size()));
            bool find = false;
            for (auto& var : vars)
            {
                if (var.to_int() != idx) {
                    continue;
                }

                prop.set_value(obj, var);
                find = true;
                dirty = true;
                break;
            }
            assert(find);
        }
    }
    else if (type.is_sequential_container() && key == info.desc)
    {
        auto var = prop.get_value(obj);
        auto view = var.create_sequential_view();
        assert(view.is_valid());
        auto val_type = view.get_value_type();
        if (val_type.is_enumeration())
        {
            auto bitset = wxANY_AS(val, int);

            view.clear();
            auto enum_vals = val_type.get_enumeration().get_values();
            for (auto& v : enum_vals)
            {
                auto idx = v.to_int();
                if (bitset & (1 << idx)) {
                    view.insert(view.end(), v);
                }
            }
        }
        else
        {
            auto array_str = wxANY_AS(val, wxArrayString);
            view.set_size(array_str.size());
            for (size_t i = 0, n = array_str.size(); i < n; ++i)
            {
                auto val = StrToVar(array_str[i].ToStdString(), view.get_value_type());
                view.set_value(i, val);
            }
        }
        prop.set_value(obj, var);
    }
    else if (key == info.desc)  // composed
    {
        auto props = type.get_properties();

        std::vector<std::string> tokens;
        auto str = wxANY_AS(val, wxString).ToStdString();
        cpputil::StringHelper::Split(str, ";", tokens);
        assert(tokens.size() == props.size());

        auto value = prop.get_value(obj);
        size_t ptr = 0;
        for (auto& dst : props) {
            dst.set_value(value, StrToVar(tokens[ptr++], dst.get_type()));
        }
        prop.set_value(obj, value);
    }
    else
    {
        dirty = false;
    }
    return dirty;
}

wxEnumProperty* WxPropHelper::CreateEnumProp(const std::string& label, rttr::type type, int init_val)
{
    wxArrayString choices;

    auto vars = type.get_enumeration().get_values();
    choices.resize(vars.size());
    for (auto& var : vars)
    {
        auto idx = var.to_int();
        auto desc = type.get_enumeration().get_metadata(idx);
        assert(desc.is_valid());
        choices[idx] = desc.to_string();
    }

    auto wx_prop = new wxEnumProperty(label, wxPG_LABEL, choices);
    wx_prop->SetValue(init_val);
    return wx_prop;
}

rttr::variant WxPropHelper::QueryEnumPropByLabel(const std::string& label, rttr::type type)
{
    auto vars = type.get_enumeration().get_values();
    for (auto& var : vars)
    {
        auto idx = var.to_int();
        auto desc = type.get_enumeration().get_metadata(idx);
        assert(desc.is_valid());
        if (label == desc.to_string()) {
            return var;
        }
    }
    assert(0);
    return rttr::variant();
}

rttr::variant WxPropHelper::StrToVar(const std::string& str, rttr::type type)
{
    if (type == rttr::type::get<bool>())
    {
        if (str == "0" || str.find("Not") != std::string::npos) {
            return false;
        } else {
            return true;
        }
    }
    else if (type == rttr::type::get<int>())
    {
        return std::stoi(str);
    }
    else if (type == rttr::type::get<unsigned int>())
    {
        return std::stoul(str);
    }
    else if (type == rttr::type::get<float>())
    {
        return std::stof(str);
    }
    else if (type == rttr::type::get<const char*>()
          || type == rttr::type::get<std::string>())
    {
        return cpputil::StringHelper::GBKToUTF8(str.c_str());
    }
    else if (type.is_enumeration())
    {
        return type.get_enumeration().name_to_value(str);
    }
    else
    {
        auto var = type.create();

        auto props = type.get_properties();

        std::vector<std::string> tokens;
        cpputil::StringHelper::Split(str, ";", tokens);
        if (tokens.size() != props.size()) {
            return rttr::variant();
        }

        size_t ptr = 0;
        for (auto& dst : props) {
            dst.set_value(var, StrToVar(tokens[ptr++], dst.get_type()));
        }

        assert(var.is_valid());
        return var;
    }
}

std::string WxPropHelper::VarToStr(rttr::variant var, bool wrapped, rttr::type type)
{
    if (type == rttr::type::get<bool>())
    {
        auto val = wrapped ? var.get_wrapped_value<bool>() : var.get_value<bool>();
        return std::to_string(val);
    }
    else if (type == rttr::type::get<int>())
    {
        auto val = wrapped ? var.get_wrapped_value<int>() : var.get_value<int>();
        return std::to_string(val);
    }
    else if (type == rttr::type::get<unsigned int>())
    {
        auto val = wrapped ? var.get_wrapped_value<unsigned int>() : var.get_value<unsigned int>();
        return std::to_string(val);
    }
    else if (type == rttr::type::get<float>())
    {
        auto val = wrapped ? var.get_wrapped_value<float>() : var.get_value<float>();
        return std::to_string(val);
    }
    else if (type == rttr::type::get<const char*>()
          || type == rttr::type::get<std::string>())
    {
		return (type == rttr::type::get<const char*>()) ?
			var.get_value<const char*>() : var.to_string();
    }
    else if (type.is_enumeration())
    {
        return type.get_enumeration().value_to_name(var).to_string();
    }
    else
    {
        std::string ret;
        for (auto& prop : type.get_properties()) {
            ret += VarToStr(prop.get_value(var), false, prop.get_type());
            ret += ";";
        }
        if (!ret.empty()) {
            ret.pop_back();
        }
        return ret;
    }
}

}