#include "ee0/WxNavigationBar.h"

namespace ee0
{

WxNavigationBar::WxNavigationBar(wxWindow* parent)
    : wxWindow(parent, wxID_ANY)
{
    InitLayout();
}

void WxNavigationBar::Push(const std::string& path)
{
    m_paths.push_back(path);

    auto sizer = GetSizer();
    auto btn = new wxButton(this, wxID_ANY, path);
    sizer->Add(btn);
}

void WxNavigationBar::Pop()
{
    if (m_paths.empty()) {
        return;
    }

    m_paths.pop_back();

    auto sizer = GetSizer();
    sizer->Remove(2 + m_paths.size());
}

void WxNavigationBar::MoveTo()
{
}

void WxNavigationBar::InitLayout()
{
    wxSizer* top_sizer = new wxBoxSizer(wxHORIZONTAL);
    {
        m_btn_prev = new wxButton(this, wxID_ANY, "<<", wxDefaultPosition, wxSize(20, 20));
        Connect(m_btn_prev->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(WxNavigationBar::OnPrevPress));
        top_sizer->Add(m_btn_prev, 0, wxLEFT | wxRIGHT, 5);

        m_btn_next = new wxButton(this, wxID_ANY, ">>", wxDefaultPosition, wxSize(20, 20));
        Connect(m_btn_next->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(WxNavigationBar::OnNextPress));
        top_sizer->Add(m_btn_next, 0, wxLEFT | wxRIGHT, 5);
    }
    SetSizer(top_sizer);
}

void WxNavigationBar::OnPrevPress(wxCommandEvent& event)
{

}

void WxNavigationBar::OnNextPress(wxCommandEvent& event)
{

}

}