#include "ee0/Callback.h"
#include "ee0/CompCustomProp.h"
#include "ee0/NSCompCustomProp.h"

#include <ns/RegistCallback.h>

namespace ee0
{

void Callback::RegisterCallback()
{
	ns::CompSerializer::Instance()->AddExtTypeToIndex(CompCustomProp::TYPE_NAME, ns::CompSerializer::COMP_MAX);
	ns::RegistCallback::AddUniqueCB<CompCustomProp, NSCompCustomProp>();
}

}