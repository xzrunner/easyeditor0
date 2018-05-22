#include "ee0/WxCompIdPanel.h"
#include "ee0/SubjectMgr.h"
#include "ee0/MessageID.h"

#ifndef GAME_OBJ_ECS
#include <node0/SceneNode.h>
#include <node0/CompIdentity.h>
#else
#endif // GAME_OBJ_ECS

#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/checkbox.h>

namespace ee0
{

WxCompIdPanel::WxCompIdPanel(wxWindow* parent, const SubjectMgrPtr& sub_mgr,
	                         ECS_WORLD_PARAM const GameObj& obj)
	: WxCompPanel(parent, "NodeID")
	, m_sub_mgr(sub_mgr)
	ECS_WORLD_SELF_ASSIGN
	, m_obj(obj)
{
	InitLayout();
	Expand();
}

void WxCompIdPanel::RefreshNodeComp()
{
#ifndef GAME_OBJ_ECS
	auto& cid = m_obj->GetUniqueComp<n0::CompIdentity>();
	m_filepath->SetValue(cid.GetFilepath());
	m_name->SetValue(cid.GetName());
#else
	auto& cid = m_world.GetComponent<ee0::CompEntityEditor>(m_obj);

	m_filepath->SetValue(*cid.filepath);
	if (cid.name) {
		m_name->SetValue(*cid.name);
	}
#endif // GAME_OBJ_ECS
}

void WxCompIdPanel::InitLayout()
{
	wxWindow* win = GetPane();
	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);

#ifndef GAME_OBJ_ECS
	auto& cid = m_obj->GetUniqueComp<n0::CompIdentity>();
#else
	auto& cid = m_world.GetComponent<ee0::CompEntityEditor>(m_obj);
#endif // GAME_OBJ_ECS

	// filepath
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

#ifndef GAME_OBJ_ECS
		auto& filepath = cid.GetFilepath();
#else
		std::string filepath;
		if (cid.filepath) {
			filepath = *cid.filepath;
		}
#endif // GAME_OBJ_ECS
		sizer->Add(m_filepath = new wxTextCtrl(win, wxID_ANY, filepath,
			wxDefaultPosition, wxSize(200, 20), wxTE_READONLY));

		pane_sizer->Add(sizer);
	}
	// name & id
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Name ")));

#ifndef GAME_OBJ_ECS
		auto& name = cid.GetName();
#else
		std::string name;
		if (cid.name) {
			name = *cid.name;
		}
#endif // GAME_OBJ_ECS
		sizer->Add(m_name = new wxTextCtrl(win, wxID_ANY, name,
			wxDefaultPosition, wxSize(80, 20), wxTE_PROCESS_ENTER));
		Connect(m_name->GetId(), wxEVT_COMMAND_TEXT_ENTER,
			wxCommandEventHandler(WxCompIdPanel::EnterNameValue));

#ifndef GAME_OBJ_ECS
		sizer->AddSpacer(10);
		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("ID ")));
		sizer->Add(new wxTextCtrl(win, wxID_ANY, std::to_string(cid.GetID()),
			wxDefaultPosition, wxSize(50, 20), wxTE_READONLY));
#endif // GAME_OBJ_ECS

		pane_sizer->Add(sizer);
	}

	win->SetSizer(pane_sizer);
	pane_sizer->SetSizeHints(win);
}

void WxCompIdPanel::EnterNameValue(wxCommandEvent& event)
{
#ifndef GAME_OBJ_ECS
	auto& cid = m_obj->GetUniqueComp<n0::CompIdentity>();
	cid.SetName(m_name->GetValue().ToStdString());
#else
	auto& cid = m_world.GetComponent<ee0::CompEntityEditor>(m_obj);
	cid.name = std::make_unique<std::string>(m_name->GetValue().ToStdString());
#endif // GAME_OBJ_ECS

	VariantSet vars;
	Variant var;
	var.m_type = VT_PVOID;
	var.m_val.pv = &m_obj;
	vars.SetVariant("obj", var);

	m_sub_mgr->NotifyObservers(ee0::MSG_UPDATE_NODE_NAME, vars);
}

}