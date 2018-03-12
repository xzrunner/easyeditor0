#pragma once

#include <node0/typedef.h>

#include <vector>

namespace ee0
{

class SubjectMgr;

class MsgHelper
{
public:
	static bool InsertNode(SubjectMgr& sub_mgr, n0::SceneNodePtr& node, bool select_new = false);
	static bool DeleteNode(SubjectMgr& sub_mgr, const n0::SceneNodePtr& node);

	static void InsertNodeSelection(SubjectMgr& sub_mgr, const std::vector<n0::SceneNodePtr>& nodes);

	static bool SetEditorDirty(SubjectMgr& sub_mgr, bool dirty);

}; // MsgHelper

}