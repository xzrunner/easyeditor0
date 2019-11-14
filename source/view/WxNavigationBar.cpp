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
    auto sizer = GetSizer();
    auto btn = new wxButton(this, wxID_ANY, path);
    Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(WxNavigationBar::OnSeekPath));
    sizer->Add(btn);

    sizer->Fit(this);

    m_paths.push_back(Path(path, btn));
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

void WxNavigationBar::OnSeekPath(wxCommandEvent& event)
{
    int id = event.GetId();
    for (int i = 0, n = m_paths.size(); i < n; ++i)
    {
        if (m_paths[i].btn->GetId() != id) {
            continue;
        }

        if (i == m_paths.size() - 1) {
            break;
        }

        auto sizer = GetSizer();
        for (int j = i + 1; j < n; ++j) {
            sizer->Detach(m_paths[j].btn);
            delete m_paths[j].btn;
        }
        sizer->Fit(this);

        m_paths.erase(m_paths.begin() + i + 1, m_paths.end());

        if (m_seek_cb) {
            m_seek_cb(i);
        }

        break;
    }
}

}