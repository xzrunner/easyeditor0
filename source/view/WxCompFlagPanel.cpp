#include "ee0/WxCompFlagPanel.h"
#include "ee0/SubjectMgr.h"
#include "ee0/MessageID.h"

#ifndef GAME_OBJ_ECS
#include <node0/SceneNode.h>
#include <node0/CompFlags.h>
#include <node0/NodeFlags.h>
#include <node0/NodeFlagsHelper.h>
#else
#include <entity0/World.h>
#endif // GAME_OBJ_ECS

#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/checkbox.h>

namespace ee0
{

WxCompFlagPanel::WxCompFlagPanel(wxWindow* parent, const SubjectMgrPtr& sub_mgr,
	                             ECS_WORLD_PARAM const GameObj& obj)
	: WxCompPanel(parent, "NodeEditor")
	, m_sub_mgr(sub_mgr)
	ECS_WORLD_SELF_ASSIGN
	, m_obj(obj)
{
	InitLayout();
	Expand();
}

void WxCompFlagPanel::RefreshNodeComp()
{
#ifndef GAME_OBJ_ECS
	if (m_obj->HasUniqueComp<n0::CompFlags>()) {
		auto& cflags = m_obj->GetUniqueComp<n0::CompFlags>();
		m_visible->SetValue(!cflags.GetFlag<n0::NodeNotVisible>());
		m_editable->SetValue(!cflags.GetFlag<n0::NodeNotEditable>());
	} else {
		m_visible->SetValue(true);
		m_editable->SetValue(true);
	}
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

void WxCompFlagPanel::InitLayout()
{
	wxWindow* win = GetPane();
	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);

	// visible, editable
	{
#ifndef GAME_OBJ_ECS
		bool visible = true;
		bool editable = true;
		if (m_obj->HasUniqueComp<n0::CompFlags>())
		{
			auto& cflags = m_obj->GetUniqueComp<n0::CompFlags>();
			visible = !cflags.GetFlag<n0::NodeNotVisible>();
			editable = !cflags.GetFlag<n0::NodeNotEditable>();
		}
#else
		auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(m_obj);
		bool visible = ceditor.visible;
		bool editable = ceditor.editable;
#endif // GAME_OBJ_ECS

		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		{
			sizer->Add(m_visible = new wxCheckBox(win, wxID_ANY, "visible"));
			m_visible->SetValue(visible);
			Connect(m_visible->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED,
				wxCommandEventHandler(WxCompFlagPanel::UpdateVisibleValue));
		}
		pane_sizer->AddSpacer(10);
		{
			sizer->Add(m_editable = new wxCheckBox(win, wxID_ANY, "editable"));
			m_editable->SetValue(editable);
			Connect(m_editable->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED,
				wxCommandEventHandler(WxCompFlagPanel::UpdateEditableValue));
		}
		pane_sizer->Add(sizer);
	}

	win->SetSizer(pane_sizer);
	pane_sizer->SetSizeHints(win);
}

void WxCompFlagPanel::UpdateVisibleValue(wxCommandEvent& event)
{
#ifndef GAME_OBJ_ECS
	n0::NodeFlagsHelper::SetFlag<n0::NodeNotVisible>(*m_obj, !event.IsChecked());
#else
	auto& cid = m_world.GetComponent<ee0::CompEntityEditor>(m_obj);
	cid.visible = event.IsChecked();
#endif // GAME_OBJ_ECS
}

void WxCompFlagPanel::UpdateEditableValue(wxCommandEvent& event)
{
#ifndef GAME_OBJ_ECS
	n0::NodeFlagsHelper::SetFlag<n0::NodeNotEditable>(*m_obj, !event.IsChecked());
#else
	auto& cid = m_world.GetComponent<ee0::CompEntityEditor>(m_obj);
	cid.editable = event.IsChecked();
#endif // GAME_OBJ_ECS
}

}