#pragma once

#include "ee0/GameObj.h"
#include "ee0/WxCompPanel.h"
#include "ee0/typedef.h"

ECS_WORLD_DECL
class wxTextCtrl;
class wxCheckBox;

namespace ee0
{

class WxCompFlagPanel : public WxCompPanel
{
public:
	WxCompFlagPanel(wxWindow* parent, const SubjectMgrPtr& sub_mgr,
		ECS_WORLD_PARAM const GameObj& obj);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void UpdateVisibleValue(wxCommandEvent& event);
	void UpdateEditableValue(wxCommandEvent& event);

private:
	SubjectMgrPtr      m_sub_mgr;
	ECS_WORLD_SELF_DEF
	GameObj            m_obj;

	wxCheckBox *m_visible, *m_editable;

}; // WxCompFlagPanel

}