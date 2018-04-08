#include "ee0/CompNodeEditor.h"

#include <string>

namespace ee0
{

const char* const CompNodeEditor::TYPE_NAME = "ee0_node_editor";

CompNodeEditor::CompNodeEditor()
	: m_visible(true)
	, m_editable(true)
{
	static size_t COUNT = 0;
	m_name = "obj" + std::to_string(COUNT++);
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

const std::string& CompNodeEditor::GetName() const
{
	return m_name;
}

void CompNodeEditor::SetName(const std::string& name)
{
	m_name = name;
}

}