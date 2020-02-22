#pragma once

#include <wx/dialog.h>

namespace ee0
{

class WxCodeCtrl;

class WxCodeEditDlg : public wxDialog
{
public:
    WxCodeEditDlg(wxWindow* parent, const std::string& code_str);

    void SetCallback(std::function<void(const std::string&)> cb) { m_cb = cb; }

    std::string GetCodeStr() const;

private:
    void InitLayout();

    void OnSize(wxSizeEvent& event) {}
    void OnRunPress(wxCommandEvent& event);

private:
    WxCodeCtrl* m_code;

    std::function<void(const std::string&)> m_cb = nullptr;

    DECLARE_EVENT_TABLE()

}; // WxCodeEditDlg

}