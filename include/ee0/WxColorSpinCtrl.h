#pragma once

#include "ee0/typedef.h"

#include <wx/panel.h>

namespace pt0 { class Color; }

namespace ee0
{

class WxColorSpinCtrl : public wxPanel
{
public:
	WxColorSpinCtrl(wxWindow* parent, pt0::Color& col,
		const std::string& title, const SubjectMgrPtr& sub_mgr);

	void Refresh();

private:
	void InitLayout(const std::string& title);

	void UpdateColorValue(wxSpinEvent& event);
	void UpdateTextValue(wxCommandEvent& event);

	void SetColor(int id);

private:
	SubjectMgrPtr m_sub_mgr;

	pt0::Color& m_col;

	wxSpinCtrl *m_r, *m_g, *m_b, *m_a;

}; // WxColorSpinCtrl

}