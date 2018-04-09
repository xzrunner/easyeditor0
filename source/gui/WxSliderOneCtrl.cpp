#include "ee0/WxSliderOneCtrl.h"
#include "ee0/SliderItem.h"
#include "ee0/VariantSet.h"
#include "ee0/GuiCallback.h"

#include <guard/check.h>
#include <sx/StringHelper.h>

#include <wx/statbox.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

namespace ee0
{

WxSliderOneCtrl::WxSliderOneCtrl(wxPanel* parent, const char* title, const char* name,
	                             GuiCallback& cb, int key, const SliderItem& item, float scale_slider2text)
	: WxSliderCtrl(parent)
	, m_name(name)
	, m_cb(cb)
	, m_key(key)
	, m_scale_slider2text(scale_slider2text)
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, title);
	wxBoxSizer* top_sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(new wxStaticText(this, wxID_ANY, item.title));

	m_slider = new wxSlider(this, wxID_ANY, item.val, item.min, item.max,
		wxDefaultPosition, wxSize(200, -1));
	Connect(m_slider->GetId(), wxEVT_SCROLL_THUMBTRACK, 
		wxScrollEventHandler(WxSliderOneCtrl::OnSetValue));
	sizer->Add(m_slider);

	m_text = new wxTextCtrl(this, wxID_ANY, std::to_string(item.val), 
		wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER);
	Connect(m_text->GetId(), wxEVT_COMMAND_TEXT_ENTER, 
		wxCommandEventHandler(WxSliderOneCtrl::OnSetValue));
	sizer->Add(m_text);

	top_sizer->Add(sizer);

	SetSizer(top_sizer);
}

void WxSliderOneCtrl::LoadFromCB()
{
	VariantSet variants;
	m_cb.GetValue(m_key, variants);

	auto var = variants.GetVariant("var");
	GD_ASSERT(var.m_type == VT_FLOAT, "err val");
	float fval = var.m_val.flt;
	int ival = std::lround(fval / m_scale_slider2text);
	m_slider->SetValue(ival);
	m_text->SetValue(sx::StringHelper::ToString(fval));
}

void WxSliderOneCtrl::StoreToCB()
{
	OnSetValue(wxScrollEvent());
}

void WxSliderOneCtrl::OnSetValue(wxScrollEvent& event)
{
	int ival = m_slider->GetValue();
	float fval = ival * m_scale_slider2text;
	m_text->SetValue(sx::StringHelper::ToString(fval));

	VariantSet variants;
	Variant var;
	var.m_type = VT_FLOAT;
	var.m_val.flt = fval;
	variants.SetVariant("var", var);
	m_cb.SetValue(m_key, variants);
}

void WxSliderOneCtrl::OnSetValue(wxCommandEvent& event)
{
	double fval;
	m_text->GetValue().ToDouble(&fval);
	int ival = (int)(fval / m_scale_slider2text);
	m_slider->SetValue(ival);

	VariantSet variants;
	Variant var;
	var.m_type = VT_FLOAT;
	var.m_val.flt = fval;
	variants.SetVariant("var", var);
	m_cb.SetValue(m_key, variants);
}

}