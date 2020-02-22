#pragma once

#include <wx/propgrid/propgrid.h>

namespace ee0
{

class WxEditCodeProp : public wxLongStringProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(WxEditCodeProp)

public:
	WxEditCodeProp(const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL,
		const wxString& value = wxEmptyString);

	virtual bool OnButtonClick(wxPropertyGrid* propgrid, wxString& value) override;

	void SetParent(wxWindow* parent) { m_parent = parent; }

	void SetCallback(std::function<void(const std::string&)> cb) { m_cb = cb; }

private:
	wxWindow* m_parent = nullptr;

	std::function<void(const std::string&)> m_cb = nullptr;

	std::string m_code_str;

}; // WxEditCodeProp

}