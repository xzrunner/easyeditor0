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
	n0::NodeWithPos nwp;

	auto var_node = variants.GetVariant("node");
	GD_ASSERT(var_node.m_type == VT_PVOID, "no var in vars: node");
	nwp.node = *static_cast<n0::SceneNodePtr*>(var_node.m_val.pv);
	GD_ASSERT(nwp.node, "err scene node");

	auto var_root = variants.GetVariant("root");
	if (var_root.m_type != VT_EMPTY) {
		GD_ASSERT(var_root.m_type == VT_PVOID, "no var in vars: node");
		nwp.root = *static_cast<n0::SceneNodePtr*>(var_root.m_val.pv);
	}

	auto var_id = variants.GetVariant("id");
	if (var_id.m_type != VT_EMPTY) {
		GD_ASSERT(var_id.m_type == VT_ULONG, "no var in vars: node");
		nwp.node_id = var_id.m_val.ul;
	}

	auto var_clear = variants.GetVariant("clear");
	if (var_clear.m_type != VT_EMPTY) {
		GD_ASSERT(var_clear.m_type == VT_BOOL, "no var in vars: node");
		if (var_clear.m_val.bl) {
			m_node_selection.Clear();
		}
	}

	m_node_selection.Add(nwp);

	m_sub_mgr.NotifyObservers(MSG_SET_CANVAS_DIRTY);
}

void WxStagePage::NodeSelectionDelete(const VariantSet& variants)
{
	auto var = variants.GetVariant("node");
	GD_ASSERT(var.m_type == VT_PVOID, "no var in vars: node");
	n0::SceneNodePtr* node = static_cast<n0::SceneNodePtr*>(var.m_val.pv);
	GD_ASSERT(node, "err scene node");

	n0::NodeWithPos node_pos(*node, *node, 0);
	m_node_selection.Remove(node_pos);

	m_sub_mgr.NotifyObservers(MSG_SET_CANVAS_DIRTY);
}

}