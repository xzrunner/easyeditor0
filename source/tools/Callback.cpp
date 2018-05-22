#include "ee0/Callback.h"
#include "ee0/CompCustomProp.h"
#include "ee0/NSCompCustomProp.h"

#include <ns/RegistCallback.h>

namespace ee0
{

void Callback::RegisterCallback()
{
	ns::RegistCallback::AddUniqueCB<CompCustomProp, NSCompCustomProp>();
}

}