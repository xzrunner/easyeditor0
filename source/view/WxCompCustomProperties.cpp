#include "ee0/WxCompCustomProperties.h"
#include "ee0/CompCustomProperties.h"

#include <SM_Vector.h>
#include <node0/SceneNode.h>
#include <painting2/Color.h>

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

namespace
{

const wxString TYPE_LABELS[] = { 
	"bool", "int", "float", "string", "vec2", "color" 
};

class WxCustomPropertyDlg : public wxDialog
{
public:
	WxCustomPropertyDlg(wxWindow* parent, const wxPoint& pos = wxDefaultPosition)
		: wxDialog(parent, wxID_ANY, "Create custom property", pos)
	{
		InitLayout();
	}

	int GetType() const { return m_type->GetSelection(); }

	std::string GetVarName() const { return m_var_name->GetValue().ToStdString(); }

	std::string GetDisName() const { return m_dis_name->GetValue().ToStdString(); }

private:
	void InitLayout()
	{
		wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

		top_sizer->AddSpacer(10);

		// type
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

			sizer->Add(new wxStaticText(this, wxID_ANY, "Type:"));

			sizer->AddSpacer(80);

			sizer->Add(m_type = new wxChoice(this, wxID_ANY, wxDefaultPosition,
				wxSize(200, -1), 6, TYPE_LABELS));
			m_type->SetSelection(0);

			top_sizer->Add(sizer);
		}

		top_sizer->AddSpacer(10);

		// variable name
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, "Variable name:"));
			sizer->AddSpacer(25);
			sizer->Add(m_var_name = new wxTextCtrl(this, wxID_ANY,
				wxEmptyString, wxDefaultPosition, wxSize(200, -1)));
			top_sizer->Add(sizer);
		}

		top_sizer->AddSpacer(10);

		// display name
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, "Display name:"));
			sizer->AddSpacer(30);
			sizer->Add(m_dis_name = new wxTextCtrl(this, wxID_ANY, 
				wxEmptyString, wxDefaultPosition, wxSize(200, -1)));
			top_sizer->Add(sizer);
		}

		top_sizer->AddSpacer(20);

		// btn
		{
			wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxButton(this, wxID_OK), wxALL, 5);
			sizer->Add(new wxButton(this, wxID_CANCEL), wxALL, 5);
			top_sizer->Add(sizer, 0, wxALIGN_CENTER);
		}

		SetSizer(top_sizer);
		top_sizer->Fit(this);
	}

private:
	wxChoice*   m_type;
	wxTextCtrl* m_var_name;
	wxTextCtrl* m_dis_name;

}; // WxCustomPropertyDlg

}

namespace ee0
{

WxCompCustomProperties::WxCompCustomProperties(wxWindow* parent, 
	                                           CompCustomProperties& cprop)
	: WxCompPanel(parent, "CustomProperties")
	, m_cprop(cprop)
{
	InitLayout();
	Expand();
}

void WxCompCustomProperties::RefreshNodeComp()
{
}

void WxCompCustomProperties::InitLayout()
{
	wxWindow* win = GetPane();

	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);
	InitHeader(win, pane_sizer);
	InitPropertyGrid(win, pane_sizer);

	win->SetSizer(pane_sizer);
	pane_sizer->SetSizeHints(win);
}

void WxCompCustomProperties::InitHeader(wxWindow* win, wxSizer* pane_sizer)
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	m_add_btn = new wxButton(win, wxID_ANY, "+", wxDefaultPosition, wxSize(20, 20));
	Connect(m_add_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(WxCompCustomProperties::OnAddPress));
	sizer->Add(m_add_btn, 0, wxLEFT | wxRIGHT, 5);

	pane_sizer->Add(sizer);
}

void WxCompCustomProperties::InitPropertyGrid(wxWindow* win, wxSizer* pane_sizer)
{
	m_pg = new wxPropertyGrid(win,
		-1,
		wxDefaultPosition,
		wxSize(200, 100),
		wxPG_SPLITTER_AUTO_CENTER | wxPG_BOLD_MODIFIED
	);
	Connect(m_pg->GetId(), wxEVT_PG_CHANGED, wxPropertyGridEventHandler(
		WxCompCustomProperties::OnPropertyGridChange));
	pane_sizer->Add(m_pg, 1, wxEXPAND);

	InitProperties();
}

void WxCompCustomProperties::InitProperties()
{
	auto& props = m_cprop.GetAllProp();
	for (auto& prop : props)
	{
		auto& dis_name = prop.dis_name;
		switch (prop.type)
		{
		case CompCustomProperties::PROP_BOOL:
			m_pg->Append(new wxBoolProperty(dis_name, wxPG_LABEL, prop.var.m_val.bl));
			m_pg->SetPropertyAttribute(dis_name.c_str(), wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
			break;
		case CompCustomProperties::PROP_INT:
			m_pg->Append(new wxIntProperty(dis_name, wxPG_LABEL, prop.var.m_val.l));
			break;
		case CompCustomProperties::PROP_FLOAT:
			m_pg->Append(new wxFloatProperty(dis_name, wxPG_LABEL, prop.var.m_val.flt));
			break;
		case CompCustomProperties::PROP_STRING:
			m_pg->Append(new wxStringProperty(dis_name, wxPG_LABEL, prop.var.m_val.pc));
			break;
		case CompCustomProperties::PROP_VEC2:
			{
				wxPGProperty* pos_prop = m_pg->Append(new wxStringProperty(dis_name, wxPG_LABEL, wxT("<composed>")));
				pos_prop->SetExpanded(false);
				auto str_x = dis_name + ".X";
				auto str_y = dis_name + ".Y";
				sm::vec2* vec2 = static_cast<sm::vec2*>(prop.var.m_val.pv);
				m_pg->AppendIn(pos_prop, new wxFloatProperty(wxT("X"), wxPG_LABEL, vec2->x));
				m_pg->SetPropertyAttribute(str_x.c_str(), wxPG_ATTR_UNITS, wxT("pixels"));
				m_pg->SetPropertyAttribute(str_x.c_str(), "Precision", 1);
				m_pg->AppendIn(pos_prop, new wxFloatProperty(wxT("Y"), wxPG_LABEL, vec2->y));
				m_pg->SetPropertyAttribute(str_y.c_str(), wxPG_ATTR_UNITS, wxT("pixels"));
				m_pg->SetPropertyAttribute(str_y.c_str(), "Precision", 1);
			}
			break;
		case CompCustomProperties::PROP_COLOR:
			{
				auto pcol = static_cast<pt2::Color*>(prop.var.m_val.pv);
				wxColour col = wxColour(pcol->r, pcol->g, pcol->b, pcol->a);
				m_pg->Append(new wxColourProperty(dis_name, wxPG_LABEL, col));
				m_pg->SetPropertyAttribute(dis_name.c_str(), "HasAlpha", true);
			}
			break;
		}
	}
}

void WxCompCustomProperties::OnPropertyGridChange(wxPropertyGridEvent& event)
{

}

void WxCompCustomProperties::OnAddPress(wxCommandEvent& event)
{
	WxCustomPropertyDlg dlg(this, m_add_btn->GetScreenPosition());
	if (dlg.ShowModal() != wxID_OK) {
		return;
	}

	CompCustomProperties::Property prop;

	std::string dis_name = dlg.GetDisName();
	std::string var_name = dlg.GetVarName();
	prop.dis_name = dis_name;
	prop.var_name = var_name;
	prop.type = static_cast<CompCustomProperties::PropertyType>(dlg.GetType());

	switch (dlg.GetType())
	{
	case CompCustomProperties::PROP_BOOL:
		m_pg->Append(new wxBoolProperty(dis_name, wxPG_LABEL));
		m_pg->SetPropertyAttribute(dis_name.c_str(), wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
		prop.var.m_type = VT_BOOL;
		break;
	case CompCustomProperties::PROP_INT:
		m_pg->Append(new wxIntProperty(dis_name, wxPG_LABEL));
		prop.var.m_type = VT_LONG;
		break;
	case CompCustomProperties::PROP_FLOAT:
		m_pg->Append(new wxFloatProperty(dis_name, wxPG_LABEL));
		prop.var.m_type = VT_FLOAT;
		break;
	case CompCustomProperties::PROP_STRING:
		m_pg->Append(new wxStringProperty(dis_name, wxPG_LABEL));
		prop.var.m_type = VT_PCHAR;
		break;
	case CompCustomProperties::PROP_VEC2:
		{
			wxPGProperty* pos_prop = m_pg->Append(new wxStringProperty(dis_name, wxPG_LABEL, wxT("<composed>")));
			pos_prop->SetExpanded(false);
			auto str_x = dis_name + ".X";
			auto str_y = dis_name + ".Y";
			m_pg->AppendIn(pos_prop, new wxFloatProperty(wxT("X"), wxPG_LABEL));
			m_pg->SetPropertyAttribute(str_x.c_str(), wxPG_ATTR_UNITS, wxT("pixels"));
			m_pg->SetPropertyAttribute(str_x.c_str(), "Precision", 1);
			m_pg->AppendIn(pos_prop, new wxFloatProperty(wxT("Y"), wxPG_LABEL));
			m_pg->SetPropertyAttribute(str_y.c_str(), wxPG_ATTR_UNITS, wxT("pixels"));
			m_pg->SetPropertyAttribute(str_y.c_str(), "Precision", 1);
		}
		prop.var.m_type = VT_PVOID;
		break;
	case CompCustomProperties::PROP_COLOR:
		m_pg->Append(new wxColourProperty(dis_name, wxPG_LABEL));
		m_pg->SetPropertyAttribute(dis_name.c_str(), "HasAlpha", true);
		prop.var.m_type = VT_PVOID;
		break;
	}

	m_cprop.Add(prop);
}

}