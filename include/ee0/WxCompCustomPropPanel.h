#pragma once

#include "ee0/WxCompPanel.h"

class wxPropertyGrid;
class wxPropertyGridEvent;

namespace ee0
{

class CompCustomProp;

class WxCompCustomPropPanel : public WxCompPanel
{
public:
	WxCompCustomPropPanel(wxWindow* parent,
		CompCustomProp& cprop);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();
	void InitHeader(wxWindow* win, wxSizer* pane_sizer);
	void InitPropertyGrid(wxWindow* win, wxSizer* pane_sizer);
	void InitProperties();

	void OnPropertyGridChange(wxPropertyGridEvent& event);

	void OnAddPress(wxCommandEvent& event);

private:
	CompCustomProp& m_cprop;

	wxPropertyGrid* m_pg;

	wxButton* m_add_btn;

}; // WxCompCustomPropPanel

}