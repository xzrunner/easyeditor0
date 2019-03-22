#pragma once

#include <cu/cu_macro.h>

#include <unordered_map>

namespace ee0
{

class AssetsMap
{
public:
    void LoadDirWithUnity(const std::string& dir);
    bool LoadFileWithUnity(const std::string& filepath);

    std::string QueryFilepath(const std::string& guid) const;

private:
    std::unordered_map<std::string, std::string> m_guid2path;

    CU_SINGLETON_DECLARATION(AssetsMap);

}; // AssetsMap

}
