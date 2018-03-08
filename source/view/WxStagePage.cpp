#include "ee0/WxStagePage.h"

#include <node0/typedef.h>
#include <guard/check.h>

namespace ee0
{

WxStagePage::WxStagePage(wxWindow* parent)
	: WxEditPanel(parent, m_sub_mgr)
{
	m_sub_mgr.RegisterObserver(MSG_NODE_SELECTION_INSERT, this);
	m_sub_mgr.RegisterObserver(MSG_NODE_SELECTION_DELETE, this);
	m_sub_mgr.RegisterObserver(MSG_NODE_SELECTION_CLEAR, this);
}

void WxStagePage::OnNotify(MessageID msg, const VariantSet& variants)
{
	switch (msg)
	{
	case MSG_NODE_SELECTION_INSERT:
		NodeSelectionInsert(variants);
		break;
	case MSG_NODE_SELECTION_DELETE:
		NodeSelectionDelete(variants);
		break;
	case MSG_NODE_SELECTION_CLEAR:
		m_node_selection.Clear();
		m_sub_mgr.NotifyObservers(MSG_SET_CANVAS_DIRTY);
		break;
	}
}

void WxStagePage::NodeSelectionInsert(const VariantSet& variants)
{
	auto var = variants.GetVariant("node");
	GD_ASSERT(var.m_type == VT_PVOID, "no var in vars: node");
	n0::SceneNodePtr* node = static_cast<n0::SceneNodePtr*>(var.m_val.pv);
	GD_ASSERT(node, "err scene node");

	m_node_selection.Add(*node);

	m_sub_mgr.NotifyObservers(MSG_SET_CANVAS_DIRTY);
}

void WxStagePage::NodeSelectionDelete(const VariantSet& variants)
{
	auto var = variants.GetVariant("node");
	GD_ASSERT(var.m_type == VT_PVOID, "no var in vars: node");
	n0::SceneNodePtr* node = static_cast<n0::SceneNodePtr*>(var.m_val.pv);
	GD_ASSERT(node, "err scene node");

	m_node_selection.Remove(*node);

	m_sub_mgr.NotifyObservers(MSG_SET_CANVAS_DIRTY);
}

}