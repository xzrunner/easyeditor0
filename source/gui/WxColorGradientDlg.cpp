#include "ee0/WxColorGradientDlg.h"

#include <sm_const.h>
#include <painting2/GradientColor.h>

#include <wx/sizer.h>
#include <wx/clrpicker.h>
#include <wx/textctrl.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/dcclient.h>
#include <wx/colordlg.h>

namespace
{

wxColour ToWxColor(const pt0::Color& col)
{
	return wxColour(col.r, col.g, col.b, col.a);
}

}

namespace ee0
{

WxColorGradientDlg::WxColorGradientDlg(wxWindow* parent, const pt2::GradientColor& col)
	: wxDialog(parent, wxID_ANY, "Color Gradient", wxGetMousePosition() - wxPoint(500, 350))
	, m_col(col)
{
	InitLayout();
}

void WxColorGradientDlg::InitLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
	// preview
	{
		top_sizer->Add(m_preview = new ColorPreview(this, wxSize(400, 50), m_col), 0, wxCENTER);
	}
	// control
	{
		top_sizer->Add(m_slider = new ColorSlider(this, wxSize(400, 20), m_col), 0, wxCENTER);
	}
	top_sizer->AddSpacer(10);
	// angle
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Angle ")));

		int deg = static_cast<int>(m_col.angle * SM_RAD_TO_DEG);
		sizer->Add(m_angle = new wxSpinCtrl(this, wxID_ANY, std::to_string(deg),
			wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 90, deg));
		Connect(m_angle->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED,
			wxSpinEventHandler(WxColorGradientDlg::SpinEventHandler));

		top_sizer->Add(sizer, 0, wxCENTER);

	}
	top_sizer->AddSpacer(10);
	// default
	{
		SetEscapeId(wxID_CANCEL);
		top_sizer->Add(CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxCENTER);
	}
	SetSizer(top_sizer);
	top_sizer->Fit(this);
}

void WxColorGradientDlg::SpinEventHandler(wxSpinEvent& event)
{
	if (event.GetId() == m_angle->GetId())
	{
		m_col.angle = m_angle->GetValue() * SM_DEG_TO_RAD;
	}
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

WxColorGradientDlg::ColorPreview::Canvas::
Canvas(wxWindow* parent, pt2::GradientColor& col)
	: WxSimpleCanvas(parent)
	, m_col(col)
{
}

void WxColorGradientDlg::ColorPreview::Canvas::
OnDraw() const
{
	if (m_col.items.empty()) {
		return;
	}

	glBegin(GL_TRIANGLE_STRIP);

	DrawItem(m_col.items.front().col, 0);
	for (auto& item : m_col.items) {
		if (item.pos < 0) {
			continue;
		}
		DrawItem(item.col, item.pos);
	}
	DrawItem(m_col.items.back().col, 1);

	glEnd();
}

void WxColorGradientDlg::ColorPreview::Canvas::
DrawItem(const pt0::Color& col, float pos) const
{
	glColor3ub(col.r, col.g, col.b);
	float x = -1 + 2 * pos;
	glVertex2f(x, -1);
	glVertex2f(x, 1);
}

//////////////////////////////////////////////////////////////////////////
// class WxColorGradientDlg::ColorSlider
//////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(WxColorGradientDlg::ColorSlider, wxPanel)
	EVT_PAINT(WxColorGradientDlg::ColorSlider::OnPaint)
	EVT_MOUSE_EVENTS(WxColorGradientDlg::ColorSlider::OnMouse)
END_EVENT_TABLE()

WxColorGradientDlg::ColorSlider::
ColorSlider(wxWindow* parent, wxSize size, pt2::GradientColor& col)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, size, wxBORDER_DEFAULT)
	, m_col(col)
	, m_selected(-1)
{
}

void WxColorGradientDlg::ColorSlider::
OnPaint(wxPaintEvent& event)
{
	auto sz = GetSize();

	wxPaintDC dc(this);
	for (auto& item : m_col.items)
	{
		dc.SetBrush(wxBrush(ToWxColor(item.col)));
		dc.DrawRectangle(
			wxPoint(sz.x * item.pos - HALF_WIDTH, 0),
			wxSize(HALF_WIDTH * 2, sz.GetHeight()));
	}
}

void WxColorGradientDlg::ColorSlider::
OnMouse(wxMouseEvent& event)
{
	int w = GetSize().x;
	int x = event.GetX();
	// insert
	if (event.LeftDown())
	{
		SelectByPos(x);
		if (m_selected == -1)
		{
			float pos = static_cast<float>(x) / w;
			if (pos < m_col.items.front().pos)
			{
				m_col.items.insert(m_col.items.begin(), pt2::GradientColor::Item(pos));
				m_selected = 0;
			}
			else
			{
				for (int i = m_col.items.size() - 1; i >= 0; --i)
				{
					if (pos > m_col.items[i].pos) {
						m_col.items.insert(m_col.items.begin() + i + 1, pt2::GradientColor::Item(pos));
						m_selected = i + 1;
						break;
					}
				}
			}
			m_parent->Refresh();
		}
	}
	// remove
	else if (event.RightDown())
	{
		SelectByPos(x);
		if (m_selected != -1)
		{
			m_col.items.erase(m_col.items.begin() + m_selected);
			m_parent->Refresh();
		}
	}
	// move
	else if (event.Dragging())
	{
		if (m_selected != -1)
		{
			float pos = static_cast<float>(x) / w;
			m_col.items[m_selected].pos = pos;
			m_parent->Refresh();
		}
	}
	// set color
	else if (event.LeftDClick())
	{
		if (m_selected != -1)
		{
			auto& item = m_col.items[m_selected];

			wxColourData data;
			data.SetColour(wxColour(item.col.r, item.col.g, item.col.b));
			wxColourDialog dlg(m_parent, &data);

			dlg.SetTitle(wxT("Set Color"));

			wxPoint pos = wxGetMousePosition();
			pos.x -= 400;
			dlg.SetPosition(pos);
			if (dlg.ShowModal() == wxID_OK)
			{
				const wxColor& col = dlg.GetColourData().GetColour();
				item.col.r = col.Red();
				item.col.g = col.Green();
				item.col.b = col.Blue();
				item.col.a = col.Alpha();

				m_parent->Refresh();
			}
		}
	}
}

void WxColorGradientDlg::ColorSlider::
SelectByPos(int x)
{
	int w = GetSize().x;
	int idx = 0;
	for (auto& item : m_col.items)
	{
		float pos = w * item.pos;
		if (x >= pos - HALF_WIDTH && x <= pos + HALF_WIDTH) {
			m_selected = idx;
			return;
		}
		++idx;
	}
	m_selected = -1;
}

}