#pragma once

#include <node0/NodeComponent.h>

namespace ee0
{

class CompNodeEditor : public n0::NodeComponent
{
public:
	CompNodeEditor();

	virtual const char* Type() const override { return TYPE_NAME; }
	virtual std::unique_ptr<n0::NodeComponent> Clone() const override;

	void SetFilepath(const std::string& filepath) { m_filepath = filepath; }
	const std::string& GetFilepath() const { return m_filepath; }

	const std::string& GetName() const;
	void SetName(const std::string& name);

	bool IsVisible() const { return m_visible; }
	bool IsEditable() const { return m_editable; }
	void SetVisible(bool visible) { m_visible = visible; }
	void SetEditable(bool editable) { m_editable = editable; }

	static const char* const TYPE_NAME;

private:
	std::string m_filepath;
	std::string m_name;

	bool m_visible, m_editable;

}; // CompNodeEditor

}