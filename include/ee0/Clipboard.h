#pragma once

#include <cu/cu_macro.h>
#include <node0/typedef.h>

#include <vector>

namespace ee0
{

class Clipboard
{
public:
    void Clear();

    void SetSceneNodes(const std::vector<n0::SceneNodePtr>& nodes) {
        m_nodes = nodes;
    }
    auto& GetSceneNodes() const { return m_nodes; }
    
private:
    std::vector<n0::SceneNodePtr> m_nodes;

    CU_SINGLETON_DECLARATION(Clipboard);

}; // Clipboard

}