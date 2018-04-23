#pragma once

#include <string>

namespace ee0
{

class AtomicOP
{
public:
	virtual ~AtomicOP() {}

	virtual void Undo() = 0;
	virtual void Redo() = 0;

	virtual std::string ToString() const { return "todo"; }
	virtual std::string ToScript() const { return "todo"; }

}; // AtomicOP

}