#pragma once

#include "ee0/WxSliderCtrl.h"

class wxTextCtrl;

namespace ee0
{

class GuiCallback;
class SliderItem;

class WxSliderOneCtrl : public WxSliderCtrl
{
public:
	WxSliderOneCtrl(wxPanel* parent, const char* title, const char* name,
		GuiCallback& cb, int key, const SliderItem& item, float scale_slider2text = 1);

	virtual void LoadFromCB() override;
	virtual void StoreToCB() override;

private:
	void OnSetValue(wxScrollEvent& event);
	void OnSetValue(wxCommandEvent& event);

private:
	std::string m_name;

	GuiCallback& m_cb;
	int m_key;

	float m_scale_slider2text;

	wxSlider*   m_slider;
	wxTextCtrl* m_text;

}; // WxSliderOneCtrl

}