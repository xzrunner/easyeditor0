#include "ee0/MsgHelper.h"
#include "ee0/VariantSet.h"
#include "ee0/SubjectMgr.h"

namespace ee0
{

bool MsgHelper::InsertNode(SubjectMgr& sub_mgr, n0::SceneNodePtr& node, bool select_new)
{
	VariantSet vars;

	Variant var;
	var.m_type = VT_PVOID;
	var.m_val.pv = &node;
	vars.SetVariant("node", var);

	bool insert = sub_mgr.NotifyObservers(MSG_INSERT_SCENE_NODE, vars);

	if (select_new) 
	{
		Variant var_root;
		var_root.m_type = ee0::VT_PVOID;
		var_root.m_val.pv = &node;
		vars.SetVariant("root", var_root);

		Variant var_id;
		var_id.m_type = ee0::VT_ULONG;
		var_id.m_val.ul = 0;
		vars.SetVariant("id", var_id);

		bool select = sub_mgr.NotifyObservers(MSG_NODE_SELECTION_INSERT, vars);
		return insert && select;
	} 
	else 
	{
		return insert;
	}
}

bool MsgHelper::DeleteNode(SubjectMgr& sub_mgr, const n0::SceneNodePtr& node)
{
	ee0::VariantSet vars;
	ee0::Variant var;
	var.m_type = ee0::VT_PVOID;
	var.m_val.pv = &std::const_pointer_cast<n0::SceneNode>(node);
	vars.SetVariant("node", var);
	return sub_mgr.NotifyObservers(MSG_DELETE_SCENE_NODE, vars);
}

void MsgHelper::InsertNodeSelection(SubjectMgr& sub_mgr, const std::vector<n0::NodeWithPos>& nodes)
{
	for (auto& node : nodes)
	{
		VariantSet vars;

		if (nodes.size() > 1) {
			Variant var;
			var.m_type = VT_BOOL;
			var.m_val.bl = true;
			vars.SetVariant("multiple", var);
		}

		Variant var_node;
		var_node.m_type = VT_PVOID;
		var_node.m_val.pv = &std::const_pointer_cast<n0::SceneNode>(node.node);
		vars.SetVariant("node", var_node);

		Variant var_root;
		var_root.m_type = ee0::VT_PVOID;
		var_root.m_val.pv = &std::const_pointer_cast<n0::SceneNode>(node.root);
		vars.SetVariant("root", var_root);

		Variant var_id;
		var_id.m_type = ee0::VT_ULONG;
		var_id.m_val.ul = node.node_id;
		vars.SetVariant("id", var_id);

		sub_mgr.NotifyObservers(MSG_NODE_SELECTION_INSERT, vars);
	}
}

bool MsgHelper::SetEditorDirty(SubjectMgr& sub_mgr, bool dirty)
{
	ee0::VariantSet vars;
	ee0::Variant var;
	var.m_type = ee0::VT_BOOL;
	var.m_val.bl = dirty;
	vars.SetVariant("dirty", var);
	return sub_mgr.NotifyObservers(MSG_SET_EDITOR_DIRTY, vars);
}

}