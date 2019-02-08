#include "ee0/WxCompPanel.h"
#include "ee0/ReflectPropTypes.h"

#include <SM_Vector.h>
#include <painting0/Color.h>

#include <wx/sizer.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>

namespace
{

std::string FLAG_XYZW[4] = {
    "x",
    "y",
    "z",
    "w",
};

}

namespace ee0
{

BEGIN_EVENT_TABLE(WxCompPanel, wxCollapsiblePane)
	EVT_COLLAPSIBLEPANE_CHANGED(ID_NODE_COMP_PANEL, WxCompPanel::OnCollapsChanged)
END_EVENT_TABLE()

WxCompPanel::WxCompPanel(wxWindow* parent, const std::string& title)
	: wxCollapsiblePane(parent, ID_NODE_COMP_PANEL, title)
{
}

void WxCompPanel::RefreshNodeComp()
{
    for (auto& ctrl : m_controls)
    {
        auto type = ctrl.prop.get_type();
        if (type == rttr::type::get<bool>())
        {
            auto b = ctrl.prop.get_value(ctrl.obj).get_value<bool>();
            static_cast<wxCheckBox*>(ctrl.control)->SetValue(b);
        }
        else if (type == rttr::type::get<sm::bvec2>())
        {
            auto b2 = ctrl.prop.get_value(ctrl.obj).get_value<sm::bvec2>();
            if (ctrl.flag == FLAG_XYZW[0]) {
                static_cast<wxCheckBox*>(ctrl.control)->SetValue(b2.x);
            } else if (ctrl.flag == FLAG_XYZW[1]) {
                static_cast<wxCheckBox*>(ctrl.control)->SetValue(b2.y);
            }
        }
        else if (type == rttr::type::get<float>())
        {
            auto v = ctrl.prop.get_value(ctrl.obj).get_value<float>();
            static_cast<wxTextCtrl*>(ctrl.control)->SetValue(std::to_string(v));
        }
        else if (type == rttr::type::get<sm::vec2>())
        {
            auto v2 = ctrl.prop.get_value(ctrl.obj).get_value<sm::vec2>();
            if (ctrl.flag == FLAG_XYZW[0]) {
                static_cast<wxTextCtrl*>(ctrl.control)->SetValue(std::to_string(v2.x));
            } else if (ctrl.flag == FLAG_XYZW[1]) {
                static_cast<wxTextCtrl*>(ctrl.control)->SetValue(std::to_string(v2.y));
            }
        }
        else if (type == rttr::type::get<sm::vec3>())
        {
            auto v3 = ctrl.prop.get_value(ctrl.obj).get_value<sm::vec3>();
            if (ctrl.flag == FLAG_XYZW[0]) {
                static_cast<wxTextCtrl*>(ctrl.control)->SetValue(std::to_string(v3.x));
            } else if (ctrl.flag == FLAG_XYZW[1]) {
                static_cast<wxTextCtrl*>(ctrl.control)->SetValue(std::to_string(v3.y));
            } else if (ctrl.flag == FLAG_XYZW[2]) {
                static_cast<wxTextCtrl*>(ctrl.control)->SetValue(std::to_string(v3.z));
            }
        }
        else if (type == rttr::type::get<sm::vec4>())
        {
            auto v4 = ctrl.prop.get_value(ctrl.obj).get_value<sm::vec4>();
            if (ctrl.flag == FLAG_XYZW[0]) {
                static_cast<wxTextCtrl*>(ctrl.control)->SetValue(std::to_string(v4.x));
            } else if (ctrl.flag == FLAG_XYZW[1]) {
                static_cast<wxTextCtrl*>(ctrl.control)->SetValue(std::to_string(v4.y));
            } else if (ctrl.flag == FLAG_XYZW[2]) {
                static_cast<wxTextCtrl*>(ctrl.control)->SetValue(std::to_string(v4.z));
            } else if (ctrl.flag == FLAG_XYZW[4]) {
                static_cast<wxTextCtrl*>(ctrl.control)->SetValue(std::to_string(v4.w));
            }
        }
        else if (type == rttr::type::get<const char*>()
              || type == rttr::type::get<std::string>())
        {
            std::string str = (type == rttr::type::get<const char*>()) ?
                ctrl.prop.get_value(ctrl.obj).get_value<const char*>() :
                ctrl.prop.get_value(ctrl.obj).to_string();
            static_cast<wxTextCtrl*>(ctrl.control)->SetValue(str);
        }
        else if (type == rttr::type::get<pt0::Color>())
        {
            auto col = ctrl.prop.get_value(ctrl.obj).get_value<pt0::Color>();
            if (ctrl.flag == FLAG_XYZW[0]) {
                static_cast<wxTextCtrl*>(ctrl.control)->SetValue(std::to_string(col.r));
            } else if (ctrl.flag == FLAG_XYZW[1]) {
                static_cast<wxTextCtrl*>(ctrl.control)->SetValue(std::to_string(col.g));
            } else if (ctrl.flag == FLAG_XYZW[2]) {
                static_cast<wxTextCtrl*>(ctrl.control)->SetValue(std::to_string(col.b));
            } else if (ctrl.flag == FLAG_XYZW[3]) {
                static_cast<wxTextCtrl*>(ctrl.control)->SetValue(std::to_string(col.a));
            }
        }
        else
        {
            // unknown type
            assert(0);
        }
    }
}

void WxCompPanel::InitControl(wxSizer* top_sizer, const UIMetaInfo& info,
                              rttr::instance obj, rttr::property prop)
{
    static const wxSize INPUT_SIZE(65, 19);

    wxWindow* win = GetPane();
    auto type = prop.get_type();
    if (type == rttr::type::get<bool>())
    {
        auto b = prop.get_value(obj).get_value<bool>();
        auto ctrl = new wxCheckBox(win, wxID_ANY, info.desc);
        ctrl->SetValue(b);
        Connect(ctrl->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED,
            wxCommandEventHandler(WxCompPanel::SetCheckboxValue));
        top_sizer->Add(ctrl);
        m_controls.push_back({ "", obj, prop, ctrl });
    }
    else if (type == rttr::type::get<sm::bvec2>())
    {
        wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

        sizer->Add(new wxStaticText(win, wxID_ANY, info.desc));

        auto b = prop.get_value(obj).get_value<sm::bvec2>();
        {
            auto ctrl = new wxCheckBox(win, wxID_ANY, "X");
            ctrl->SetValue(b.x);
            Connect(ctrl->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED,
                wxCommandEventHandler(WxCompPanel::SetCheckboxValue));
            sizer->Add(ctrl);
            m_controls.push_back({ FLAG_XYZW[0], obj, prop, ctrl });
        }
        {
            auto ctrl = new wxCheckBox(win, wxID_ANY, "Y");
            ctrl->SetValue(b.y);
            Connect(ctrl->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED,
                wxCommandEventHandler(WxCompPanel::SetCheckboxValue));
            sizer->Add(ctrl);
            m_controls.push_back({ FLAG_XYZW[1], obj, prop, ctrl });
        }
        top_sizer->Add(sizer);
    }
    else if (type == rttr::type::get<float>())
    {
        auto f = prop.get_value(obj).get_value<float>();

        wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

        sizer->Add(new wxStaticText(win, wxID_ANY, info.desc));

        auto ctrl = new wxTextCtrl(win, wxID_ANY, std::to_string(f),
            wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER);
        Connect(ctrl->GetId(), wxEVT_COMMAND_TEXT_ENTER,
            wxCommandEventHandler(WxCompPanel::SetTextValue));
        sizer->Add(ctrl);
        m_controls.push_back({ "", obj, prop, ctrl });

        top_sizer->Add(sizer);
    }
    else if (type == rttr::type::get<sm::vec2>())
    {
        auto vec2 = prop.get_value(obj).get_value<sm::vec2>();

        wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

        sizer->Add(new wxStaticText(win, wxID_ANY, info.desc));
        for (int i = 0; i < 2; ++i)
        {
            auto ctrl = new wxTextCtrl(win, wxID_ANY, std::to_string(vec2.xy[i]),
                wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER);
            Connect(ctrl->GetId(), wxEVT_COMMAND_TEXT_ENTER,
                wxCommandEventHandler(WxCompPanel::SetTextValue));
            sizer->Add(ctrl);
            m_controls.push_back({ FLAG_XYZW[i], obj, prop, ctrl });
        }

        top_sizer->Add(sizer);
    }
    else if (type == rttr::type::get<sm::vec3>())
    {
        auto vec3 = prop.get_value(obj).get_value<sm::vec3>();

        wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

        sizer->Add(new wxStaticText(win, wxID_ANY, info.desc));
        for (int i = 0; i < 3; ++i)
        {
            auto ctrl = new wxTextCtrl(win, wxID_ANY, std::to_string(vec3.xyz[i]),
                wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER);
            Connect(ctrl->GetId(), wxEVT_COMMAND_TEXT_ENTER,
                wxCommandEventHandler(WxCompPanel::SetTextValue));
            sizer->Add(ctrl);
            m_controls.push_back({ FLAG_XYZW[i], obj, prop, ctrl });
        }

        top_sizer->Add(sizer);
    }
    else if (type == rttr::type::get<sm::vec4>())
    {
        auto vec4 = prop.get_value(obj).get_value<sm::vec4>();

        wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

        sizer->Add(new wxStaticText(win, wxID_ANY, info.desc));
        for (int i = 0; i < 4; ++i)
        {
            auto ctrl = new wxTextCtrl(win, wxID_ANY, std::to_string(vec4.xyzw[i]),
                wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER);
            Connect(ctrl->GetId(), wxEVT_COMMAND_TEXT_ENTER,
                wxCommandEventHandler(WxCompPanel::SetTextValue));
            sizer->Add(ctrl);
            m_controls.push_back({ FLAG_XYZW[i], obj, prop, ctrl });
        }

        top_sizer->Add(sizer);
    }
    else if (type == rttr::type::get<const char*>()
          || type == rttr::type::get<std::string>())
    {
        std::string str = (type == rttr::type::get<const char*>()) ?
			prop.get_value(obj).get_value<const char*>() :
			prop.get_value(obj).to_string();

        wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

        sizer->Add(new wxStaticText(win, wxID_ANY, info.desc));

        auto ctrl = new wxTextCtrl(win, wxID_ANY, str,
            wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER);
        Connect(ctrl->GetId(), wxEVT_COMMAND_TEXT_ENTER,
            wxCommandEventHandler(WxCompPanel::SetTextValue));
        sizer->Add(ctrl);
        m_controls.push_back({ "", obj, prop, ctrl });
    }
    else if (type == rttr::type::get<pt0::Color>())
    {
        auto col = prop.get_value(obj).get_value<pt0::Color>();

        wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

        sizer->Add(new wxStaticText(win, wxID_ANY, info.desc));

        auto r = new wxTextCtrl(win, wxID_ANY, std::to_string(col.r),
            wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER);
        Connect(r->GetId(), wxEVT_COMMAND_TEXT_ENTER,
            wxCommandEventHandler(WxCompPanel::SetTextValue));
        sizer->Add(r);
        m_controls.push_back({ FLAG_XYZW[0], obj, prop, r });

        auto g = new wxTextCtrl(win, wxID_ANY, std::to_string(col.g),
            wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER);
        Connect(g->GetId(), wxEVT_COMMAND_TEXT_ENTER,
            wxCommandEventHandler(WxCompPanel::SetTextValue));
        sizer->Add(g);
        m_controls.push_back({ FLAG_XYZW[1], obj, prop, g });

        auto b = new wxTextCtrl(win, wxID_ANY, std::to_string(col.b),
            wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER);
        Connect(b->GetId(), wxEVT_COMMAND_TEXT_ENTER,
            wxCommandEventHandler(WxCompPanel::SetTextValue));
        sizer->Add(b);
        m_controls.push_back({ FLAG_XYZW[2], obj, prop, b });

        auto a = new wxTextCtrl(win, wxID_ANY, std::to_string(col.a),
            wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER);
        Connect(a->GetId(), wxEVT_COMMAND_TEXT_ENTER,
            wxCommandEventHandler(WxCompPanel::SetTextValue));
        sizer->Add(a);
        m_controls.push_back({ FLAG_XYZW[3], obj, prop, a });

        top_sizer->Add(sizer);
    }
    else
    {
        // unknown type
        assert(0);
    }
}

void WxCompPanel::OnCollapsChanged(wxCollapsiblePaneEvent& event)
{
    m_parent->Layout();
}

void WxCompPanel::SetTextValue(wxCommandEvent& event)
{
    int id = event.GetId();
    for (auto& ctrl : m_controls)
    {
        if (ctrl.control->GetId() != id) {
            continue;
        }

        auto s_val = static_cast<wxTextCtrl*>(ctrl.control)->GetValue();

        auto type = ctrl.prop.get_type();
        if (type == rttr::type::get<float>())
        {
            ctrl.prop.set_value(ctrl.obj, static_cast<float>(std::atof(s_val)));
        }
        else if (type == rttr::type::get<sm::vec2>())
        {
            auto v = ctrl.prop.get_value(ctrl.obj).get_value<sm::vec2>();
            if (ctrl.flag == FLAG_XYZW[0]) {
                v.x = std::atof(s_val);
            } else if (ctrl.flag == FLAG_XYZW[1]) {
                v.y = std::atof(s_val);
            }
            ctrl.prop.set_value(ctrl.obj, v);
        }
        else if (type == rttr::type::get<sm::vec3>())
        {
            auto v = ctrl.prop.get_value(ctrl.obj).get_value<sm::vec3>();
            if (ctrl.flag == FLAG_XYZW[0]) {
                v.x = std::atof(s_val);
            } else if (ctrl.flag == FLAG_XYZW[1]) {
                v.y = std::atof(s_val);
            } else if (ctrl.flag == FLAG_XYZW[2]) {
                v.z = std::atof(s_val);
            }
            ctrl.prop.set_value(ctrl.obj, v);
        }
        else if (type == rttr::type::get<sm::vec4>())
        {
            auto v = ctrl.prop.get_value(ctrl.obj).get_value<sm::vec4>();
            if (ctrl.flag == FLAG_XYZW[0]) {
                v.x = std::atof(s_val);
            } else if (ctrl.flag == FLAG_XYZW[1]) {
                v.y = std::atof(s_val);
            } else if (ctrl.flag == FLAG_XYZW[2]) {
                v.z = std::atof(s_val);
            } else if (ctrl.flag == FLAG_XYZW[3]) {
                v.w = std::atof(s_val);
            }
            ctrl.prop.set_value(ctrl.obj, v);
        }
        else if (type == rttr::type::get<const char*>()
              || type == rttr::type::get<std::string>())
        {
            ctrl.prop.set_value(ctrl.obj, s_val.ToStdString());
        }
        else if (type == rttr::type::get<pt0::Color>())
        {
            auto col = ctrl.prop.get_value(ctrl.obj).get_value<pt0::Color>();
            if (ctrl.flag == FLAG_XYZW[0]) {
                col.r = std::atoi(s_val);
            } else if (ctrl.flag == FLAG_XYZW[1]) {
                col.g = std::atoi(s_val);
            } else if (ctrl.flag == FLAG_XYZW[2]) {
                col.b = std::atoi(s_val);
            } else if (ctrl.flag == FLAG_XYZW[3]) {
                col.a = std::atoi(s_val);
            }
            ctrl.prop.set_value(ctrl.obj, col);
        }
        else
        {
            // unknown type
            assert(0);
        }

        break;
    }
}

void WxCompPanel::SetCheckboxValue(wxCommandEvent& event)
{
    int id = event.GetId();
    for (auto& ctrl : m_controls)
    {
        if (ctrl.control->GetId() != id) {
            continue;
        }

        auto b_val = static_cast<wxCheckBox*>(ctrl.control)->GetValue();

        auto type = ctrl.prop.get_type();
        if (type == rttr::type::get<bool>())
        {
            ctrl.prop.set_value(ctrl.obj, b_val);
        }
        else if (type == rttr::type::get<sm::bvec2>())
        {
            auto b = ctrl.prop.get_value(ctrl.obj).get_value<sm::bvec2>();
            if (ctrl.flag == FLAG_XYZW[0]) {
                b.x = b_val;
            } else if (ctrl.flag == FLAG_XYZW[1]) {
                b.y = b_val;
            }
            ctrl.prop.set_value(ctrl.obj, b);
        }

        break;
    }
}

}