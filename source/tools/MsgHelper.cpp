#include "ee0/MsgHelper.h"
#include "ee0/VariantSet.h"
#include "ee0/SubjectMgr.h"
#include "ee0/MessageID.h"

namespace ee0
{

bool MsgHelper::InsertNode(SubjectMgr& sub_mgr, const GameObj& obj, bool select_new)
{
	VariantSet vars;

	Variant var;
	var.m_type = VT_PVOID;
	var.m_val.pv = &obj;
	vars.SetVariant("obj", var);

	bool insert = sub_mgr.NotifyObservers(MSG_SCENE_NODE_INSERT, vars);

	if (select_new)
	{
		Variant var_root;
		var_root.m_type = ee0::VT_PVOID;
		var_root.m_val.pv = &obj;
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

bool MsgHelper::DeleteNode(SubjectMgr& sub_mgr, const GameObj& obj)
{
	ee0::VariantSet vars;
	ee0::Variant var;
	var.m_type = ee0::VT_PVOID;
	var.m_val.pv = &obj;
	vars.SetVariant("obj", var);
	return sub_mgr.NotifyObservers(MSG_SCENE_NODE_DELETE, vars);
}

bool MsgHelper::SendObjMsg(SubjectMgr& sub_mgr, const GameObj& obj, uint32_t msg)
{
	ee0::VariantSet vars;
	ee0::Variant var;
	var.m_type = ee0::VT_PVOID;
	var.m_val.pv = &obj;
	vars.SetVariant("obj", var);
	return sub_mgr.NotifyObservers(msg, vars);
}

void MsgHelper::InsertSelection(SubjectMgr& sub_mgr, const std::vector<GameObjWithPos>& objs)
{
	for (auto& obj : objs)
	{
		VariantSet vars;

		if (objs.size() > 1) {
			Variant var;
			var.m_type = VT_BOOL;
			var.m_val.bl = true;
			vars.SetVariant("multiple", var);
		}

		PrepareSelectionVars(vars, obj);

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

void MsgHelper::AddAtomicOP(SubjectMgr& sub_mgr, const std::shared_ptr<AtomicOP>& aop)
{
	VariantSet vars;

	Variant var;
	var.m_type = VT_PVOID;
	var.m_val.pv = &aop;
	vars.SetVariant("aop", var);

	sub_mgr.NotifyObservers(MSG_ATOMIC_OP_ADD, vars);
}

void MsgHelper::CreateNewPage(SubjectMgr& sub_mgr, const char* type, const char* filepath,
                              const n0::SceneNodePtr& obj)
{
    VariantSet vars;

    Variant var_type;
    var_type.m_type = VT_PCHAR;
    var_type.m_val.pc = type;
    vars.SetVariant("type", var_type);

    Variant var_path;
    var_path.m_type = VT_PCHAR;
    var_path.m_val.pc = filepath;
    vars.SetVariant("filepath", var_path);

    Variant var_obj;
    var_obj.m_type = VT_PVOID;
    var_obj.m_val.pv = &obj;
    vars.SetVariant("obj", var_obj);

    sub_mgr.NotifyObservers(MSG_STAGE_PAGE_NEW, vars);
}

void MsgHelper::PrepareSelectionVars(VariantSet& vars, const GameObjWithPos& obj)
{
	Variant var_obj;
	var_obj.m_type = VT_PVOID;
#ifndef GAME_OBJ_ECS
	var_obj.m_val.pv = &obj.GetNode();
#else
	var_obj.m_val.pv = &obj;
#endif // GAME_OBJ_ECS
	vars.SetVariant("obj", var_obj);

#ifndef GAME_OBJ_ECS
	Variant var_root;
	var_root.m_type = ee0::VT_PVOID;
	var_root.m_val.pv = &obj.GetRoot();
	vars.SetVariant("root", var_root);

	Variant var_id;
	var_id.m_type = ee0::VT_ULONG;
	var_id.m_val.ul = obj.GetNodeID();
	vars.SetVariant("id", var_id);
#endif // GAME_OBJ_ECS
}

}