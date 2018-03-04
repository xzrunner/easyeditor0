#pragma once

#include <node0/typedef.h>

namespace ee0
{

class SubjectMgr;

class MsgHelper
{
public:
	static bool InsertNode(SubjectMgr& mgr, n0::SceneNodePtr& node);
	static bool DeleteNode(SubjectMgr& mgr, n0::SceneNodePtr& node);

}; // MsgHelper

}