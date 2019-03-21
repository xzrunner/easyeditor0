#include "ee0/Clipboard.h"

namespace ee0
{

CU_SINGLETON_DEFINITION(Clipboard)

Clipboard::Clipboard()
{
}

void Clipboard::Clear()
{
    m_nodes.clear();
}

}