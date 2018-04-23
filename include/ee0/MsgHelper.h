#pragma once

#include "ee0/GameObj.h"

#include <vector>

namespace ee0
{

class SubjectMgr;
class AtomicOP;

class MsgHelper
{
public:
	static bool InsertNode(SubjectMgr& sub_mgr, const GameObj& obj, bool select_new = false);
	static bool DeleteNode(SubjectMgr& sub_mgr, const GameObj& obj);

	static void InsertSelection(SubjectMgr& sub_mgr, const std::vector<GameObjWithPos>& objs);

	static bool SetEditorDirty(SubjectMgr& sub_mgr, bool dirty);

	static void AddAtomicOP(SubjectMgr& sub_mgr, const std::shared_ptr<AtomicOP>& aop);
	
}; // MsgHelper

}