#include "ee0/CompNodeEditor.h"

#include <string>

namespace ee0
{

const char* const CompNodeEditor::TYPE_NAME = "ee0_node_editor";

CompNodeEditor::CompNodeEditor()
	: m_visible(true)
	, m_editable(true)
	, m_id(0)
{
}

std::unique_ptr<n0::NodeUniqueComp> CompNodeEditor::Clone(const n0::SceneNode& obj) const
{
	auto comp = std::make_unique<CompNodeEditor>();
	comp->m_filepath = m_filepath;
	comp->m_name     = m_name;
	comp->m_visible  = m_visible;
	comp->m_editable = m_editable;
	return comp;
}

}