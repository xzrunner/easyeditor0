#include "ee0/AssetsMap.h"

#include <yaml-cpp/yaml.h>

#include <wx/dir.h>

#include <boost/filesystem.hpp>

namespace ee0
{

CU_SINGLETON_DEFINITION(AssetsMap)

AssetsMap::AssetsMap()
{
}

void AssetsMap::LoadDirWithUnity(const std::string& dirpath)
{
	class DirTraverser : public wxDirTraverser
	{
	public:
		DirTraverser(AssetsMap& am)
			: m_am(am) {}

		virtual wxDirTraverseResult OnFile(const wxString& filename) override
		{
            m_am.LoadFileWithUnity(filename.ToStdString());
			return wxDIR_CONTINUE;
		}

		virtual wxDirTraverseResult OnDir(const wxString& dirname) override
		{
			return wxDIR_CONTINUE;
		}

	private:
        AssetsMap& m_am;

	}; // DirTraverser

	DirTraverser traverser(*this);

	wxDir dir(dirpath.c_str());
	dir.Traverse(traverser);
}

bool AssetsMap::LoadFileWithUnity(const std::string& filepath)
{
    if (!boost::filesystem::exists(filepath)) {
        return false;
    }
    auto meta_path = filepath + ".meta";
    if (!boost::filesystem::exists(meta_path)) {
        return false;
    }

    auto root = YAML::LoadFile(meta_path);
    auto guid = root["guid"].as<std::string>();
    m_guid2path.insert({ guid, filepath });

    return true;
}

std::string AssetsMap::QueryFilepath(const std::string& guid) const
{
    auto itr = m_guid2path.find(guid);
    return itr == m_guid2path.end() ? "" : itr->second;
}

}