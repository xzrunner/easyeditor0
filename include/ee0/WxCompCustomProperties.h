#pragma once

#include "ee0/WxCompPanel.h"

class wxPropertyGrid;
class wxPropertyGridEvent;

namespace ee0
{

class CompCustomProperties;

class WxCompCustomProperties : public WxCompPanel
{
public:
	WxCompCustomProperties(wxWindow* parent, 
		CompCustomProperties& cprop);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();
	void InitHeader(wxWindow* win, wxSizer* pane_sizer);
	void InitPropertyGrid(wxWindow* win, wxSizer* pane_sizer);
	void InitProperties();

	void OnPropertyGridChange(wxPropertyGridEvent& event);

	void OnAddPress(wxCommandEvent& event);

private:
	CompCustomProperties& m_cprop;

	wxPropertyGrid* m_pg;

	wxButton* m_add_btn;

}; // WxCompCustomProperties

}