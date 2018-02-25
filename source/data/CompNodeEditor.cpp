#include "ee0/CompNodeEditor.h"

#include <string>

namespace ee0
{

const char* const CompNodeEditor::TYPE_NAME = "node_editor";

CompNodeEditor::CompNodeEditor()
	: m_visible(true)
	, m_editable(true)
{
	static size_t COUNT = 0;
	m_name = "node" + std::to_string(COUNT++);
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