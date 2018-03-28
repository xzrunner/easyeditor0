#pragma once

#include "ee0/Observer.h"
#include "ee0/WxEditPanel.h"
#include "ee0/SelectionSet.h"
#include "ee0/NodeContainer.h"
#include "ee0/typedef.h"

#include <node0/NodeWithPos.h>

namespace ee0
{

class WxStagePage : public WxEditPanel, public Observer, public NodeContainer
{
public:
	enum TraverseType
	{
		TRAV_DRAW = 0,
		TRAV_QUERY,
	};

public:
	WxStagePage(wxWindow* parent);
	// todo
//	virtual ~WxStagePage();

	virtual void OnNotify(uint32_t msg, const VariantSet& variants) override;

	const SelectionSet<n0::NodeWithPos>& GetNodeSelection() const {
		return m_node_selection;
	}
	SelectionSet<n0::NodeWithPos>& GetNodeSelection() {
		return m_node_selection;
	}

	bool IsEditDirty() const { return m_edit_dirty; }

private:
	void NodeSelectionInsert(const VariantSet& variants);
	void NodeSelectionDelete(const VariantSet& variants);

	void SetEditorDirty(const ee0::VariantSet& variants);

protected:
	SelectionSet<n0::NodeWithPos> m_node_selection;

private:
	bool m_edit_dirty;

}; // WxStagePage

}