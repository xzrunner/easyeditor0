#pragma once

#include <cu/cu_macro.h>
#include <node0/typedef.h>

#include <vector>

namespace ur { class Device; }

namespace ee0
{

class Clipboard
{
public:
//    void Clear();

    void StoreNodesToCB(const std::vector<n0::SceneNodePtr>& nodes);
    void LoadNodesFromCB(const ur::Device& dev, std::vector<n0::SceneNodePtr>& nodes);

    //void SetValue(const std::string& key, const std::string& val);
    //std::string GetValue(const std::string& key) const;

    void FromString(const std::string& all);
    std::string ToString() const;

    CU_SINGLETON_DECLARATION(Clipboard);

}; // Clipboard

}