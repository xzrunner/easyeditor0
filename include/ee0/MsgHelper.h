#pragma once

#include "ee0/GameObj.h"
#include "ee0/MessageID.h"

#include <vector>

namespace ee0
{

class SubjectMgr;
class AtomicOP;
class VariantSet;

class MsgHelper
{
public:
	static bool InsertNode(SubjectMgr& sub_mgr, const GameObj& obj, bool select_new = false);
	static bool DeleteNode(SubjectMgr& sub_mgr, const GameObj& obj);

	static bool SendObjMsg(SubjectMgr& sub_mgr, const GameObj& obj, uint32_t msg);

	static void InsertSelection(SubjectMgr& sub_mgr, const std::vector<GameObjWithPos>& objs);

	static bool SetEditorDirty(SubjectMgr& sub_mgr, bool dirty);

	static void AddAtomicOP(SubjectMgr& sub_mgr, const std::shared_ptr<AtomicOP>& aop);

    static void CreateNewPage(SubjectMgr& sub_mgr, const char* type, const char* filepath,
        const n0::SceneNodePtr& obj);

private:
	static void PrepareSelectionVars(VariantSet& vars, const GameObjWithPos& obj);

}; // MsgHelper

}