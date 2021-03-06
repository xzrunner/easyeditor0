#include "ee0/WxStagePage.h"
#include "ee0/SubjectMgr.h"
#include "ee0/MsgHelper.h"

#include <guard/check.h>
#include <moon/Blackboard.h>
#include <moon/Context.h>
#include <moon/ModuleMgr.h>
#include <moon/SceneGraph.h>

namespace
{

const uint32_t MESSAGES[] =
{
	ee0::MSG_NODE_SELECTION_INSERT,
	ee0::MSG_NODE_SELECTION_DELETE,
	ee0::MSG_NODE_SELECTION_CLEAR,

	ee0::MSG_SET_EDITOR_DIRTY,

	ee0::MSG_ATOMIC_OP_ADD,
	ee0::MSG_EDIT_OP_UNDO,
	ee0::MSG_EDIT_OP_REDO,
};

}

namespace ee0
{

WxStagePage::WxStagePage(wxWindow* parent, const SubjectMgrPtr& sub_mgr)
	: WxEditPanel(parent, sub_mgr)
	, m_edit_dirty(false)
	, m_next_obj_id(0)
{
	for (auto& msg : MESSAGES) {
		m_sub_mgr->RegisterObserver(msg, this);
	}

	RegisterMoonCallback();
}

WxStagePage::~WxStagePage()
{
	for (auto& msg : MESSAGES) {
		m_sub_mgr->UnregisterObserver(msg, this);
	}
}

void WxStagePage::OnNotify(uint32_t msg, const VariantSet& variants)
{
	switch (msg)
	{
	case MSG_NODE_SELECTION_INSERT:
		SelectionInsert(variants);
		break;
	case MSG_NODE_SELECTION_DELETE:
		SelectionDelete(variants);
		break;
	case MSG_NODE_SELECTION_CLEAR:
		SelectionClear();
		break;

	case MSG_SET_EDITOR_DIRTY:
		SetEditorDirty(variants);
		break;

	case MSG_ATOMIC_OP_ADD:
		AddAtomicOp(variants);
		break;
	case MSG_EDIT_OP_UNDO:
		OnEditOpUndo();
		break;
	case MSG_EDIT_OP_REDO:
		OnEditOpRedo();
		break;
	}
}

void WxStagePage::RegisterMoonCallback()
{
	auto scene = moon::Blackboard::Instance()->GetContext()->GetModuleMgr().GetModule<moon::SceneGraph>();

	scene->SetTraverseAllNodesFunc([&](std::function<bool(const n0::SceneNodePtr&)> func) {
		Traverse([&](const ee0::GameObj& obj)->bool {
			return func(obj);
		});
	});

	scene->SetTraverseSelectionFunc([&](std::function<bool(const n0::SceneNodePtr&)> func) {
		m_selection.Traverse([&](const ee0::GameObjWithPos& opw)->bool {
			return func(opw.GetNode());
		});
	});

	scene->SetSetSelectionFunc([&](const std::vector<n0::SceneNodePtr>& nodes)
	{
		m_sub_mgr->NotifyObservers(ee0::MSG_NODE_SELECTION_CLEAR);

		std::vector<ee0::GameObjWithPos> objs;
		objs.reserve(nodes.size());
		for (auto& node : nodes) {
			objs.push_back(n0::NodeWithPos(node, node, 0));
		}
		ee0::MsgHelper::InsertSelection(*m_sub_mgr, objs);
	});
}

void WxStagePage::SelectionInsert(const VariantSet& variants)
{
#ifndef GAME_OBJ_ECS
	n0::SceneNodePtr obj, root;
	size_t node_id = 0;

	auto var_obj = variants.GetVariant("obj");
	GD_ASSERT(var_obj.m_type == VT_PVOID, "no var in vars: obj");
	obj = *static_cast<const GameObj*>(var_obj.m_val.pv);
	GD_ASSERT(obj, "err scene obj");

	auto var_root = variants.GetVariant("root");
	if (var_root.m_type != VT_EMPTY) {
		GD_ASSERT(var_root.m_type == VT_PVOID, "no var in vars: obj");
		root = *static_cast<const GameObj*>(var_root.m_val.pv);
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
			m_selection.Clear();
		}
	}

	m_selection.Add(n0::NodeWithPos(obj, root, node_id));
#else
	auto var_obj = variants.GetVariant("obj");
	GD_ASSERT(var_obj.m_type == VT_PVOID, "no var in vars: obj");
	auto obj = *static_cast<const e0::Entity*>(var_obj.m_val.pv);
	m_selection.Add(obj);
#endif // GAME_OBJ_ECS

	m_sub_mgr->NotifyObservers(MSG_SET_CANVAS_DIRTY);
}

void WxStagePage::SelectionDelete(const VariantSet& variants)
{
	auto var = variants.GetVariant("obj");
	GD_ASSERT(var.m_type == VT_PVOID, "no var in vars: obj");
	const GameObj* obj = static_cast<const GameObj*>(var.m_val.pv);
	GD_ASSERT(obj, "err scene obj");

#ifndef GAME_OBJ_ECS
	m_selection.Remove(n0::NodeWithPos(*obj, *obj, 0));
#else
	m_selection.Remove(*obj);
#endif // GAME_OBJ_ECS

	m_sub_mgr->NotifyObservers(MSG_SET_CANVAS_DIRTY);
}

void WxStagePage::SelectionClear()
{
	m_selection.Clear();

	m_sub_mgr->NotifyObservers(MSG_SET_CANVAS_DIRTY);
}

void WxStagePage::SetEditorDirty(const ee0::VariantSet& variants)
{
	auto var = variants.GetVariant("dirty");
	GD_ASSERT(var.m_type == ee0::VT_BOOL, "err val");
	m_edit_dirty = var.m_val.bl;
}

void WxStagePage::AddAtomicOp(const ee0::VariantSet& variants)
{
	auto var_obj = variants.GetVariant("aop");
	GD_ASSERT(var_obj.m_type == VT_PVOID, "no var in vars: aop");
	auto& aop = *static_cast<const std::shared_ptr<AtomicOP>*>(var_obj.m_val.pv);
	m_edit_record.Add(aop);
}

void WxStagePage::OnEditOpUndo()
{
	bool dirty = m_edit_record.Undo();
	if (dirty) {
		MsgHelper::SetEditorDirty(*m_sub_mgr, true);
	}
}

void WxStagePage::OnEditOpRedo()
{
	bool dirty = m_edit_record.Redo();
	if (dirty) {
		MsgHelper::SetEditorDirty(*m_sub_mgr, true);
	}
}

}