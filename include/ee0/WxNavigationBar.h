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

    size_t GetPathDepth() const { return m_paths.size(); }

    void SetSeekCallback(std::function<void(size_t)> seek_cb) {
        m_seek_cb = seek_cb;
    }

private:
    void InitLayout();

    void OnPrevPress(wxCommandEvent& event);
    void OnNextPress(wxCommandEvent& event);

    void OnSeekPath(wxCommandEvent& event);

private:
    struct Path
    {
        Path(const std::string& name, wxButton* btn)
            : name(name), btn(btn)
        {
        }

        std::string name;
        wxButton*   btn = nullptr;
    };

private:
    wxButton* m_btn_prev = nullptr;
    wxButton* m_btn_next = nullptr;

    std::vector<Path> m_paths;

    std::function<void(size_t)> m_seek_cb = nullptr;

//    std::stack<std::string> m_prev_stack, m_next_stack;

}; // WxNavigationBar

}