#pragma once

#include "ee0/GameObj.h"
#include "ee0/VariantSet.h"

#include <functional>

namespace ee0
{

class SceneNodeContainer
{
public:
	virtual void Traverse(std::function<bool(const GameObj&)> func,
		const VariantSet& variants = VariantSet(), bool inverse = false) const = 0;

}; // SceneNodeContainer

}