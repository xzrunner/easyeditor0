#pragma once

#include <wx/panel.h>

namespace pt2 { class Color; }

namespace ee0
{

class SubjectMgr;

class WxColorSpinCtrl : public wxPanel
{
public:
	WxColorSpinCtrl(wxWindow* parent, pt2::Color& col, 
		const std::string& title, SubjectMgr& sub_mgr);

	void Refresh();

private:
	void InitLayout(const std::string& title);

	void UpdateColorValue(wxSpinEvent& event);
	void UpdateTextValue(wxCommandEvent& event);

	void SetColor(int id);

private:
	SubjectMgr& m_sub_mgr;

	pt2::Color& m_col;

	wxSpinCtrl *m_r, *m_g, *m_b, *m_a;

}; // WxColorSpinCtrl

}