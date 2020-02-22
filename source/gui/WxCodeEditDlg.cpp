#include "ee0/WxCodeEditDlg.h"
#include "ee0/WxCodeCtrl.h"

#include <wx/sizer.h>
#include <wx/button.h>

namespace ee0
{

BEGIN_EVENT_TABLE(WxCodeEditDlg, wxDialog)
	EVT_MENU(wxID_SAVE, WxCodeEditDlg::OnRunPress)
END_EVENT_TABLE()

WxCodeEditDlg::WxCodeEditDlg(wxWindow* parent, const std::string& code_str)
    : wxDialog(parent, wxID_ANY, "Code Editor", wxDefaultPosition, wxSize(1024, 768))
{
    InitLayout();

    m_code->SetText(code_str);

    wxAcceleratorEntry entries[1];
    entries[0].Set(wxACCEL_CTRL, (int)'S', wxID_SAVE);
    wxAcceleratorTable acc_tab(1, entries);
    SetAcceleratorTable(acc_tab);
}

std::string WxCodeEditDlg::GetCodeStr() const
{
    return m_code->GetText().ToStdString();
}

void WxCodeEditDlg::InitLayout()
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    sizer->Add(m_code = new ee0::WxCodeCtrl(this, "input"), 3, wxEXPAND);

    wxBoxSizer* btn_sizer = new wxBoxSizer(wxHORIZONTAL);

    auto run_btn = new wxButton(this, wxID_ANY, "Run");
    Connect(run_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(WxCodeEditDlg::OnRunPress));
    btn_sizer->Add(run_btn, wxALL, 5);
    btn_sizer->AddSpacer(50);

    btn_sizer->Add(new wxButton(this, wxID_OK), wxALL, 5);
    btn_sizer->Add(new wxButton(this, wxID_CANCEL), wxALL, 5);
    sizer->Add(btn_sizer, 0, wxCENTER);

    SetSizer(sizer);
}

void WxCodeEditDlg::OnRunPress(wxCommandEvent& event)
{
    if (m_cb) {
        m_cb(m_code->GetText().ToStdString());
    }
}

}