#include "ee0/WxCompNodeEditorPanel.h"
#include "ee0/SubjectMgr.h"
#include "ee0/MessageID.h"

#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/checkbox.h>

namespace ee0
{

WxCompNodeEditorPanel::WxCompNodeEditorPanel(wxWindow* parent, CompNodeEditor& ceditor, 
	                                         const SubjectMgrPtr& sub_mgr, const GameObj& obj)
	: WxCompPanel(parent, "NodeEditor")
	, m_ceditor(ceditor)
	, m_sub_mgr(sub_mgr)
	, m_obj(obj)
{
	InitLayout();
	Expand();
}

void WxCompNodeEditorPanel::RefreshNodeComp()
{
	m_filepath->SetValue(m_ceditor.GetFilepath());
	m_name->SetValue(m_ceditor.GetName());
	m_visible->SetValue(m_ceditor.IsVisible());
	m_editable->SetValue(m_ceditor.IsEditable());
}

void WxCompNodeEditorPanel::InitLayout()
{
	wxWindow* win = GetPane();

	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);

	// filepath
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(m_filepath = new wxTextCtrl(win, wxID_ANY, m_ceditor.GetFilepath(),
			wxDefaultPosition, wxSize(200, 20), wxTE_READONLY));

		pane_sizer->Add(sizer);
	}
	// name
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Name ")));

		sizer->Add(m_name = new wxTextCtrl(win, wxID_ANY, m_ceditor.GetName(), 
			wxDefaultPosition, wxSize(-1, 20), wxTE_PROCESS_ENTER));
		Connect(m_name->GetId(), wxEVT_COMMAND_TEXT_ENTER,
			wxCommandEventHandler(WxCompNodeEditorPanel::EnterNameValue));
		pane_sizer->Add(sizer);
	}
	// visible, editable
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		{
			sizer->Add(m_visible = new wxCheckBox(win, wxID_ANY, "visible"));
			m_visible->SetValue(m_ceditor.IsVisible());
			Connect(m_visible->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED,
				wxCommandEventHandler(WxCompNodeEditorPanel::UpdateVisibleValue));
		}
		pane_sizer->AddSpacer(10);
		{
			sizer->Add(m_editable = new wxCheckBox(win, wxID_ANY, "editable"));
			m_editable->SetValue(m_ceditor.IsEditable());
			Connect(m_editable->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED,
				wxCommandEventHandler(WxCompNodeEditorPanel::UpdateEditableValue));
		}
		pane_sizer->Add(sizer);
	}
	
	win->SetSizer(pane_sizer);
	pane_sizer->SetSizeHints(win);
}

void WxCompNodeEditorPanel::EnterNameValue(wxCommandEvent& event)
{
	m_ceditor.SetName(m_name->GetValue().ToStdString());

	VariantSet vars;
	Variant var;
	var.m_type = VT_PVOID;
	var.m_val.pv = &m_obj;
	vars.SetVariant("obj", var);

	m_sub_mgr->NotifyObservers(ee0::MSG_UPDATE_NODE_NAME, vars);
}

void WxCompNodeEditorPanel::UpdateVisibleValue(wxCommandEvent& event)
{
	m_ceditor.SetVisible(event.IsChecked());
}

void WxCompNodeEditorPanel::UpdateEditableValue(wxCommandEvent& event)
{
	m_ceditor.SetEditable(event.IsChecked());
}

}