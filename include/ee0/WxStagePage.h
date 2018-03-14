#pragma once

#include "ee0/Observer.h"
#include "ee0/SubjectMgr.h"
#include "ee0/WxEditPanel.h"
#include "ee0/SelectionSet.h"
#include "ee0/NodeContainer.h"

#include <node0/NodeWithPos.h>

namespace ee0
{

class WxStagePage : public WxEditPanel, public Observer, public NodeContainer
{
public:
	WxStagePage(wxWindow* parent);

	virtual void OnNotify(MessageID msg, const VariantSet& variants) override;

	SubjectMgr& GetSubjectMgr() { return m_sub_mgr; }

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
	SubjectMgr m_sub_mgr;

	SelectionSet<n0::NodeWithPos> m_node_selection;

private:
	bool m_edit_dirty;

}; // WxStagePage

}