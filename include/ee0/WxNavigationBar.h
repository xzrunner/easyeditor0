#pragma once

#include <wx/wx.h>

//#include <stack>
#include <vector>

namespace ee0
{

class WxNavigationBar : public wxWindow
{
public:
    WxNavigationBar(wxWindow* parent);

    void Push(const std::string& path);
    void Pop();

    void MoveTo();

private:
    void InitLayout();

    void OnPrevPress(wxCommandEvent& event);
    void OnNextPress(wxCommandEvent& event);

private:
    wxButton* m_btn_prev = nullptr;
    wxButton* m_btn_next = nullptr;

    std::vector<std::string> m_paths;

//    std::stack<std::string> m_prev_stack, m_next_stack;

}; // WxNavigationBar

}