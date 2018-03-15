#pragma once

#include <memory>

namespace ee0
{

class EditOpState;
using EditOpStatePtr = std::shared_ptr<EditOpState>;

class SubjectMgr;
using SubjectMgrPtr = std::shared_ptr<SubjectMgr>;

}