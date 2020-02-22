#include "ee0/WxEditCodeProp.h"
#include "ee0/WxCodeEditDlg.h"

#include <cpputil/StringHelper.h>

namespace ee0
{

WX_PG_IMPLEMENT_PROPERTY_CLASS(WxEditCodeProp, wxLongStringProperty, wxSize, const wxSize&, TextCtrlAndButton)

WxEditCodeProp::WxEditCodeProp(const wxString& label, const wxString& name, const wxString& value)
	: wxLongStringProperty(label, name, value.ToStdString())
	, m_code_str(value)
{
    cpputil::StringHelper::ReplaceAll(m_code_str, "\\n", "\n");
    cpputil::StringHelper::ReplaceAll(m_code_str, "\\t", "\t");
}

bool WxEditCodeProp::OnButtonClick(wxPropertyGrid* propgrid, wxString& value)
{
    WxCodeEditDlg dlg(m_parent, m_code_str);
    dlg.SetCallback(m_cb);

	if (dlg.ShowModal() == wxID_OK)
	{
        m_code_str = dlg.GetCodeStr();
		SetValue(m_code_str);
		//if (m_cb) {
		//	m_cb(m_code_str);
		//}
		return true;
	}
	else
	{
		return false;
	}
}

}