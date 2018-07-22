#pragma once

#include <memory>

namespace ee0
{

class EditOP;
using EditOPPtr = std::shared_ptr<EditOP>;

class EditOpState;
using EditOpStatePtr = std::shared_ptr<EditOpState>;

class SubjectMgr;
using SubjectMgrPtr = std::shared_ptr<SubjectMgr>;

}