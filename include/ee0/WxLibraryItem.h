#pragma once

#include <string>
#include <memory>

namespace ee0
{

class WxBitmap;

class WxLibraryItem
{
public:
	WxLibraryItem(const std::string& filepath,
		const std::string& name = "");

	auto& GetName() const { return m_name; }
	auto& GetFilepath() const { return m_filepath; }

	const std::shared_ptr<WxBitmap>& GetBitmap() const {
		return m_bitmap;
	}

	bool IsHide() const { return !m_show; }
	void SetHide(bool hide) { m_show = !hide; }

public:
	class UserData
	{
	public:
		virtual ~UserData() {}
	}; // UserData

	void SetUD(std::unique_ptr<UserData>& ud) { m_ud = std::move(ud); }
	const std::unique_ptr<UserData>& GetUD() const { return m_ud; }

private:
	std::string m_name;
	std::string m_filepath;

	std::shared_ptr<WxBitmap> m_bitmap = nullptr;

	std::unique_ptr<UserData> m_ud = nullptr;

	bool m_show;

}; // WxLibraryItem

}