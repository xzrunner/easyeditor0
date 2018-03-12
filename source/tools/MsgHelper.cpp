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
	if (select_new) {
		bool select = sub_mgr.NotifyObservers(MSG_NODE_SELECTION_INSERT, vars);
		return insert && select;
	} else {
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

void MsgHelper::InsertNodeSelection(SubjectMgr& sub_mgr, const std::vector<n0::SceneNodePtr>& nodes)
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

		Variant var;
		var.m_type = VT_PVOID;
		var.m_val.pv = &std::const_pointer_cast<n0::SceneNode>(node);
		vars.SetVariant("node", var);

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