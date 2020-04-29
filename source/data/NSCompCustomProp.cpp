#include "ee0/NSCompCustomProp.h"

#include <bs/ExportStream.h>
#include <bs/ImportStream.h>
#include <SM_Vector.h>
#include <painting0/Color.h>
#include <guard/check.h>

namespace ee0
{

size_t NSCompCustomProp::GetBinSize(const std::string& dir) const
{
	size_t sz = 0;

	sz += sizeof(uint8_t);		// num
	for (auto& prop : m_props)
	{
		sz += sizeof(uint8_t);		// type

		sz += bs::pack_size_str(prop.key.c_str());

		switch (prop.type)
		{
		case ee0::CompCustomProp::PROP_BOOL:
			sz += sizeof(uint8_t);
			break;
		case ee0::CompCustomProp::PROP_INT:
			sz += sizeof(uint32_t);
			break;
		case ee0::CompCustomProp::PROP_FLOAT:
			sz += sizeof(uint32_t);
			break;
		case ee0::CompCustomProp::PROP_STRING:
			sz += bs::pack_size_str(prop.val.m_val.pc);
			break;
		case ee0::CompCustomProp::PROP_VEC2:
			sz += sizeof(uint32_t) * 2;
			break;
		case ee0::CompCustomProp::PROP_COLOR:
			sz += sizeof(uint32_t);
			break;
		}
	}

	return sz;
}

void NSCompCustomProp::StoreToBin(const std::string& dir, bs::ExportStream& es) const
{
	GD_ASSERT(m_props.size() < std::numeric_limits<uint8_t>::max(), "overflow");
	es.Write(static_cast<uint8_t>(m_props.size()));
	for (auto& prop : m_props)
	{
		es.Write(static_cast<uint8_t>(prop.type));

		es.Write(prop.key);

		switch (prop.type)
		{
		case ee0::CompCustomProp::PROP_BOOL:
			es.Write(static_cast<uint8_t>(prop.val.m_val.bl));
			break;
		case ee0::CompCustomProp::PROP_INT:
			es.Write(static_cast<uint32_t>(prop.val.m_val.l));
			break;
		case ee0::CompCustomProp::PROP_FLOAT:
			es.Write(static_cast<uint32_t>(prop.val.m_val.flt));
			break;
		case ee0::CompCustomProp::PROP_STRING:
			es.Write(prop.val.m_val.pc);
			break;
		case ee0::CompCustomProp::PROP_VEC2:
			{
				auto& vec2 = *static_cast<const sm::vec2*>(prop.val.m_val.pv);
				es.Write(static_cast<uint32_t>(vec2.x));
				es.Write(static_cast<uint32_t>(vec2.y));
			}
			break;
		case ee0::CompCustomProp::PROP_COLOR:
			{
				auto& col = *static_cast<const pt0::Color*>(prop.val.m_val.pv);
				es.Write(static_cast<uint32_t>(col.ToRGBA()));
			}
			break;
		}
	}
}

void NSCompCustomProp::LoadFromBin(const ur::Device& dev, const std::string& dir, bs::ImportStream& is)
{
	size_t num = is.UInt8();
	m_props.resize(num);
	for (size_t i = 0; i < num; ++i)
	{
		auto& prop = m_props[i];

		prop.key = is.String();

		prop.type = static_cast<ee0::CompCustomProp::PropertyType>(is.UInt8());
		switch (prop.type)
		{
		case ee0::CompCustomProp::PROP_BOOL:
			prop.type = ee0::CompCustomProp::PROP_BOOL;
			prop.val.m_type = ee0::VT_BOOL;
			prop.val.m_val.bl = is.UInt8();
			break;
		case ee0::CompCustomProp::PROP_INT:
			prop.type = ee0::CompCustomProp::PROP_INT;
			prop.val.m_type = ee0::VT_LONG;
			prop.val.m_val.l = is.UInt32();
			break;
		case ee0::CompCustomProp::PROP_FLOAT:
			prop.type = ee0::CompCustomProp::PROP_FLOAT;
			prop.val.m_type = ee0::VT_FLOAT;
			prop.val.m_val.flt = is.Float();
			break;
		case ee0::CompCustomProp::PROP_STRING:
			{
				prop.type = ee0::CompCustomProp::PROP_STRING;
				prop.val.m_type = ee0::VT_PCHAR;

				auto str = is.String();
				char* tmp = new char[str.size() + 1];
				strcpy(tmp, str.c_str());
				tmp[str.size()] = '\0';
				prop.val.m_val.pc = tmp;
			}
			break;
		case ee0::CompCustomProp::PROP_VEC2:
			{
				prop.type = ee0::CompCustomProp::PROP_VEC2;
				prop.val.m_type = ee0::VT_PVOID;

				auto x = is.Float();
				auto y = is.Float();
				auto tmp = new sm::vec2(x, y);
				prop.val.m_val.pv = tmp;
			}
			break;
		case ee0::CompCustomProp::PROP_COLOR:
			{
				prop.type = ee0::CompCustomProp::PROP_COLOR;
				prop.val.m_type = ee0::VT_PVOID;

				uint32_t rgba = is.UInt32();
				auto tmp = new pt0::Color();
				tmp->FromRGBA(rgba);
				prop.val.m_val.pv = tmp;
			}
			break;
		}
	}
}

void NSCompCustomProp::StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const
{
	val.SetObject();

	rapidjson::Value props_val;
	props_val.SetArray();
	for (auto& prop : m_props)
	{
		rapidjson::Value prop_val;
		prop_val.SetObject();

		prop_val.AddMember("key", rapidjson::Value(prop.key.c_str(), alloc), alloc);

		switch (prop.type)
		{
		case ee0::CompCustomProp::PROP_BOOL:
			prop_val.AddMember("type", "bool", alloc);
			prop_val.AddMember("val", prop.val.m_val.bl, alloc);
			break;
		case ee0::CompCustomProp::PROP_INT:
			prop_val.AddMember("type", "int", alloc);
			prop_val.AddMember("val", prop.val.m_val.l, alloc);
			break;
		case ee0::CompCustomProp::PROP_FLOAT:
			prop_val.AddMember("type", "float", alloc);
			prop_val.AddMember("val", prop.val.m_val.flt, alloc);
			break;
		case ee0::CompCustomProp::PROP_STRING:
			prop_val.AddMember("type", "string", alloc);
			prop_val.AddMember("val", rapidjson::Value(prop.val.m_val.pc, alloc), alloc);
			break;
		case ee0::CompCustomProp::PROP_VEC2:
			{
				prop_val.AddMember("type", "vec2", alloc);

				auto& vec2 = *static_cast<const sm::vec2*>(prop.val.m_val.pv);
				rapidjson::Value vec2_val;
				vec2_val.SetObject();
				vec2_val.AddMember("x", vec2.x, alloc);
				vec2_val.AddMember("y", vec2.y, alloc);
				prop_val.AddMember("val", vec2_val, alloc);
			}
			break;
		case ee0::CompCustomProp::PROP_COLOR:
			{
				prop_val.AddMember("type", "color", alloc);

				auto& col = *static_cast<const pt0::Color*>(prop.val.m_val.pv);
				prop_val.AddMember("val", col.ToRGBA(), alloc);
			}
			break;
		}
		props_val.PushBack(prop_val, alloc);
	}

	val.AddMember("props", props_val, alloc);
}

void NSCompCustomProp::LoadFromJson(const ur::Device& dev, mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val)
{
	for (auto& prop_val : val["props"].GetArray())
	{
		auto type = std::string(prop_val["type"].GetString());
		auto key  = prop_val["key"].GetString();
		auto& val = prop_val["val"];

		ee0::CompCustomProp::Property prop;
		prop.key = key;
		if (type == "bool")
		{
			prop.type = ee0::CompCustomProp::PROP_BOOL;
			prop.val.m_type = ee0::VT_BOOL;
			prop.val.m_val.bl = val.GetBool();
		}
		else if (type == "int")
		{
			prop.type = ee0::CompCustomProp::PROP_INT;
			prop.val.m_type = ee0::VT_LONG;
			prop.val.m_val.l = val.GetInt();
		}
		else if (type == "float")
		{
			prop.type = ee0::CompCustomProp::PROP_FLOAT;
			prop.val.m_type = ee0::VT_FLOAT;
			prop.val.m_val.flt = val.GetFloat();
		}
		else if (type == "string")
		{
			prop.type = ee0::CompCustomProp::PROP_STRING;
			prop.val.m_type = ee0::VT_PCHAR;

			auto str = val.GetString();
			char* tmp = new char[strlen(str) + 1];
			strcpy(tmp, str);
			prop.val.m_val.pc = tmp;
		}
		else if (type == "vec2")
		{
			prop.type = ee0::CompCustomProp::PROP_VEC2;
			prop.val.m_type = ee0::VT_PVOID;

			auto x = val["x"].GetFloat();
			auto y = val["y"].GetFloat();
			auto tmp = new sm::vec2(x, y);
			prop.val.m_val.pv = tmp;
		}
		else if (type == "color")
		{
			prop.type = ee0::CompCustomProp::PROP_COLOR;
			prop.val.m_type = ee0::VT_PVOID;

			uint32_t rgba = val.GetUint();
			auto tmp = new pt0::Color();
			tmp->FromRGBA(rgba);
			prop.val.m_val.pv = tmp;
		}
		else
		{
			GD_REPORT_ASSERT("unknown type.");
		}

		m_props.push_back(prop);
	}
}

void NSCompCustomProp::StoreToMem(const ur::Device& dev, ee0::CompCustomProp& comp) const
{
	for (auto& prop : m_props) {
		comp.Add(prop);
	}
}

void NSCompCustomProp::LoadFromMem(const ee0::CompCustomProp& comp)
{
	m_props = comp.GetAllProp();
}

}