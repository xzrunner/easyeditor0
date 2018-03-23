#include "ee0/WxColorGradientDlg.h"

#include <sm_const.h>
#include <painting2/GradientColor.h>

#include <wx/sizer.h>
#include <wx/clrpicker.h>
#include <wx/textctrl.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>

namespace
{

wxColour ToWxColor(const pt2::Color& col)
{
	return wxColour(col.r, col.g, col.b, col.a);
}

}

namespace ee0
{

WxColorGradientDlg::WxColorGradientDlg(wxWindow* parent, const pt2::GradientColor& col)
	: wxDialog(parent, wxID_ANY, "Color Gradient", wxGetMousePosition() - wxPoint(500, 350), wxSize(450, 300))
	, m_col(col)
{
	InitColor();
	InitLayout();
}

pt2::GradientColor WxColorGradientDlg::GetColor() const 
{
	pt2::GradientColor ret = m_col;
	if (ret.items[1].pos < 0) {
		ret.items.erase(ret.items.begin() + 1);
	}
	return ret;
}

void WxColorGradientDlg::InitColor()
{
	if (m_col.items.size() != 3) {
		m_col.items.resize(3);
	}
	if (m_col.items[0].pos < 0) {
		m_col.items[0].pos = 0;
	}
	if (m_col.items[2].pos < 0) {
		m_col.items[2].pos = 1;
	}
}

void WxColorGradientDlg::InitLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
	// preview
	{
		top_sizer->Add(m_preview = new ColorPreview(this, wxSize(400, 50), m_col), wxALIGN_CENTER);
	}
	// color
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		m_begin_col = new wxColourPickerCtrl(this, wxID_ANY, ToWxColor(m_col.items[0].col));
		sizer->Add(m_begin_col);
		Connect(m_begin_col->GetId(), wxEVT_COLOURPICKER_CHANGED,
			wxColourPickerEventHandler(WxColorGradientDlg::ColourPickerEventHandler));

		m_mid_col = new wxColourPickerCtrl(this, wxID_ANY, ToWxColor(m_col.items[1].col));
		sizer->Add(m_mid_col);
		Connect(m_mid_col->GetId(), wxEVT_COLOURPICKER_CHANGED,
			wxColourPickerEventHandler(WxColorGradientDlg::ColourPickerEventHandler));

		m_end_col = new wxColourPickerCtrl(this, wxID_ANY, ToWxColor(m_col.items[2].col));
		sizer->Add(m_end_col);
		Connect(m_end_col->GetId(), wxEVT_COLOURPICKER_CHANGED,
			wxColourPickerEventHandler(WxColorGradientDlg::ColourPickerEventHandler));

		top_sizer->Add(sizer);
	}
	// pos
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		m_begin_pos = new wxTextCtrl(this, wxID_ANY, std::to_string(m_col.items[0].pos));
		sizer->Add(m_begin_pos);
		Connect(m_begin_pos->GetId(), wxEVT_COMMAND_TEXT_UPDATED,
			wxCommandEventHandler(WxColorGradientDlg::CommandEventHandler));

		m_mid_pos = new wxTextCtrl(this, wxID_ANY, std::to_string(m_col.items[1].pos));
		sizer->Add(m_mid_pos);
		Connect(m_mid_pos->GetId(), wxEVT_COMMAND_TEXT_UPDATED,
			wxCommandEventHandler(WxColorGradientDlg::CommandEventHandler));

		m_end_pos = new wxTextCtrl(this, wxID_ANY, std::to_string(m_col.items[2].pos));
		sizer->Add(m_end_pos);
		Connect(m_end_pos->GetId(), wxEVT_COMMAND_TEXT_UPDATED,
			wxCommandEventHandler(WxColorGradientDlg::CommandEventHandler));

		top_sizer->Add(sizer);
	}
	// angle
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Angle ")));

		int deg = static_cast<int>(m_col.angle * SM_RAD_TO_DEG);
		sizer->Add(m_angle = new wxSpinCtrl(this, wxID_ANY, std::to_string(deg),
			wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 90, deg));
		Connect(m_angle->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED,
			wxSpinEventHandler(WxColorGradientDlg::SpinEventHandler));

		top_sizer->Add(sizer);

	}
	// default
	{
		SetEscapeId(wxID_CANCEL);
		top_sizer->Add(CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxCENTER);
	}
	SetSizer(top_sizer);
	top_sizer->Fit(this);
}

void WxColorGradientDlg::CommandEventHandler(wxCommandEvent& event)
{
	if (event.GetId() == m_begin_pos->GetId()) {
		m_col.items[0].pos = std::stof(m_begin_pos->GetValue().ToStdString());
	} else if (event.GetId() == m_mid_pos->GetId()) {
		m_col.items[1].pos = std::stof(m_mid_pos->GetValue().ToStdString());
	} else if (event.GetId() == m_end_pos->GetId()) {
		m_col.items[2].pos = std::stof(m_end_pos->GetValue().ToStdString());
	}

	m_preview->Refresh();
}

void WxColorGradientDlg::SpinEventHandler(wxSpinEvent& event)
{
	if (event.GetId() == m_angle->GetId()) 
	{
		m_col.angle = m_angle->GetValue() * SM_DEG_TO_RAD;
	}
}

void WxColorGradientDlg::ColourPickerEventHandler(wxColourPickerEvent& event)
{
	if (event.GetId() == m_begin_col->GetId()) {
		auto col = m_begin_col->GetColour();
		m_col.items[0].col.FromABGR(col.GetRGBA());
	} else if (event.GetId() == m_mid_col->GetId()) {
		auto col = m_mid_col->GetColour();
		m_col.items[1].col.FromABGR(col.GetRGBA());
	} else if (event.GetId() == m_end_col->GetId()) {
		auto col = m_end_col->GetColour();
		m_col.items[2].col.FromABGR(col.GetRGBA());
	}

	m_preview->Refresh();
}

//////////////////////////////////////////////////////////////////////////
// class WxColorGradientDlg::ColorPreview
//////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(WxColorGradientDlg::ColorPreview, wxPanel)
	EVT_SIZE(WxColorGradientDlg::ColorPreview::OnSize)
END_EVENT_TABLE()

WxColorGradientDlg::ColorPreview::ColorPreview(wxWindow* parent, wxSize size, pt2::GradientColor& col)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, size, wxBORDER_DEFAULT)
{
	m_canvas = std::make_unique<Canvas>(this, col);
}

void WxColorGradientDlg::ColorPreview::OnSize(wxSizeEvent& event)
{
	m_canvas->SetSize(event.GetSize());
}

//////////////////////////////////////////////////////////////////////////
// class WxColorGradientDlg::ColorPreview::Canvas
//////////////////////////////////////////////////////////////////////////

WxColorGradientDlg::ColorPreview::Canvas::Canvas(wxWindow* parent, pt2::GradientColor& col)
	: SimpleGLCanvas(parent)
	, m_col(col)
{
}

void WxColorGradientDlg::ColorPreview::Canvas::OnDraw() const
{
	glBegin(GL_TRIANGLE_STRIP);

	DrawItem(m_col.items[0].col, 0);
	DrawItem(m_col.items[0].col, m_col.items[0].pos);
	if (m_col.items[1].pos > 0) {
		DrawItem(m_col.items[1].col, m_col.items[1].pos);
	}
	DrawItem(m_col.items[2].col, m_col.items[2].pos);
	DrawItem(m_col.items[2].col, 1);

	glEnd();
}

void WxColorGradientDlg::ColorPreview::Canvas::DrawItem(const pt2::Color& col, float pos) const
{
	glColor3ub(col.r, col.g, col.b);
	float x = -1 + 2 * pos;
	glVertex2f(x, -1);
	glVertex2f(x, 1);
}

}