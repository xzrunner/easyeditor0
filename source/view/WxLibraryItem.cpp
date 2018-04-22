#include "ee0/WxLibraryItem.h"
#include "ee0/WxBitmap.h"

#include <facade/ResPool.h>

#include <boost/filesystem.hpp>

namespace ee0
{

WxLibraryItem::WxLibraryItem(const std::string& filepath)
	: m_filepath(filepath)
	, m_ud(nullptr)
	, m_show(true)
{
	m_name = boost::filesystem::path(filepath).filename().string();

	m_bitmap = facade::ResPool::Instance().Fetch<WxBitmap>(filepath);
}

}