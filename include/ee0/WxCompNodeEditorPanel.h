#pragma once

#include "ee0/WxCompPanel.h"
#include "ee0/CompNodeEditor.h"
#include "ee0/typedef.h"

#include <node0/typedef.h>

class wxTextCtrl;
class wxCheckBox;

namespace ee0
{

class WxCompNodeEditorPanel : public WxCompPanel
{
public:
	WxCompNodeEditorPanel(wxWindow* parent, CompNodeEditor& ceditor,
		const SubjectMgrPtr& sub_mgr, const n0::SceneNodePtr& node);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void EnterNameValue(wxCommandEvent& event);
	void UpdateVisibleValue(wxCommandEvent& event);
	void UpdateEditableValue(wxCommandEvent& event);

private:
	CompNodeEditor&  m_ceditor;
	SubjectMgrPtr    m_sub_mgr;
	n0::SceneNodePtr m_node;

	wxTextCtrl* m_filepath;
	wxTextCtrl* m_name;
	wxCheckBox *m_visible, *m_editable;

}; // WxCompNodeEditorPanel

}