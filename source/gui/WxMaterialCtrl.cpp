#include "ee0/WxMaterialCtrl.h"

#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>

#include <painting0/Material.h>

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

namespace ee0
{

WxMaterialCtrl::WxMaterialCtrl(wxWindow* parent, const ee0::SubjectMgrPtr& sub_mgr, pt0::Material& mat)
    : wxWindow(parent, wxID_ANY)
    , m_sub_mgr(sub_mgr)
    , m_mat(mat)
{
    InitLayout();
}

void WxMaterialCtrl::RefreshView()
{
    auto& vars = m_mat.GetAllVars();
    assert(vars.size() == m_ctrls.size());
    auto itr = vars.begin();
    for (size_t i = 0, n = vars.size(); i < n; ++i, ++itr)
    {
        auto& name = itr->first;
        auto& val = itr->second;
        switch (val.type)
        {
        case pt0::RenderVarType::BOOL:
            assert(m_ctrls[i].size() == 1);
            static_cast<wxTextCtrl*>(m_ctrls[i][0])
                ->SetValue(std::to_string(val.b));
            break;
        case pt0::RenderVarType::INT:
            assert(m_ctrls[i].size() == 1);
            static_cast<wxTextCtrl*>(m_ctrls[i][0])
                ->SetValue(std::to_string(val.i));
            break;
        case pt0::RenderVarType::FLOAT:
            assert(m_ctrls[i].size() == 1);
            static_cast<wxTextCtrl*>(m_ctrls[i][0])
                ->SetValue(std::to_string(val.f));
            break;
        case pt0::RenderVarType::SAMPLER1D:
            break;
        case pt0::RenderVarType::SAMPLER2D:
            break;
        case pt0::RenderVarType::SAMPLER3D:
            break;
        case pt0::RenderVarType::SAMPLERCUBE:
            break;
        case pt0::RenderVarType::VEC2:
            assert(m_ctrls[i].size() == 2);
            for (int j = 0; j < 2; ++j) {
                static_cast<wxTextCtrl*>(m_ctrls[i][j])
                    ->SetValue(std::to_string(val.vec2.xy[j]));
            }
            break;
        case pt0::RenderVarType::VEC3:
            assert(m_ctrls[i].size() == 3);
            for (int j = 0; j < 3; ++j) {
                static_cast<wxTextCtrl*>(m_ctrls[i][j])
                    ->SetValue(std::to_string(val.vec3.xyz[j]));
            }
            break;
        case pt0::RenderVarType::VEC4:
            assert(m_ctrls[i].size() == 4);
            for (int j = 0; j < 4; ++j) {
                static_cast<wxTextCtrl*>(m_ctrls[i][j])
                    ->SetValue(std::to_string(val.vec4.xyzw[j]));
            }
            break;
        case pt0::RenderVarType::MAT2:
            break;
        case pt0::RenderVarType::MAT3:
            break;
        case pt0::RenderVarType::MAT4:
            break;
        }
    }
}

void WxMaterialCtrl::InitLayout()
{
    wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);

    static const wxSize INPUT_SIZE(65, 19);

    auto& vars = m_mat.GetAllVars();
    for (auto& v : vars)
    {
        auto& name = v.first;
        auto& val  = v.second;

        wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

        sizer->Add(new wxStaticText(this, wxID_ANY, name));

        std::vector<wxControl*> ctrls;

        switch (val.type)
        {
        case pt0::RenderVarType::BOOL:
        {
            auto ctrl = new wxTextCtrl(this, wxID_ANY, std::to_string(val.b),
                wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER);
            ctrls.push_back(ctrl);
            sizer->Add(ctrl);
        }
            break;
        case pt0::RenderVarType::INT:
        {
            auto ctrl = new wxTextCtrl(this, wxID_ANY, std::to_string(val.i),
                wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER);
            ctrls.push_back(ctrl);
            sizer->Add(ctrl);
        }
            break;
        case pt0::RenderVarType::FLOAT:
        {
            auto ctrl = new wxTextCtrl(this, wxID_ANY, std::to_string(val.f),
                wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER);
            ctrls.push_back(ctrl);
            sizer->Add(ctrl);
        }
            break;
        case pt0::RenderVarType::SAMPLER1D:
            break;
        case pt0::RenderVarType::SAMPLER2D:
            break;
        case pt0::RenderVarType::SAMPLER3D:
            break;
        case pt0::RenderVarType::SAMPLERCUBE:
            break;
        case pt0::RenderVarType::VEC2:
            for (int i = 0; i < 2; ++i)
            {
                auto ctrl = new wxTextCtrl(this, wxID_ANY, std::to_string(val.vec2.xy[i]),
                    wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER);
                ctrls.push_back(ctrl);
                sizer->Add(ctrl);
            }
            break;
        case pt0::RenderVarType::VEC3:
            for (int i = 0; i < 3; ++i)
            {
                auto ctrl = new wxTextCtrl(this, wxID_ANY, std::to_string(val.vec3.xyz[i]),
                    wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER);
                ctrls.push_back(ctrl);
                sizer->Add(ctrl);
            }
            break;
        case pt0::RenderVarType::VEC4:
            for (int i = 0; i < 3; ++i)
            {
                auto ctrl = new wxTextCtrl(this, wxID_ANY, std::to_string(val.vec4.xyzw[i]),
                    wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER);
                ctrls.push_back(ctrl);
                sizer->Add(ctrl);
            }
            break;
        case pt0::RenderVarType::MAT2:
            break;
        case pt0::RenderVarType::MAT3:
            break;
        case pt0::RenderVarType::MAT4:
            break;
        }

        m_ctrls.emplace_back(ctrls);

        pane_sizer->Add(sizer);
    }

    for (auto& i : m_ctrls) {
        for (auto& j : i) {
            Connect(j->GetId(), wxEVT_COMMAND_TEXT_ENTER,
                wxCommandEventHandler(WxMaterialCtrl::EnterTextValue));
        }
    }

    SetSizer(pane_sizer);
    pane_sizer->SetSizeHints(this);
}

void WxMaterialCtrl::EnterTextValue(wxCommandEvent& event)
{
    auto& vars = m_mat.GetAllVars();
    assert(vars.size() == m_ctrls.size());
    auto itr = vars.begin();
    int id = event.GetId();
    for (int i = 0, n = m_ctrls.size(); i < n; ++i, ++itr) {
        for (int j = 0, m = m_ctrls[i].size(); j < m; ++j) {
            if (m_ctrls[i][j]->GetId() == id)
            {
                double v;
                static_cast<wxTextCtrl*>(m_ctrls[i][j])->GetValue().ToDouble(&v);

                auto& name = itr->first;
                auto& val = itr->second;
                switch (val.type)
                {
                case pt0::RenderVarType::BOOL:
                    assert(m == 1);
                    m_mat.SetVar(name, pt0::RenderVariant(v == 0 ? false : true));
                    break;
                case pt0::RenderVarType::INT:
                    assert(m == 1);
                    m_mat.SetVar(name, pt0::RenderVariant(static_cast<int>(v)));
                    break;
                case pt0::RenderVarType::FLOAT:
                    assert(m == 1);
                    m_mat.SetVar(name, pt0::RenderVariant(static_cast<float>(v)));
                    break;
                case pt0::RenderVarType::SAMPLER1D:
                    break;
                case pt0::RenderVarType::SAMPLER2D:
                    break;
                case pt0::RenderVarType::SAMPLER3D:
                    break;
                case pt0::RenderVarType::SAMPLERCUBE:
                    break;
                case pt0::RenderVarType::VEC2:
                {
                    assert(m == 2);
                    sm::vec2 vec = val.vec2;
                    vec.xy[j] = static_cast<float>(v);
                    m_mat.SetVar(name, pt0::RenderVariant(vec));
                }
                    break;
                case pt0::RenderVarType::VEC3:
                {
                    assert(m == 3);
                    sm::vec3 vec = val.vec3;
                    vec.xyz[j] = static_cast<float>(v);
                    m_mat.SetVar(name, pt0::RenderVariant(vec));
                }
                    break;
                case pt0::RenderVarType::VEC4:
                {
                    assert(m == 4);
                    sm::vec4 vec = val.vec4;
                    vec.xyzw[j] = static_cast<float>(v);
                    m_mat.SetVar(name, pt0::RenderVariant(vec));
                }
                    break;
                case pt0::RenderVarType::MAT2:
                    break;
                case pt0::RenderVarType::MAT3:
                    break;
                case pt0::RenderVarType::MAT4:
                    break;
                }

                m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
                return;
            }
        }
    }
}

}