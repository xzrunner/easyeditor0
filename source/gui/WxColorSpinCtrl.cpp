#include "ee0/WxColorSpinCtrl.h"
#include "ee0/SubjectMgr.h"
#include "ee0/MessageID.h"

#include <painting0/Color.h>

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>

namespace ee0
{

WxColorSpinCtrl::WxColorSpinCtrl(wxWindow* parent, pt0::Color& col,
	                             const std::string& title, const SubjectMgrPtr& sub_mgr)
	: wxPanel(parent, wxID_ANY)
	, m_col(col)
	, m_sub_mgr(sub_mgr)
{
	InitLayout(title);
}

void WxColorSpinCtrl::Refresh()
{
	m_r->SetValue(m_col.r);
	m_g->SetValue(m_col.g);
	m_b->SetValue(m_col.b);
	m_a->SetValue(m_col.a);
}

void WxColorSpinCtrl::InitLayout(const std::string& title)
{
	static const wxSize SIZE(50, 19);

	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	sizer->Add(new wxStaticText(this, wxID_ANY, title));

	sizer->Add(m_r = new wxSpinCtrl(this, wxID_ANY, std::to_string(m_col.r),
		wxDefaultPosition, SIZE, wxSP_ARROW_KEYS, 0, 255, m_col.r));
	sizer->Add(m_g = new wxSpinCtrl(this, wxID_ANY, std::to_string(m_col.g),
		wxDefaultPosition, SIZE, wxSP_ARROW_KEYS, 0, 255, m_col.g));
	sizer->Add(m_b = new wxSpinCtrl(this, wxID_ANY, std::to_string(m_col.b),
		wxDefaultPosition, SIZE, wxSP_ARROW_KEYS, 0, 255, m_col.b));
	sizer->Add(m_a = new wxSpinCtrl(this, wxID_ANY, std::to_string(m_col.a),
		wxDefaultPosition, SIZE, wxSP_ARROW_KEYS, 0, 255, m_col.a));

	Connect(m_r->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED,
		wxSpinEventHandler(WxColorSpinCtrl::UpdateColorValue));
	Connect(m_g->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED,
		wxSpinEventHandler(WxColorSpinCtrl::UpdateColorValue));
	Connect(m_b->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED,
		wxSpinEventHandler(WxColorSpinCtrl::UpdateColorValue));
	Connect(m_a->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED,
		wxSpinEventHandler(WxColorSpinCtrl::UpdateColorValue));

	Connect(m_r->GetId(), wxEVT_COMMAND_TEXT_UPDATED,
		wxCommandEventHandler(WxColorSpinCtrl::UpdateTextValue));
	Connect(m_g->GetId(), wxEVT_COMMAND_TEXT_UPDATED,
		wxCommandEventHandler(WxColorSpinCtrl::UpdateTextValue));
	Connect(m_b->GetId(), wxEVT_COMMAND_TEXT_UPDATED,
		wxCommandEventHandler(WxColorSpinCtrl::UpdateTextValue));
	Connect(m_a->GetId(), wxEVT_COMMAND_TEXT_UPDATED,
		wxCommandEventHandler(WxColorSpinCtrl::UpdateTextValue));

	SetSizer(sizer);
}

void WxColorSpinCtrl::UpdateColorValue(wxSpinEvent& event)
{
	SetColor(event.GetId());
}

void WxColorSpinCtrl::UpdateTextValue(wxCommandEvent& event)
{
	SetColor(event.GetId());
}

void WxColorSpinCtrl::SetColor(int id)
{
	if (id == m_r->GetId()) {
		m_col.r = m_r->GetValue();
	} else if (id == m_g->GetId()) {
		m_col.g = m_g->GetValue();
	} else if (id == m_b->GetId()) {
		m_col.b = m_b->GetValue();
	} else if (id == m_a->GetId()) {
		m_col.a = m_a->GetValue();
	}

	m_sub_mgr->NotifyObservers(MSG_SET_CANVAS_DIRTY);
}

}