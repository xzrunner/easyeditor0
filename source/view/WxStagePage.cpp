#include "ee0/WxStagePage.h"
#include "ee0/SubjectMgr.h"

#include <guard/check.h>

namespace
{

const uint32_t MESSAGES[] =
{
	ee0::MSG_NODE_SELECTION_INSERT,
	ee0::MSG_NODE_SELECTION_DELETE,
	ee0::MSG_NODE_SELECTION_CLEAR,
	ee0::MSG_SET_EDITOR_DIRTY,
};

}

namespace ee0
{

WxStagePage::WxStagePage(wxWindow* parent)
	: WxEditPanel(parent, nullptr)
	, m_edit_dirty(false)
{
	for (auto& msg : MESSAGES) {
		m_sub_mgr->RegisterObserver(msg, this);
	}
}

// todo
//WxStagePage::~WxStagePage()
//{
//	for (auto& msg : MESSAGES) {
//		m_sub_mgr->UnregisterObserver(msg, this);
//	}
//}

void WxStagePage::OnNotify(uint32_t msg, const VariantSet& variants)
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
		m_sub_mgr->NotifyObservers(MSG_SET_CANVAS_DIRTY);
		break;
	case MSG_SET_EDITOR_DIRTY:
		SetEditorDirty(variants);
		break;
	}
}

void WxStagePage::NodeSelectionInsert(const VariantSet& variants)
{
	GameObj obj, root;
	size_t node_id = 0;

	auto var_obj = variants.GetVariant("obj");
	GD_ASSERT(var_obj.m_type == VT_PVOID, "no var in vars: obj");
	obj = *static_cast<GameObj*>(var_obj.m_val.pv);
	GD_ASSERT(obj, "err scene obj");

	auto var_root = variants.GetVariant("root");
	if (var_root.m_type != VT_EMPTY) {
		GD_ASSERT(var_root.m_type == VT_PVOID, "no var in vars: obj");
		root = *static_cast<GameObj*>(var_root.m_val.pv);
	}

	auto var_id = variants.GetVariant("id");
	if (var_id.m_type != VT_EMPTY) {
		GD_ASSERT(var_id.m_type == VT_ULONG, "no var in vars: obj");
		node_id = var_id.m_val.ul;
	}

	auto var_clear = variants.GetVariant("clear");
	if (var_clear.m_type != VT_EMPTY) {
		GD_ASSERT(var_clear.m_type == VT_BOOL, "no var in vars: obj");
		if (var_clear.m_val.bl) {
			m_node_selection.Clear();
		}
	}

	m_node_selection.Add(n0::NodeWithPos(obj, root, node_id));

	m_sub_mgr->NotifyObservers(MSG_SET_CANVAS_DIRTY);
}

void WxStagePage::NodeSelectionDelete(const VariantSet& variants)
{
	auto var = variants.GetVariant("obj");
	GD_ASSERT(var.m_type == VT_PVOID, "no var in vars: obj");
	GameObj* obj = static_cast<GameObj*>(var.m_val.pv);
	GD_ASSERT(obj, "err scene obj");

	n0::NodeWithPos node_pos(*obj, *obj, 0);
	m_node_selection.Remove(node_pos);

	m_sub_mgr->NotifyObservers(MSG_SET_CANVAS_DIRTY);
}

void WxStagePage::SetEditorDirty(const ee0::VariantSet& variants)
{
	auto var = variants.GetVariant("dirty");
	GD_ASSERT(var.m_type == ee0::VT_BOOL, "err val");
	m_edit_dirty = var.m_val.bl;
}

}