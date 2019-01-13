#include "ee0/WxPropHelper.h"
#include "ee0/ReflectPropTypes.h"
#include "ee0/WxOpenFileProp.h"

#include <SM_Vector.h>

#include <wx/propgrid/propgrid.h>

namespace ee0
{

void WxPropHelper::CreateProp(wxPropertyGrid* pg, const UIMetaInfo& info,
	                          rttr::instance obj, rttr::property prop,
                              std::function<void()> open_file_cb)
{
	auto type = prop.get_type();
	if (type == rttr::type::get<bool>())
	{
		auto b = prop.get_value(obj).get_value<bool>();
		pg->Append(new wxBoolProperty(info.desc, wxPG_LABEL, b));
		pg->SetPropertyAttribute(info.desc.c_str(), wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
	}
	else if (type == rttr::type::get<sm::bvec2>())
	{
		auto b = prop.get_value(obj).get_value<sm::bvec2>();
		pg->Append(new wxFloatProperty("R", wxPG_LABEL, b.x));
		pg->Append(new wxFloatProperty("G", wxPG_LABEL, b.y));
		pg->SetPropertyAttribute("R", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
		pg->SetPropertyAttribute("G", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
	}
	else if (type == rttr::type::get<float>())
	{
		auto v = prop.get_value(obj).get_value<float>();
		pg->Append(new wxFloatProperty(info.desc, wxPG_LABEL, v));
	}
	else if (type == rttr::type::get<sm::vec2>())
	{
		auto v = prop.get_value(obj).get_value<sm::vec2>();
		pg->Append(new wxFloatProperty("R", wxPG_LABEL, v.x));
		pg->Append(new wxFloatProperty("G", wxPG_LABEL, v.y));
	}
	else if (type == rttr::type::get<sm::vec3>())
	{
		auto v = prop.get_value(obj).get_value<sm::vec3>();
		pg->Append(new wxFloatProperty("R", wxPG_LABEL, v.x));
		pg->Append(new wxFloatProperty("G", wxPG_LABEL, v.y));
		pg->Append(new wxFloatProperty("B", wxPG_LABEL, v.z));
	}
	else if (type == rttr::type::get<sm::vec4>())
	{
		auto v = prop.get_value(obj).get_value<sm::vec4>();
		pg->Append(new wxFloatProperty("R", wxPG_LABEL, v.x));
		pg->Append(new wxFloatProperty("G", wxPG_LABEL, v.y));
		pg->Append(new wxFloatProperty("B", wxPG_LABEL, v.z));
		pg->Append(new wxFloatProperty("A", wxPG_LABEL, v.w));
	}
	else if (type == rttr::type::get<const char*>()
	      || type == rttr::type::get<std::string>())
	{
		bool is_char = type == rttr::type::get<const char*>();
		std::string str = (type == rttr::type::get<const char*>()) ?
			prop.get_value(obj).get_value<const char*>() :
			prop.get_value(obj).to_string();

		auto open_file_obj = prop.get_metadata(PropOpenFileTag());
		if (open_file_obj.is_valid())
		{
			auto open_file = open_file_obj.get_value<PropOpenFile>();
			auto wx_prop = new WxOpenFileProp(info.desc, wxPG_LABEL, str);
			wx_prop->SetFilter(open_file.filter);
			std::string prop_name = prop.get_name().to_string();
			wx_prop->SetCallback([=](const std::string& filepath)
			{
				if (type == rttr::type::get<const char*>()) {
					prop.set_value(obj, filepath.c_str());
				} else {
					prop.set_value(obj, filepath);
				}
                if (open_file_cb) {
                    open_file_cb();
                }
			});

			pg->Append(wx_prop);
		}
		else
		{
            if (prop.get_metadata(PropLongStringTag()).is_valid()) {
                pg->Append(new wxLongStringProperty(info.desc, wxPG_LABEL, str));
            } else {
                pg->Append(new wxStringProperty(info.desc, wxPG_LABEL, str));
            }
		}
	}
}

void WxPropHelper::UpdateProp(const wxString& key, const wxAny& val, const UIMetaInfo& info,
	                          rttr::instance obj, rttr::property prop)
{
	auto type = prop.get_type();
	if (type == rttr::type::get<bool>() && key == info.desc)
	{
		prop.set_value(obj, wxANY_AS(val, bool));
	}
	else if (type == rttr::type::get<sm::bvec2>() && (key == "R" || key == "G"))
	{
		auto b = prop.get_value(obj).get_value<sm::bvec2>();
		if (key == "R") {
			b.x = wxANY_AS(val, bool);
		} else if (key == "G") {
			b.y = wxANY_AS(val, bool);
		}
		prop.set_value(obj, b);
	}
	else if (type == rttr::type::get<float>() && key == info.desc)
	{
		prop.set_value(obj, wxANY_AS(val, float));
	}
	else if (type == rttr::type::get<sm::vec2>() && (key == "R" || key == "G"))
	{
		auto v = prop.get_value(obj).get_value<sm::vec2>();
		if (key == "R") {
			v.x = wxANY_AS(val, float);
		} else if (key == "G") {
			v.y = wxANY_AS(val, float);
		}
		prop.set_value(obj, v);
	}
	else if (type == rttr::type::get<sm::vec3>() && (key == "R" || key == "G" || key == "B"))
	{
		auto v = prop.get_value(obj).get_value<sm::vec3>();
		if (key == "R") {
			v.x = wxANY_AS(val, float);
		} else if (key == "G") {
			v.y = wxANY_AS(val, float);
		} else if (key == "B") {
			v.z = wxANY_AS(val, float);
		}
		prop.set_value(obj, v);
	}
	else if (type == rttr::type::get<sm::vec4>() && (key == "R" || key == "G" || key == "B" || key == "A"))
	{
		auto v = prop.get_value(obj).get_value<sm::vec4>();
		if (key == "R") {
			v.x = wxANY_AS(val, float);
		} else if (key == "G") {
			v.y = wxANY_AS(val, float);
		} else if (key == "B") {
			v.z = wxANY_AS(val, float);
		} else if (key == "A") {
			v.w = wxANY_AS(val, float);
		}
		prop.set_value(obj, v);
	}
	else if (type == rttr::type::get<const char*>() && key == info.desc)
	{
		prop.set_value(obj, wxANY_AS(val, wxString).ToStdString());
	}
	else if (type == rttr::type::get<std::string>() && key == info.desc)
	{
		prop.set_value(obj, wxANY_AS(val, wxString).ToStdString());
	}
}

}