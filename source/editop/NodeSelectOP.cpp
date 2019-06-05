#include "ee0/NodeSelectOP.h"
#include "ee0/WxStagePage.h"
#include "ee0/MessageID.h"
#include "ee0/MsgHelper.h"
#include "ee0/SubjectMgr.h"
#include "ee0/Clipboard.h"

#include <guard/check.h>
#include <node0/SceneNode.h>

namespace ee0
{

NodeSelectOP::NodeSelectOP(const std::shared_ptr<pt0::Camera>& cam,
	                       WxStagePage& stage)
	: EditOP(cam)
	, m_stage(stage)
{
	m_last_pos.MakeInvalid();
}

bool NodeSelectOP::OnKeyDown(int key_code)
{
	if (EditOP::OnKeyDown(key_code)) {
		return true;
	}

    switch (key_code)
    {
    case WXK_DELETE:
        DeleteSelection();
        break;
    case 'C':
        if (wxGetKeyState(WXK_CONTROL)) {
            CopySelectionToClipboard();
        }
        break;
    case 'V':
        if (wxGetKeyState(WXK_CONTROL)) {
            PasteSelectionFromClipboard();
        }
        break;
    case 'X':
        if (wxGetKeyState(WXK_CONTROL)) {
            CopySelectionToClipboard();
            DeleteSelection();
        }
        break;
    }

	return false;
}

bool NodeSelectOP::OnMouseLeftDown(int x, int y)
{
	if (EditOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	auto& sub_mgr = m_stage.GetSubjectMgr();

	auto& selection = m_stage.GetSelection();
	auto selected = QueryByPos(x, y);
	if (GAME_OBJ_VALID(selected))
	{
		VariantSet vars;

		Variant var_obj;
		var_obj.m_type = VT_PVOID;
		var_obj.m_val.pv = &selected;
		vars.SetVariant("obj", var_obj);

#ifndef GAME_OBJ_ECS
		Variant var_root;
		var_root.m_type = ee0::VT_PVOID;
		var_root.m_val.pv = &selected;
		vars.SetVariant("root", var_root);

		Variant var_id;
		var_id.m_type = ee0::VT_ULONG;
		var_id.m_val.ul = 0;
		vars.SetVariant("id", var_id);
#endif // GAME_OBJ_ECS

		if (wxGetKeyState(WXK_CONTROL))
		{
#ifndef GAME_OBJ_ECS
			if (selection.IsExist(n0::NodeWithPos(selected, selected, 0))) {
#else
			if (selection.IsExist(selected)) {
#endif // GAME_OBJ_ECS
                DeleteSelected(sub_mgr, selected, vars);
			} else {
                InsertSelected(sub_mgr, selected, vars);
			}
		}
		else
		{
#ifndef GAME_OBJ_ECS
			if (!selection.IsExist(n0::NodeWithPos(selected, selected, 0)))
#else
			if (!selection.IsExist(selected))
#endif // GAME_OBJ_ECS
			{
                ClearSelection(sub_mgr);
                InsertSelected(sub_mgr, selected, vars);
			}
		}
	}
	else
	{
        ClearSelection(sub_mgr);
	}

	sub_mgr->NotifyObservers(MSG_SET_CANVAS_DIRTY);

	m_last_pos.Set(x, y);

	return false;
}

bool NodeSelectOP::OnMouseLeftUp(int x, int y)
{
	if (EditOP::OnMouseLeftUp(x, y)) {
		return true;
	}

	auto& selection = m_stage.GetSelection();
	if (!m_last_pos.IsValid() || m_last_pos == sm::ivec2(x, y) || !selection.IsEmpty()) {
		m_last_pos.MakeInvalid();
		return false;
	}

	auto& sub_mgr = m_stage.GetSubjectMgr();

	std::vector<GameObj> objs;
	QueryByRect(m_last_pos, sm::ivec2(x, y), m_last_pos.x < x, objs);
	if (wxGetKeyState(WXK_CONTROL))
	{
		for (auto& obj : objs)
		{
			VariantSet vars;

			Variant var_obj;
			var_obj.m_type = VT_PVOID;
			var_obj.m_val.pv = &obj;
			vars.SetVariant("obj", var_obj);

#ifndef GAME_OBJ_ECS
			Variant var_root;
			var_root.m_type = ee0::VT_PVOID;
			var_root.m_val.pv = &obj;
			vars.SetVariant("root", var_root);

			Variant var_id;
			var_id.m_type = ee0::VT_ULONG;
			var_id.m_val.ul = 0;
			vars.SetVariant("id", var_id);
#endif // GAME_OBJ_ECS

#ifndef GAME_OBJ_ECS
			if (selection.IsExist(n0::NodeWithPos(obj, obj, 0))) {
#else
			if (selection.IsExist(obj)) {
#endif // GAME_OBJ_ECS
                DeleteSelected(sub_mgr, obj, vars);
			} else {
                InsertSelected(sub_mgr, obj, vars);
			}
		}
	}
	else
	{
        ClearSelection(sub_mgr);

#ifndef GAME_OBJ_ECS
		std::vector<n0::NodeWithPos> nwps;
		nwps.reserve(objs.size());
		for (auto& obj : objs) {
			nwps.push_back(n0::NodeWithPos(obj, obj, 0));
		}
		MsgHelper::InsertSelection(*sub_mgr, nwps);
#else
		MsgHelper::InsertSelection(*sub_mgr, objs);
#endif // GAME_OBJ_ECS
	}

	sub_mgr->NotifyObservers(MSG_SET_CANVAS_DIRTY);

	m_last_pos.MakeInvalid();

	return false;
}

bool NodeSelectOP::OnMouseMove(int x, int y)
{
    if (EditOP::OnMouseMove(x, y)) {
        return true;
    }

    if (m_last_pos.IsValid()) {
        OnMouseLeftUp(x, y);
    }

    return false;
}

void NodeSelectOP::DeleteSelection()
{
	m_stage.GetSelection().Traverse([&](const GameObjWithPos& owp)->bool
	{
#ifndef GAME_OBJ_ECS
		bool succ = MsgHelper::DeleteNode(*m_stage.GetSubjectMgr(),
			std::const_pointer_cast<n0::SceneNode>(owp.GetNode()));
#else
		bool succ = MsgHelper::DeleteNode(*m_stage.GetSubjectMgr(), owp);
#endif // GAME_OBJ_ECS
		GD_ASSERT(succ, "fail to MSG_SCENE_NODE_DELETE");
		return true;
	});

    ClearSelection(m_stage.GetSubjectMgr());
}

void NodeSelectOP::CopySelectionToClipboard()
{
    auto clipboard = Clipboard::Instance();
//    clipboard->Clear();

    std::vector<n0::SceneNodePtr> nodes;
    nodes.reserve(m_stage.GetSelection().Size());
    m_stage.GetSelection().Traverse([&](const GameObjWithPos& owp)->bool
    {
        nodes.push_back(owp.GetNode());
        return true;
    });

//    clipboard->SetSceneNodes(nodes);
    clipboard->StoreNodesToCB(nodes);
}

void NodeSelectOP::PasteSelectionFromClipboard()
{
    ClearSelection(m_stage.GetSubjectMgr());

    //auto& sub_mgr = m_stage.GetSubjectMgr();
    //for (auto& n : Clipboard::Instance()->GetSceneNodes()) {
    //    auto copy = n->Clone();
    //    MsgHelper::InsertNode(*sub_mgr, copy, true);
    //}

    auto clipboard = Clipboard::Instance();
    std::vector<n0::SceneNodePtr> nodes;
    clipboard->LoadNodesFromCB(nodes);
    auto& sub_mgr = m_stage.GetSubjectMgr();
    for (auto& n : nodes) {
//        auto copy = n->Clone();
        MsgHelper::InsertNode(*sub_mgr, n, true);
    }
}

void NodeSelectOP::InsertSelected(const SubjectMgrPtr& sub_mgr,
                                  const n0::SceneNodePtr& node,
                                  const VariantSet& vars) const
{
    sub_mgr->NotifyObservers(MSG_NODE_SELECTION_INSERT, vars);
    AfterInsertSelected(node);
}

void NodeSelectOP::DeleteSelected(const SubjectMgrPtr& sub_mgr,
                                  const n0::SceneNodePtr& node,
                                  const VariantSet& vars) const
{
    sub_mgr->NotifyObservers(MSG_NODE_SELECTION_DELETE, vars);
    AfterDeleteSelected(node);
}

void NodeSelectOP::ClearSelection(const SubjectMgrPtr& sub_mgr) const
{
    sub_mgr->NotifyObservers(MSG_NODE_SELECTION_CLEAR);
    AfterClearSelection();
}

}