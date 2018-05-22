#pragma once

#include <node0/NodeUniqueComp.h>

namespace ee0
{

class CompNodeEditor : public n0::NodeUniqueComp
{
public:
	CompNodeEditor();

	virtual const char* Type() const override { return TYPE_NAME; }
	virtual n0::UniqueCompID TypeID() const override {
		return n0::GetUniqueCompTypeID<CompNodeEditor>();
	}
	virtual std::unique_ptr<n0::NodeUniqueComp> Clone(const n0::SceneNode& obj) const override;

	bool IsVisible() const { return m_visible; }
	bool IsEditable() const { return m_editable; }
	void SetVisible(bool visible) { m_visible = visible; }
	void SetEditable(bool editable) { m_editable = editable; }

	static const char* const TYPE_NAME;

private:
	bool m_visible, m_editable;

}; // CompNodeEditor

}