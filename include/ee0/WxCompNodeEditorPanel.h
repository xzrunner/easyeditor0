#pragma once

#include "ee0/WxCompPanel.h"
#include "ee0/CompNodeEditor.h"

class wxTextCtrl;
class wxCheckBox;

namespace ee0 { class SubjectMgr; }

namespace ee0
{

class WxCompNodeEditorPanel : public WxCompPanel
{
public:
	WxCompNodeEditorPanel(wxWindow* parent, CompNodeEditor& ceditor,
		SubjectMgr& sub_mgr);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void UpdateNameValue(wxCommandEvent& event);
	void UpdateVisibleValue(wxCommandEvent& event);
	void UpdateEditableValue(wxCommandEvent& event);

private:
	CompNodeEditor&  m_ceditor;
	SubjectMgr& m_sub_mgr;

	wxTextCtrl* m_filepath;
	wxTextCtrl* m_name;
	wxCheckBox *m_visible, *m_editable;

}; // WxCompNodeEditorPanel

}