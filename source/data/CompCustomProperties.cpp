#include "ee0/CompCustomProperties.h"

#include <SM_Vector.h>
#include <painting2/Color.h>

namespace ee0
{

const char* const CompCustomProperties::TYPE_NAME = "ee0_custom_properties";

CompCustomProperties::CompCustomProperties()
{
}

std::unique_ptr<n0::NodeUniqueComp> CompCustomProperties::Clone(const n0::SceneNode& obj) const
{
	auto comp = std::make_unique<CompCustomProperties>();
	comp->m_props = m_props;
	for (auto& itr : comp->m_props)
	{
		switch (itr.type)
		{
		case PROP_STRING:
			{
				auto str = itr.var.m_val.pc;
				char* tmp = new char[strlen(str) + 1];
				strcpy(tmp, str);
				itr.var.m_val.pc = tmp;
			}
			break;
		case PROP_VEC2:
			{
				auto& vec2 = *static_cast<sm::vec2*>(itr.var.m_val.pv);
				auto tmp = new sm::vec2(vec2);
				itr.var.m_val.pv = tmp;
			}
			break;
		case PROP_COLOR:
			{
				auto& col = *static_cast<pt2::Color*>(itr.var.m_val.pv);
				auto tmp = new pt2::Color(col);
				itr.var.m_val.pv = tmp;
			}
			break;
		default:
			;
		}
	}
	return comp;
}

void CompCustomProperties::Add(const Property& prop)
{
	// already exist
	for (auto& pp : m_props) {
		if (pp.dis_name == prop.dis_name) {
			return;
		}
	}

	m_props.push_back(prop);
}

void CompCustomProperties::Remove(const std::string& name)
{
	for (auto itr = m_props.begin(); itr != m_props.end(); ++itr) {
		if (itr->dis_name == name) {
			m_props.erase(itr);
			break;
		}
	}
}

}