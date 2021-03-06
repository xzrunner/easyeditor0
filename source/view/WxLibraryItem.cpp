#include "ee0/WxLibraryItem.h"
#include "ee0/WxBitmap.h"

#include <facade/ResPool.h>

#include <boost/filesystem.hpp>

namespace ee0
{

WxLibraryItem::WxLibraryItem(const std::string& filepath,
	                         const std::string& name)
	: m_filepath(filepath)
	, m_name(name)
	, m_ud(nullptr)
	, m_show(true)
{
	if (m_name.empty()) {
		m_name = boost::filesystem::path(m_filepath).filename().string();
	}
	m_bitmap = facade::ResPool::Instance().Fetch<WxBitmap>(filepath);
}

}