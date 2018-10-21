#pragma once

#include <wx/panel.h>
#include <wx/image.h>
#include <wx/bitmap.h>

namespace ee0
{

class WxImagePanel : public wxPanel
{
public:
	WxImagePanel(wxWindow* parent, const std::string& filepath, float max_len);

private:
	void OnPaint(wxPaintEvent& event);

	void Draw(wxDC& dc) const;

	void Resize(float max_len);

private:
	wxImage m_image;
	wxBitmap m_resized;

	DECLARE_EVENT_TABLE()

}; // WxImagePanel

}