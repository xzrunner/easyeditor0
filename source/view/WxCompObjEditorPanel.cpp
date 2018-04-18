#include "ee0/WxCompObjEditorPanel.h"
#include "ee0/SubjectMgr.h"
#include "ee0/MessageID.h"

#ifndef GAME_OBJ_ECS
#include <node0/SceneNode.h>
#else
#include <entity0/World.h>
#endif // GAME_OBJ_ECS

#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/checkbox.h>

namespace ee0
{

WxCompObjEditorPanel::WxCompObjEditorPanel(wxWindow* parent, const SubjectMgrPtr& sub_mgr, 
	                                       ECS_WORLD_PARAM const GameObj& obj)
	: WxCompPanel(parent, "NodeEditor")
	, m_sub_mgr(sub_mgr)
	ECS_WORLD_SELF_ASSIGN
	, m_obj(obj)
{
	InitLayout();
	Expand();
}

void WxCompObjEditorPanel::RefreshNodeComp()
{
#ifndef GAME_OBJ_ECS
	auto& ceditor = m_obj->GetUniqueComp<ee0::CompNodeEditor>();
	m_filepath->SetValue(ceditor.GetFilepath());
	m_name->SetValue(ceditor.GetName());
	m_visible->SetValue(ceditor.IsVisible());
	m_editable->SetValue(ceditor.IsEditable());
#else
	auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(m_obj);

	m_filepath->SetValue(*ceditor.filepath);
	if (ceditor.name) {
		m_name->SetValue(*ceditor.name);
	}
	m_visible->SetValue(ceditor.visible);
	m_editable->SetValue(ceditor.editable);
#endif // GAME_OBJ_ECS
}

void WxCompObjEditorPanel::InitLayout()
{
	wxWindow* win = GetPane();
	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);

#ifndef GAME_OBJ_ECS
	auto& ceditor = m_obj->GetUniqueComp<ee0::CompNodeEditor>();
#else
	auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(m_obj);
#endif // GAME_OBJ_ECS

	// filepath
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

#ifndef GAME_OBJ_ECS
		auto& filepath = ceditor.GetFilepath();
#else
		std::string filepath;
		if (ceditor.filepath) {
			filepath = *ceditor.filepath;
		}
#endif // GAME_OBJ_ECS
		sizer->Add(m_filepath = new wxTextCtrl(win, wxID_ANY, filepath,
			wxDefaultPosition, wxSize(200, 20), wxTE_READONLY));

		pane_sizer->Add(sizer);
	}
	// name
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Name ")));

#ifndef GAME_OBJ_ECS
		auto& name = ceditor.GetName();
#else
		std::string name;
		if (ceditor.name) {
			name = *ceditor.name;
		}
#endif // GAME_OBJ_ECS
		sizer->Add(m_name = new wxTextCtrl(win, wxID_ANY, name, 
			wxDefaultPosition, wxSize(-1, 20), wxTE_PROCESS_ENTER));
		Connect(m_name->GetId(), wxEVT_COMMAND_TEXT_ENTER,
			wxCommandEventHandler(WxCompObjEditorPanel::EnterNameValue));
		pane_sizer->Add(sizer);
	}
	// visible, editable
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		{
			sizer->Add(m_visible = new wxCheckBox(win, wxID_ANY, "visible"));
#ifndef GAME_OBJ_ECS
			auto visible = ceditor.IsVisible();
#else
			auto visible = ceditor.visible;
#endif // GAME_OBJ_ECS
			m_visible->SetValue(visible);
			Connect(m_visible->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED,
				wxCommandEventHandler(WxCompObjEditorPanel::UpdateVisibleValue));
		}
		pane_sizer->AddSpacer(10);
		{
			sizer->Add(m_editable = new wxCheckBox(win, wxID_ANY, "editable"));
#ifndef GAME_OBJ_ECS
			auto editable = ceditor.IsEditable();
#else
			auto editable = ceditor.editable;
#endif // GAME_OBJ_ECS
			m_editable->SetValue(editable);
			Connect(m_editable->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED,
				wxCommandEventHandler(WxCompObjEditorPanel::UpdateEditableValue));
		}
		pane_sizer->Add(sizer);
	}
	
	win->SetSizer(pane_sizer);
	pane_sizer->SetSizeHints(win);
}

void WxCompObjEditorPanel::EnterNameValue(wxCommandEvent& event)
{
#ifndef GAME_OBJ_ECS
	auto& ceditor = m_obj->GetUniqueComp<ee0::CompNodeEditor>();
	ceditor.SetName(m_name->GetValue().ToStdString());
#else
	auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(m_obj);
	ceditor.name = std::make_unique<std::string>(m_name->GetValue().ToStdString());
#endif // GAME_OBJ_ECS

	VariantSet vars;
	Variant var;
	var.m_type = VT_PVOID;
	var.m_val.pv = &m_obj;
	vars.SetVariant("obj", var);

	m_sub_mgr->NotifyObservers(ee0::MSG_UPDATE_NODE_NAME, vars);
}

void WxCompObjEditorPanel::UpdateVisibleValue(wxCommandEvent& event)
{
#ifndef GAME_OBJ_ECS
	auto& ceditor = m_obj->GetUniqueComp<ee0::CompNodeEditor>();
	ceditor.SetVisible(event.IsChecked());
#else
	auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(m_obj);
	ceditor.visible = event.IsChecked();
#endif // GAME_OBJ_ECS
}

void WxCompObjEditorPanel::UpdateEditableValue(wxCommandEvent& event)
{
#ifndef GAME_OBJ_ECS
	auto& ceditor = m_obj->GetUniqueComp<ee0::CompNodeEditor>();
	ceditor.SetEditable(event.IsChecked());
#else
	auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(m_obj);
	ceditor.editable = event.IsChecked();
#endif // GAME_OBJ_ECS
}

}