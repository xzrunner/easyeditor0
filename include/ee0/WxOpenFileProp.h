#pragma once

#include <wx/propgrid/propgrid.h>

namespace ee0
{

class WxOpenFileProp : public wxLongStringProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(WxOpenFileProp)

public:
	WxOpenFileProp(const wxString& label = wxPG_LABEL, const wxString& name = wxPG_LABEL,
		const wxString& value = wxEmptyString);

	virtual bool OnButtonClick(wxPropertyGrid* propgrid, wxString& value) override;

	void SetParent(wxWindow* parent) { m_parent = parent; }

	void SetFilter(const std::string& filter) { m_filter = filter; }

	void SetCallback(std::function<void(const std::string&)> cb) { m_cb = cb; }

private:
	wxWindow* m_parent = nullptr;

	std::string m_filter;

	std::function<void(const std::string&)> m_cb = nullptr;

	wxString m_filepath;

}; // WxOpenFileProp

}