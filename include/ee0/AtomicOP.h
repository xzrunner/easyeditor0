#pragma once

namespace ee0
{

class AtomicOP
{
public:
	virtual ~AtomicOP() {}

	virtual void Undo() = 0;
	virtual void Redo() = 0;

}; // AtomicOP

}