#pragma once

#include "ee0/Observer.h"
#include "ee0/SubjectMgr.h"
#include "ee0/WxEditPanel.h"
#include "ee0/SelectionSet.h"

#include <node0/typedef.h>

namespace ee0
{

class WxStagePage : public WxEditPanel, public Observer
{
public:
	WxStagePage(wxWindow* parent);

	virtual void OnNotify(MessageID msg, const VariantSet& variants) override;

	SubjectMgr& GetSubjectMgr() { return m_sub_mgr; }

	const SelectionSet<n0::SceneNode>& GetNodeSelection() const {
		return m_node_selection;
	}
	SelectionSet<n0::SceneNode>& GetNodeSelection() {
		return m_node_selection;
	}

private:
	void NodeSelectionInsert(const VariantSet& variants);
	void NodeSelectionDelete(const VariantSet& variants);

protected:
	SubjectMgr m_sub_mgr;

	SelectionSet<n0::SceneNode> m_node_selection;

}; // WxStagePage

}