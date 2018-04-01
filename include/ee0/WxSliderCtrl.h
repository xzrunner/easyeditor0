#pragma once

#include <wx/panel.h>

namespace ee0
{

class WxSliderCtrl : public wxPanel
{
public:
	WxSliderCtrl(wxPanel* parent) : wxPanel(parent, wxID_ANY) {}
	virtual ~WxSliderCtrl() {}
	virtual void LoadFromCB() = 0;
	virtual void StoreToCB() = 0;

}; // WxSliderCtrl

}