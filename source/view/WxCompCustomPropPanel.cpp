#include "ee0/WxCompCustomPropPanel.h"
#include "ee0/CompCustomProp.h"

#include <SM_Vector.h>
#include <node0/SceneNode.h>
#include <painting2/Color.h>
#include <sx/StringHelper.h>

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

	std::string GetKey() const { return m_key->GetValue().ToStdString(); }

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

		// key
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, "Variable name:"));
			sizer->AddSpacer(25);
			sizer->Add(m_key = new wxTextCtrl(this, wxID_ANY,
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
	wxTextCtrl* m_key;

}; // WxCustomPropertyDlg

}

namespace ee0
{

WxCompCustomPropPanel::WxCompCustomPropPanel(wxWindow* parent, 
	                                           CompCustomProp& cprop)
	: WxCompPanel(parent, "CustomProperties")
	, m_cprop(cprop)
{
	InitLayout();
	Expand();
}

void WxCompCustomPropPanel::RefreshNodeComp()
{
}

void WxCompCustomPropPanel::InitLayout()
{
	wxWindow* win = GetPane();

	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);
	InitHeader(win, pane_sizer);
	InitPropertyGrid(win, pane_sizer);

	win->SetSizer(pane_sizer);
	pane_sizer->SetSizeHints(win);
}

void WxCompCustomPropPanel::InitHeader(wxWindow* win, wxSizer* pane_sizer)
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	m_add_btn = new wxButton(win, wxID_ANY, "+", wxDefaultPosition, wxSize(20, 20));
	Connect(m_add_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(WxCompCustomPropPanel::OnAddPress));
	sizer->Add(m_add_btn, 0, wxLEFT | wxRIGHT, 5);

	pane_sizer->Add(sizer);
}

void WxCompCustomPropPanel::InitPropertyGrid(wxWindow* win, wxSizer* pane_sizer)
{
	m_pg = new wxPropertyGrid(win,
		-1,
		wxDefaultPosition,
		wxSize(200, 100),
		wxPG_SPLITTER_AUTO_CENTER | wxPG_BOLD_MODIFIED
	);
	Connect(m_pg->GetId(), wxEVT_PG_CHANGED, wxPropertyGridEventHandler(
		WxCompCustomPropPanel::OnPropertyGridChange));
	pane_sizer->Add(m_pg, 1, wxEXPAND);

	InitProperties();
}

void WxCompCustomPropPanel::InitProperties()
{
	auto& props = m_cprop.GetAllProp();
	for (auto& prop : props)
	{
		auto& name = prop.key;
		switch (prop.type)
		{
		case CompCustomProp::PROP_BOOL:
			m_pg->Append(new wxBoolProperty(name, wxPG_LABEL, prop.val.m_val.bl));
			m_pg->SetPropertyAttribute(name.c_str(), wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
			break;
		case CompCustomProp::PROP_INT:
			m_pg->Append(new wxIntProperty(name, wxPG_LABEL, prop.val.m_val.l));
			break;
		case CompCustomProp::PROP_FLOAT:
			m_pg->Append(new wxFloatProperty(name, wxPG_LABEL, prop.val.m_val.flt));
			break;
		case CompCustomProp::PROP_STRING:
			m_pg->Append(new wxStringProperty(name, wxPG_LABEL, prop.val.m_val.pc));
			break;
		case CompCustomProp::PROP_VEC2:
			{
				wxPGProperty* pos_prop = m_pg->Append(new wxStringProperty(name, wxPG_LABEL, wxT("<composed>")));
				pos_prop->SetExpanded(false);
				auto str_x = name + ".X";
				auto str_y = name + ".Y";
				sm::vec2* vec2 = static_cast<sm::vec2*>(prop.val.m_val.pv);
				m_pg->AppendIn(pos_prop, new wxFloatProperty(wxT("X"), wxPG_LABEL, vec2->x));
				m_pg->SetPropertyAttribute(str_x.c_str(), wxPG_ATTR_UNITS, wxT("pixels"));
				m_pg->SetPropertyAttribute(str_x.c_str(), "Precision", 1);
				m_pg->AppendIn(pos_prop, new wxFloatProperty(wxT("Y"), wxPG_LABEL, vec2->y));
				m_pg->SetPropertyAttribute(str_y.c_str(), wxPG_ATTR_UNITS, wxT("pixels"));
				m_pg->SetPropertyAttribute(str_y.c_str(), "Precision", 1);
			}
			break;
		case CompCustomProp::PROP_COLOR:
			{
				auto pcol = static_cast<pt2::Color*>(prop.val.m_val.pv);
				wxColour col = wxColour(pcol->r, pcol->g, pcol->b, pcol->a);
				m_pg->Append(new wxColourProperty(name, wxPG_LABEL, col));
				m_pg->SetPropertyAttribute(name.c_str(), "HasAlpha", true);
			}
			break;
		}
	}
}

void WxCompCustomPropPanel::OnPropertyGridChange(wxPropertyGridEvent& event)
{
	wxPGProperty* property = event.GetProperty();
	auto key = property->GetName().ToStdString();
	wxAny val = property->GetValue();
	
	auto& props = m_cprop.GetAllProp();
	for (auto& prop : props)
	{
		if (prop.key == key)
		{
			switch (prop.type)
			{
			case CompCustomProp::PROP_BOOL:
				prop.val.m_val.bl = wxANY_AS(val, bool);
				break;
			case CompCustomProp::PROP_INT:
				prop.val.m_val.l = wxANY_AS(val, int);
				break;
			case CompCustomProp::PROP_FLOAT:
				prop.val.m_val.flt = wxANY_AS(val, float);
				break;
			case CompCustomProp::PROP_STRING:
				{
					delete[] prop.val.m_val.pc;
					auto str = wxANY_AS(val, wxString).ToStdString();
					char* tmp = new char[str.size() + 1];
					strcpy(tmp, str.c_str());
					prop.val.m_val.pc = tmp;
				}
				break;
			case CompCustomProp::PROP_VEC2:
				{
					std::vector<std::string> tokens;
					auto str = wxANY_AS(val, wxString).ToStdString();
					sx::StringHelper::Split(str, ";", tokens);
					GD_ASSERT(tokens.size() == 2, "err prop str");

					auto pvec2 = static_cast<sm::vec2*>(prop.val.m_val.pv);
					pvec2->x = std::stof(tokens[0]);
					pvec2->y = std::stof(tokens[1]);
				}
				break;
			case CompCustomProp::PROP_COLOR:
				{
					wxColour col = wxANY_AS(val, wxColour);
					
					auto pcol = static_cast<pt2::Color*>(prop.val.m_val.pv);
					pcol->r = col.Red();
					pcol->g = col.Green();
					pcol->b = col.Blue();
					pcol->a = col.Alpha();
				}
				break;
			}
		}
	}
}

void WxCompCustomPropPanel::OnAddPress(wxCommandEvent& event)
{
	WxCustomPropertyDlg dlg(this, m_add_btn->GetScreenPosition());
	if (dlg.ShowModal() != wxID_OK) {
		return;
	}

	CompCustomProp::Property prop;

	std::string key = dlg.GetKey();
	prop.key  = key;
	prop.type = static_cast<CompCustomProp::PropertyType>(dlg.GetType());

	switch (dlg.GetType())
	{
	case CompCustomProp::PROP_BOOL:
		m_pg->Append(new wxBoolProperty(key, wxPG_LABEL));
		m_pg->SetPropertyAttribute(key.c_str(), wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
		prop.val.m_type = VT_BOOL;
		break;
	case CompCustomProp::PROP_INT:
		m_pg->Append(new wxIntProperty(key, wxPG_LABEL));
		prop.val.m_type = VT_LONG;
		break;
	case CompCustomProp::PROP_FLOAT:
		m_pg->Append(new wxFloatProperty(key, wxPG_LABEL));
		prop.val.m_type = VT_FLOAT;
		break;
	case CompCustomProp::PROP_STRING:
		m_pg->Append(new wxStringProperty(key, wxPG_LABEL));
		prop.val.m_type = VT_PCHAR;
		prop.val.m_val.pc = nullptr;
		break;
	case CompCustomProp::PROP_VEC2:
		{
			wxPGProperty* pos_prop = m_pg->Append(new wxStringProperty(key, wxPG_LABEL, wxT("<composed>")));
			pos_prop->SetExpanded(false);
			auto str_x = key + ".X";
			auto str_y = key + ".Y";
			m_pg->AppendIn(pos_prop, new wxFloatProperty(wxT("X"), wxPG_LABEL));
			m_pg->SetPropertyAttribute(str_x.c_str(), wxPG_ATTR_UNITS, wxT("pixels"));
			m_pg->SetPropertyAttribute(str_x.c_str(), "Precision", 1);
			m_pg->AppendIn(pos_prop, new wxFloatProperty(wxT("Y"), wxPG_LABEL));
			m_pg->SetPropertyAttribute(str_y.c_str(), wxPG_ATTR_UNITS, wxT("pixels"));
			m_pg->SetPropertyAttribute(str_y.c_str(), "Precision", 1);

			prop.val.m_type = VT_PVOID;
			prop.val.m_val.pv = new sm::vec2(0, 0);
		}
		break;
	case CompCustomProp::PROP_COLOR:
		m_pg->Append(new wxColourProperty(key, wxPG_LABEL, wxColour(255, 255, 255)));
		m_pg->SetPropertyAttribute(key.c_str(), "HasAlpha", true);
		prop.val.m_type = VT_PVOID;
		prop.val.m_val.pv = new pt2::Color(255, 255, 255);
		break;
	}

	m_cprop.Add(prop);
}

}