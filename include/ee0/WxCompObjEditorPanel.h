#pragma once

#include "ee0/GameObj.h"
#include "ee0/WxCompPanel.h"
#ifndef GAME_OBJ_ECS
#include "ee0/CompNodeEditor.h"
#else
#include "ee0/CompEntityEditor.h"
#endif // GAME_OBJ_ECS
#include "ee0/typedef.h"

ECS_WORLD_DECL
class wxTextCtrl;
class wxCheckBox;

namespace ee0
{

class WxCompObjEditorPanel : public WxCompPanel
{
public:
	WxCompObjEditorPanel(wxWindow* parent, const SubjectMgrPtr& sub_mgr, 
		ECS_WORLD_PARAM const GameObj& obj);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void EnterNameValue(wxCommandEvent& event);
	void UpdateVisibleValue(wxCommandEvent& event);
	void UpdateEditableValue(wxCommandEvent& event);

private:
	SubjectMgrPtr      m_sub_mgr;
	ECS_WORLD_SELF_DEF
	GameObj            m_obj;

	wxTextCtrl* m_filepath;
	wxTextCtrl* m_name;
	wxCheckBox *m_visible, *m_editable;

}; // WxCompObjEditorPanel

}