#pragma once

#include <wx/bitmap.h>

#include <string>
#include <memory>

namespace ee0
{

class WxBitmap
{
public:
	WxBitmap();

	const std::unique_ptr<wxBitmap>& GetLargeBmp() const { return m_bmp_large; }
	const std::unique_ptr<wxBitmap>& GetSmallBmp() const { return m_bmp_small; }

	bool LoadFromFile(const std::string& filepath);

private:
	bool LoadFromImageFile(const std::string& filepath);
	void LoadFromImage(const wxImage& image, bool scale);

	bool LoadFromJsonFile(const std::string& filepath);

	static bool CanLoadFromWX(const std::string& filepath);

private:
	std::unique_ptr<wxBitmap> m_bmp_large = nullptr;
	std::unique_ptr<wxBitmap> m_bmp_small = nullptr;

}; // WxBitmap

}