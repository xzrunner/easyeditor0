#include "ee0/WxSliderTwoCtrl.h"
#include "ee0/SliderItem.h"
#include "ee0/VariantSet.h"
#include "ee0/GuiCallback.h"

#include <guard/check.h>
#include <cpputil/StringHelper.h>

#include <wx/statbox.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

namespace ee0
{

WxSliderTwoCtrl::WxSliderTwoCtrl(wxPanel* parent, const char* title, const char* name,
	                             GuiCallback& cb, int key, const SliderItem& item0, 
	                             const SliderItem& item1, float scale_slider2text)
	: WxSliderCtrl(parent)
	, m_name(name)
	, m_cb(cb)
	, m_key(key)
	, m_scale_slider2text(scale_slider2text)
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, title);
	wxBoxSizer* top_sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, item0.title, wxDefaultPosition, wxSize(40, -1)));

		m_item0.slider = new wxSlider(this, wxID_ANY, item0.val, item0.min, item0.max, 
			wxDefaultPosition, wxSize(160, -1));
		Connect(m_item0.slider->GetId(), wxEVT_SCROLL_THUMBTRACK, 
			wxScrollEventHandler(WxSliderTwoCtrl::OnSetValue));
		sizer->Add(m_item0.slider);

		m_item0.text = new wxTextCtrl(this, wxID_ANY, std::to_string(item0.val), 
			wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER);
		Connect(m_item0.text->GetId(), wxEVT_COMMAND_TEXT_ENTER, 
			wxCommandEventHandler(WxSliderTwoCtrl::OnSetValue));
		sizer->Add(m_item0.text);

		m_item0.name = item0.name;

		top_sizer->Add(sizer);
	}
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, item1.title, wxDefaultPosition, wxSize(40, -1)));

		m_item1.slider = new wxSlider(this, wxID_ANY, item1.val, item1.min, item1.max, 
			wxDefaultPosition, wxSize(160, -1));
		Connect(m_item1.slider->GetId(), wxEVT_SCROLL_THUMBTRACK, 
			wxScrollEventHandler(WxSliderTwoCtrl::OnSetValue));
		sizer->Add(m_item1.slider);

		m_item1.text = new wxTextCtrl(this, wxID_ANY, std::to_string(item1.val), 
			wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER);
		Connect(m_item1.text->GetId(), wxEVT_COMMAND_TEXT_ENTER, 
			wxCommandEventHandler(WxSliderTwoCtrl::OnSetValue));
		sizer->Add(m_item1.text);

		m_item1.name = item1.name;

		top_sizer->Add(sizer);
	}
	SetSizer(top_sizer);
}

void WxSliderTwoCtrl::LoadFromCB()
{
	VariantSet variants;
	m_cb.GetValue(m_key, variants);

	auto var0 = variants.GetVariant("var0");
	GD_ASSERT(var0.m_type == VT_FLOAT, "err var");
	float fval0 = var0.m_val.flt;

	auto val1 = variants.GetVariant("var1");
	GD_ASSERT(val1.m_type == VT_FLOAT, "err var");
	float fval1 = val1.m_val.flt;

	int ival0 = (int)(fval0 / m_scale_slider2text),
		ival1 = (int)(fval1 / m_scale_slider2text);
	m_item0.slider->SetValue(ival0);
	m_item1.slider->SetValue(ival1);
	m_item0.text->SetValue(cpputil::StringHelper::ToString(fval0));
	m_item1.text->SetValue(cpputil::StringHelper::ToString(fval1));
}

void WxSliderTwoCtrl::StoreToCB()
{
	OnSetValue(wxScrollEvent());
}

void WxSliderTwoCtrl::OnSetValue(wxScrollEvent& event)
{
	int ival0 = m_item0.slider->GetValue(),
		ival1 = m_item1.slider->GetValue();
	float fval0 = ival0 * m_scale_slider2text,
		  fval1 = ival1 * m_scale_slider2text;
	m_item0.text->SetValue(cpputil::StringHelper::ToString(fval0));
	m_item1.text->SetValue(cpputil::StringHelper::ToString(fval1));

	VariantSet variants;

	Variant var0;
	var0.m_type = VT_FLOAT;
	var0.m_val.flt = fval0;
	variants.SetVariant("var0", var0);

	Variant var1;
	var1.m_type = VT_FLOAT;
	var1.m_val.flt = fval1;
	variants.SetVariant("var1", var1);

	m_cb.SetValue(m_key, variants);
}

void WxSliderTwoCtrl::OnSetValue(wxCommandEvent& event)
{
	double fval0;
	m_item0.text->GetValue().ToDouble(&fval0);
	double fval1;
	m_item1.text->GetValue().ToDouble(&fval1);

	int ival0 = (int)(fval0 / m_scale_slider2text),
		ival1 = (int)(fval1 / m_scale_slider2text);

	m_item0.slider->SetValue(ival0);
	m_item1.slider->SetValue(ival1);

	VariantSet variants;

	Variant var0;
	var0.m_type = VT_FLOAT;
	var0.m_val.flt = fval0;
	variants.SetVariant("var0", var0);

	Variant var1;
	var1.m_type = VT_FLOAT;
	var1.m_val.flt = fval1;
	variants.SetVariant("var1", var1);

	m_cb.SetValue(m_key, variants);
}

}