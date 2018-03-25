#pragma once

#include "ee0/MessageID.h"

#include <stdint.h>

namespace ee0
{

class VariantSet;

class Observer
{
public:
	virtual ~Observer() {}

	virtual void OnNotify(uint32_t msg, const VariantSet& variants) = 0;

}; // Observer

}