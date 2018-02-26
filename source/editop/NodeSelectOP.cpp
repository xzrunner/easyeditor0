#include "ee0/NodeSelectOP.h"
#include "ee0/WxStagePage.h"
#include "ee0/MessageID.h"

#include <guard/check.h>

namespace ee0
{

NodeSelectOP::NodeSelectOP(WxStagePage& stage)
	: m_stage(stage)
{
	m_last_pos.MakeInvalid();
}

bool NodeSelectOP::OnKeyDown(int keyCode)
{
	if (EditOP::OnKeyDown(keyCode)) {
		return true;
	}

	if (keyCode == WXK_DELETE)
	{
		m_stage.GetNodeSelection().Traverse([&](const n0::SceneNodePtr& node)->bool
		{
			ee0::VariantSet vars;
			ee0::Variant var;
			var.m_type = ee0::VT_PVOID;
			var.m_val.pv = &std::const_pointer_cast<n0::SceneNode>(node);
			vars.SetVariant("node", var);
			bool succ = m_stage.GetSubjectMgr().NotifyObservers(MSG_DELETE_SCENE_NODE, vars);
			GD_ASSERT(succ, "fail to MSG_DELETE_SCENE_NODE");

			return true;
		});

		m_stage.GetSubjectMgr().NotifyObservers(MSG_NODE_SELECTION_CLEAR);
	}

	return false;
}

bool NodeSelectOP::OnMouseLeftDown(int x, int y)
{
	if (EditOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	auto& sub_mgr = m_stage.GetSubjectMgr();

	auto& selection = m_stage.GetNodeSelection();
	auto selected = QueryByPos(x, y);
	if (selected)
	{
		VariantSet vars;
		Variant var;
		var.m_type = VT_PVOID;
		var.m_val.pv = &selected;
		vars.SetVariant("node", var);

		if (m_stage.GetKeyState(WXK_CONTROL)) 
		{
			if (selection.IsExist(selected)) {
				sub_mgr.NotifyObservers(MSG_NODE_SELECTION_DELETE, vars);
			} else {
				sub_mgr.NotifyObservers(MSG_NODE_SELECTION_INSERT, vars);
			}
		}
		else
		{
			if (!selection.IsExist(selected))
			{
				sub_mgr.NotifyObservers(MSG_NODE_SELECTION_CLEAR);
				sub_mgr.NotifyObservers(MSG_NODE_SELECTION_INSERT, vars);
			}
		}
	}
	else
	{
		sub_mgr.NotifyObservers(MSG_NODE_SELECTION_CLEAR);
	}

	sub_mgr.NotifyObservers(MSG_SET_CANVAS_DIRTY);

	m_last_pos.Set(x, y);

	return false;
}

bool NodeSelectOP::OnMouseLeftUp(int x, int y)
{
	if (EditOP::OnMouseLeftUp(x, y)) {
		return true;
	}

	if (!m_last_pos.IsValid() || m_last_pos == sm::ivec2(x, y)) {
		return false;
	}

	auto& sub_mgr = m_stage.GetSubjectMgr();
	auto& selection = m_stage.GetNodeSelection();

	std::vector<n0::SceneNodePtr> nodes;
	QueryByRect(m_last_pos, sm::ivec2(x, y), m_last_pos.x < x, nodes);
	if (m_stage.GetKeyState(WXK_CONTROL))
	{
		for (auto& node : nodes)
		{
			VariantSet vars;
			Variant var;
			var.m_type = VT_PVOID;
			var.m_val.pv = &node;
			vars.SetVariant("node", var);

			if (selection.IsExist(node)) {
				sub_mgr.NotifyObservers(MSG_NODE_SELECTION_DELETE, vars);
			} else {
				sub_mgr.NotifyObservers(MSG_NODE_SELECTION_INSERT, vars);
			}
		}
	}
	else
	{
		sub_mgr.NotifyObservers(MSG_NODE_SELECTION_CLEAR);

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
			var.m_val.pv = &node;
			vars.SetVariant("node", var);

			sub_mgr.NotifyObservers(MSG_NODE_SELECTION_INSERT, vars);
		}
	}

	sub_mgr.NotifyObservers(MSG_SET_CANVAS_DIRTY);

	m_last_pos.MakeInvalid();
	
	return false;
}

}