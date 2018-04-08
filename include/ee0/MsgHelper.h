#pragma once

#include "ee0/GameObj.h"

#include <node0/NodeWithPos.h>

#include <vector>

namespace ee0
{

class SubjectMgr;

class MsgHelper
{
public:
	static bool InsertNode(SubjectMgr& sub_mgr, GameObj& obj, bool select_new = false);
	static bool DeleteNode(SubjectMgr& sub_mgr, const GameObj& obj);

	static void InsertNodeSelection(SubjectMgr& sub_mgr, const std::vector<n0::NodeWithPos>& nodes);

	static bool SetEditorDirty(SubjectMgr& sub_mgr, bool dirty);

}; // MsgHelper

}