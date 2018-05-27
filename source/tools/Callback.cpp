#include "ee0/Callback.h"
#include "ee0/CompCustomProp.h"
#include "ee0/NSCompCustomProp.h"

#include <ns/RegistCallback.h>
#include <ns/CompIdxMgr.h>

namespace ee0
{

void Callback::RegisterCallback()
{
	ns::CompIdxMgr::Instance()->AddExtTypeToIdx(CompCustomProp::TYPE_NAME, ns::CompIdx::COMP_MAX);
	ns::RegistCallback::AddUniqueCB<CompCustomProp, NSCompCustomProp>();
}

}