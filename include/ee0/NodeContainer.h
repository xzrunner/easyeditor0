#pragma once

#include "ee0/VariantSet.h"

#include <node0/typedef.h>

#include <functional>

namespace ee0
{

class NodeContainer
{
public:
	virtual void Traverse(std::function<bool(const n0::SceneNodePtr&)> func,
		const VariantSet& variants = VariantSet(), bool inverse = false) const = 0;

}; // NodeContainer

}