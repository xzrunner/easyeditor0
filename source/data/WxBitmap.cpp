#include "ee0/WxBitmap.h"

#include <sx/ResFileHelper.h>
#include <gimg_typedef.h>
#include <gimg_import.h>
#include <pimg/Condense.h>
#include <pimg/Rect.h>
#include <ns/CompFactory.h>
#include <node0/CompAsset.h>
#include <node2/RenderSystem.h>
#include <painting2/DrawRT.h>

#include <wx/image.h>

#include <boost/filesystem.hpp>

namespace ee0
{

static const int SMALL_SIZE = 24;

static const float MAX_WIDTH = 150.0f;
static const float SCALE = 0.5f;

WxBitmap::WxBitmap()
{
	static bool inited = false;
	if (!inited)
	{
		wxInitAllImageHandlers();
		inited = true;
	}
}

bool WxBitmap::LoadFromFile(const std::string& filepath)
{
	bool ret = false;
	auto type = sx::ResFileHelper::Type(filepath);
	switch (type)
	{
	case sx::RES_FILE_IMAGE:
		ret = LoadFromImageFile(filepath);
		break;
	case sx::RES_FILE_JSON:
		ret = LoadFromJsonFile(filepath);
		break;
	}
	return ret;
}

bool WxBitmap::LoadFromImageFile(const std::string& filepath)
{
	int width, height, format;
	uint8_t* pixels = gimg_import(filepath.c_str(), &width, &height, &format);
	if (!pixels) {
		return false;
	}

	// todo
	//if (format == GPF_RGBA8 && gum::Config::Instance()->GetPreMulAlpha()) {
	//	gimg_pre_mul_alpha(pixels, width, height);
	//}

	if (CanLoadFromWX(filepath))
	{
		wxImage wx_img;
		bool succ = wx_img.LoadFile(filepath.c_str());
		if (!succ) {
			return false;
		}

		// condense transparent edge
		if (format == GPF_RGBA8 || format == GPF_RGBA4 || format == GPF_BGRA_EXT)
		{
			pimg::Condense cd(pixels, width, height);
			pimg::Rect r = cd.GetRegion();

			wxRect wx_rect;
			int h = height;
			wx_rect.SetLeft(r.xmin);
			wx_rect.SetRight(r.xmax - 1);
			wx_rect.SetTop(h - r.ymax);
			wx_rect.SetBottom(h - r.ymin - 1);

			auto sub_img = wx_img.GetSubImage(wx_rect);
			LoadFromImage(sub_img, true);

			return true;
		}
		else
		{
			LoadFromImage(wx_img, true);
			return true;
		}
	}
	else
	{
        uint8_t* rgb8 = nullptr;
        switch (format)
        {
        case GPF_RGBA8:
            rgb8 = gimg_rgba8_to_rgb8(pixels, width, height);
            break;
        case GPF_RGB32F:
            gimg_revert_y(pixels, width, height, format);
            rgb8 = gimg_rgb32f_to_rgb8(pixels, width, height);
            break;
        }

        if (rgb8)
        {
            wxImage wx_img(width, height, rgb8);
            LoadFromImage(wx_img, true);
            return true;
        }
        else
        {
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

bool WxBitmap::LoadFromJsonFile(const std::string& filepath)
{
	auto casset = ns::CompFactory::Instance()->CreateAsset(filepath);
	if (!casset) {
		return false;
	}

	auto bounding = pt2::Callback::GetBounding(*casset);
	if (!bounding.IsValid()) {
		return false;
	}

	float w = std::max(1.0f, bounding.Size().x),
		  h = std::max(1.0f, bounding.Size().y);
	float scale = w > (MAX_WIDTH / SCALE) ? (MAX_WIDTH / w) : SCALE;
	w *= scale;
	h *= scale;
	w = std::max(1.0f, w);
	h = std::max(1.0f, h);

	pt2::DrawRT rt;
	rt.Draw<n0::CompAsset>(*casset, [](const n0::CompAsset& casset, const sm::Matrix2D& mt) {
		n2::RenderSystem::Instance()->Draw(casset, mt);
	}, true, scale);
 	unsigned char* rgba = rt.StoreToMemory(w, h, 4);
 	if (!rgba) {
 		return false;
 	}

 	uint8_t* rgb = gimg_rgba8_to_rgb8(rgba, w, h);

 	wxImage image(w, h, rgb, true);
	LoadFromImage(image, false);

 	free(rgb);
 	delete[] rgba;

	return true;
}

bool WxBitmap::CanLoadFromWX(const std::string& filepath)
{
	const wxList& list = wxImage::GetHandlers();
	for (wxList::compatibility_iterator obj = list.GetFirst(); obj; obj = obj->GetNext())
	{
		auto handler = (wxImageHandler*)obj->GetData();
		if (handler->CanRead(filepath)) {
			return true;
		}
	}
	return false;
}

}