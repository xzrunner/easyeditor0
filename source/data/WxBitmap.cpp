#include "ee0/WxBitmap.h"
#include "ee0/CompSymEditor.h"

#include <gimg_typedef.h>
#include <gimg_import.h>
#include <pimg/Condense.h>
#include <pimg/Rect.h>
#include <sprite2/SymType.h>
#include <sprite2/DrawRT.h>
#include <sprite2/Symbol.h>
#include <s2loader/SymbolFile.h>
#include <gum/Config.h>
#include <gum/SymbolPool.h>

#include <wx/image.h>

namespace ee0
{

static const int SMALL_SIZE = 24;

static const float MAX_WIDTH = 150.0f;
static const float SCALE = 0.5f;

WxBitmap::WxBitmap(const std::string& filepath)
{
	static bool inited = false;
	if (!inited)
	{
		wxInitAllImageHandlers();
		inited = true;
	}

	LoadFromFile(filepath);
}

bool WxBitmap::LoadFromFile(const std::string& filepath)
{
	int type = s2loader::SymbolFile::Instance()->Type(filepath.c_str());
	if (type == s2::SYM_INVALID || type == s2::SYM_UNKNOWN) {
		return false;
	} else if (type == s2::SYM_IMAGE) {
		return LoadFromImageFile(filepath);
	} else {
		return LoadFromSymbol(filepath, type);
	}
}

bool WxBitmap::LoadFromImageFile(const std::string& filepath)
{
	wxImage image;

	int width, height, format;
	uint8_t* pixels = gimg_import(filepath.c_str(), &width, &height, &format);
	if (format == GPF_RGBA8 && gum::Config::Instance()->GetPreMulAlpha()) {
		gimg_pre_mul_alpha(pixels, width, height);
	}
	if (format != GPF_RGBA8) {
		image.LoadFile(filepath.c_str());
		return true;
	}

	if (CanLoadFromWX(filepath))
	{
		wxImage wx_img;
		bool succ = wx_img.LoadFile(filepath.c_str());

		pimg::Condense cd(pixels, width, height);
		pimg::Rect r = cd.GetRegion();

		wxRect wx_rect;
		int h = height;
		wx_rect.SetLeft(r.xmin);
		wx_rect.SetRight(r.xmax - 1);
		wx_rect.SetTop(h - r.ymax);
		wx_rect.SetBottom(h - r.ymin - 1);

		image = wx_img.GetSubImage(wx_rect);
		LoadFromImage(image, true);

		return true;
	}
	else
	{
		uint8_t* rgb = gimg_rgba2rgb(pixels, width, height);
		bool succ = image.Create(wxSize(width, height), rgb);
		if (succ) {
			LoadFromImage(image, true);
			return true;
		} else {
			return false;
		}
	}
}

void WxBitmap::LoadFromImage(const wxImage& image, bool need_scale)
{
	{
		float w = image.GetWidth(),
			  h = image.GetHeight();
		float scale = 1;
		if (need_scale) {
			scale = w > (MAX_WIDTH / SCALE) ? (MAX_WIDTH / w) : SCALE; 
		}
		w = std::max(1.0f, w * scale);
		h = std::max(1.0f, h * scale);
		m_bmp_large = std::make_unique<wxBitmap>(image.Scale(w, h));
	}
	{
		float w = image.GetWidth(),
			h = image.GetHeight();
		float scale = (float)SMALL_SIZE / w;
		w = std::max(1.0f, w * scale);
		h = std::max(1.0f, h * scale);
		m_bmp_small = std::make_unique<wxBitmap>(image.Scale(w, h));
	}
}

bool WxBitmap::LoadFromSymbol(const std::string& filepath, int type)
{
	auto sym = gum::SymbolPool::Instance()->Fetch(filepath.c_str(), type);
	if (!sym) {
		return false;
	}
	
	sm::rect rect = sym->GetBounding();
	if (!rect.IsValid()) {
		return false;
	}

	float w = std::max(1.0f, rect.Size().x),
		  h = std::max(1.0f, rect.Size().y);
	float scale = w > (MAX_WIDTH / SCALE) ? (MAX_WIDTH / w) : SCALE; 
	w *= scale;
	h *= scale;
	w = std::max(1.0f, w);
	h = std::max(1.0f, h);

 	s2::DrawRT rt;
 	rt.Draw(*sym, true, scale);
 	unsigned char* rgba = rt.StoreToMemory(w, h, 4);
 	if (!rgba) {
 		return false;
 	}
 
 	uint8_t* rgb = gimg_rgba2rgb(rgba, w, h);
 
 	wxImage image(w, h, rgb, true);
	LoadFromImage(image, false);
 
 	free(rgb);
 	delete[] rgba;

	return true;
}

bool WxBitmap::CanLoadFromWX(const std::string& filepath)
{
	const wxList& list = wxImage::GetHandlers();
	for (wxList::compatibility_iterator node = list.GetFirst(); node; node = node->GetNext())
	{
		auto handler = (wxImageHandler*)node->GetData();
		if (handler->CanRead(filepath)) {
			return true;
		}
	}
	return false;
}

}