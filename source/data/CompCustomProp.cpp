#include "ee0/CompCustomProp.h"

#include <SM_Vector.h>
#include <painting2/Color.h>

namespace ee0
{

const char* const CompCustomProp::TYPE_NAME = "ee0_custom_properties";

CompCustomProp::CompCustomProp()
{
}

std::unique_ptr<n0::NodeComp> CompCustomProp::Clone(const n0::SceneNode& obj) const
{
	auto comp = std::make_unique<CompCustomProp>();
	comp->m_props = m_props;
	for (auto& itr : comp->m_props)
	{
		switch (itr.type)
		{
		case PROP_STRING:
			{
				auto str = itr.val.m_val.pc;
				char* tmp = new char[strlen(str) + 1];
				strcpy(tmp, str);
				itr.val.m_val.pc = tmp;
			}
			break;
		case PROP_VEC2:
			{
				auto& vec2 = *static_cast<sm::vec2*>(itr.val.m_val.pv);
				auto tmp = new sm::vec2(vec2);
				itr.val.m_val.pv = tmp;
			}
			break;
		case PROP_COLOR:
			{
				auto& col = *static_cast<pt2::Color*>(itr.val.m_val.pv);
				auto tmp = new pt2::Color(col);
				itr.val.m_val.pv = tmp;
			}
			break;
		default:
			;
		}
	}
	return comp;
}

void CompCustomProp::Add(const Property& prop)
{
	// already exist
	for (auto& pp : m_props) {
		if (pp.key == prop.key) {
			return;
		}
	}

	m_props.push_back(prop);
}

void CompCustomProp::Remove(const std::string& name)
{
	for (auto itr = m_props.begin(); itr != m_props.end(); ++itr) {
		if (itr->key == name) {
			m_props.erase(itr);
			break;
		}
	}
}

}