#pragma once

#include "ee0/AtomicOP.h"

#include <vector>

namespace ee0
{

class CombineAOP : public AtomicOP
{
public:
    void Insert(const std::shared_ptr<AtomicOP>& atomic);

    virtual void Undo() override;
    virtual void Redo() override;

private:
    std::vector<std::shared_ptr<AtomicOP>> m_atomics;

}; // CombineAOP

}