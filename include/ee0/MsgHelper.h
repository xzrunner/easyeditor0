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

	static bool SendNodeMsg(SubjectMgr& sub_mgr, const GameObj& obj, MessageID msg);

	static void InsertSelection(SubjectMgr& sub_mgr, const std::vector<GameObjWithPos>& objs);

	static bool SetEditorDirty(SubjectMgr& sub_mgr, bool dirty);

	static void AddAtomicOP(SubjectMgr& sub_mgr, const std::shared_ptr<AtomicOP>& aop);

private:
	static void PrepareSelectionVars(VariantSet& vars, const GameObjWithPos& obj);

}; // MsgHelper

}