#pragma once

#include "ee0/WxSliderCtrl.h"

class wxTextCtrl;

namespace ee0
{

class GuiCallback;
class SliderItem;

class WxSliderTwoCtrl : public WxSliderCtrl
{
public:
	WxSliderTwoCtrl(wxPanel* parent, const char* title, const char* name,
		GuiCallback& cb, int key, const SliderItem& item0, const SliderItem& item1,
		float scale_slider2text = 1);

	virtual void LoadFromCB() override;
	virtual void StoreToCB() override;

private:
	void OnSetValue(wxScrollEvent& event);
	void OnSetValue(wxCommandEvent& event);

private:
	struct Item
	{
		wxSlider*   slider;
		wxTextCtrl* text;
		std::string name;
	};

private:
	std::string m_name;

	GuiCallback& m_cb;
	int m_key;

	float m_scale_slider2text;

	Item m_item0, m_item1;

}; // WxSliderTwoCtrl

}