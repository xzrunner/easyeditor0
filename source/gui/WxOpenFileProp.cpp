#include "ee0/WxOpenFileProp.h"

#include <wx/filedlg.h>

#include <boost/filesystem.hpp>

namespace ee0
{

WX_PG_IMPLEMENT_PROPERTY_CLASS(WxOpenFileProp, wxLongStringProperty, wxSize, const wxSize&, TextCtrlAndButton)

WxOpenFileProp::WxOpenFileProp(const wxString& label, const wxString& name, const wxString& value)
	: wxLongStringProperty(label, name, value.ToStdString())
	, m_filepath(value)
{
}

bool WxOpenFileProp::OnButtonClick(wxPropertyGrid* propgrid, wxString& value)
{
	std::string dir = boost::filesystem::path(m_filepath).parent_path().string();
	wxFileDialog dlg(m_parent, "Select File", dir, m_filepath, m_filter);
	if (dlg.ShowModal() == wxID_OK)
	{
		m_filepath = dlg.GetPath();
		SetValue(m_filepath);
		if (m_cb) {
			m_cb(m_filepath.ToStdString());
		}
		return true;
	}
	else
	{
		return false;
	}
}

}