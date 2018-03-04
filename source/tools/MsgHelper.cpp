#include "ee0/MsgHelper.h"
#include "ee0/VariantSet.h"
#include "ee0/SubjectMgr.h"

namespace ee0
{

bool MsgHelper::InsertNode(SubjectMgr& mgr, n0::SceneNodePtr& node)
{
	VariantSet vars;
	Variant var;
	var.m_type = VT_PVOID;
	var.m_val.pv = &node;
	vars.SetVariant("node", var);
	return mgr.NotifyObservers(MSG_INSERT_SCENE_NODE, vars);
}

bool MsgHelper::DeleteNode(SubjectMgr& mgr, n0::SceneNodePtr& node)
{
	ee0::VariantSet vars;
	ee0::Variant var;
	var.m_type = ee0::VT_PVOID;
	var.m_val.pv = &std::const_pointer_cast<n0::SceneNode>(node);
	vars.SetVariant("node", var);
	return mgr.NotifyObservers(MSG_DELETE_SCENE_NODE, vars);
}

}