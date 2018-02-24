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

}