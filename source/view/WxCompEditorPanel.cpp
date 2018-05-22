#include "ee0/WxCompEditorPanel.h"
#include "ee0/SubjectMgr.h"
#include "ee0/MessageID.h"
#include "ee0/CompNodeEditor.h"

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

WxCompEditorPanel::WxCompEditorPanel(wxWindow* parent, const SubjectMgrPtr& sub_mgr,
	                                       ECS_WORLD_PARAM const GameObj& obj)
	: WxCompPanel(parent, "NodeEditor")
	, m_sub_mgr(sub_mgr)
	ECS_WORLD_SELF_ASSIGN
	, m_obj(obj)
{
	InitLayout();
	Expand();
}

void WxCompEditorPanel::RefreshNodeComp()
{
#ifndef GAME_OBJ_ECS
	auto& ceditor = m_obj->GetUniqueComp<CompNodeEditor>();
	m_visible->SetValue(ceditor.IsVisible());
	m_editable->SetValue(ceditor.IsEditable());
#else
	auto& cid = m_world.GetComponent<ee0::CompEntityEditor>(m_obj);

	m_filepath->SetValue(*cid.filepath);
	if (cid.name) {
		m_name->SetValue(*cid.name);
	}
	m_visible->SetValue(cid.visible);
	m_editable->SetValue(cid.editable);
#endif // GAME_OBJ_ECS
}

void WxCompEditorPanel::InitLayout()
{
	wxWindow* win = GetPane();
	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);

	// visible, editable
	{
#ifndef GAME_OBJ_ECS
		auto& ceditor = m_obj->GetUniqueComp<CompNodeEditor>();
#else
		auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(m_obj);
#endif // GAME_OBJ_ECS

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
				wxCommandEventHandler(WxCompEditorPanel::UpdateVisibleValue));
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
				wxCommandEventHandler(WxCompEditorPanel::UpdateEditableValue));
		}
		pane_sizer->Add(sizer);
	}

	win->SetSizer(pane_sizer);
	pane_sizer->SetSizeHints(win);
}

void WxCompEditorPanel::UpdateVisibleValue(wxCommandEvent& event)
{
#ifndef GAME_OBJ_ECS
	auto& ceditor = m_obj->GetUniqueComp<CompNodeEditor>();
	ceditor.SetVisible(event.IsChecked());
#else
	auto& cid = m_world.GetComponent<ee0::CompEntityEditor>(m_obj);
	cid.visible = event.IsChecked();
#endif // GAME_OBJ_ECS
}

void WxCompEditorPanel::UpdateEditableValue(wxCommandEvent& event)
{
#ifndef GAME_OBJ_ECS
	auto& ceditor = m_obj->GetUniqueComp<CompNodeEditor>();
	ceditor.SetEditable(event.IsChecked());
#else
	auto& cid = m_world.GetComponent<ee0::CompEntityEditor>(m_obj);
	cid.editable = event.IsChecked();
#endif // GAME_OBJ_ECS
}

}