#pragma once

#include "ee0/typedef.h"

#include <painting0/RenderVariant.h>

#include <wx/window.h>

class wxTextCtrl;

namespace pt0 { class Material; class RenderVariant; }

namespace ee0
{

class WxMaterialCtrl : public wxWindow
{
public:
    WxMaterialCtrl(wxWindow* parent, const ee0::SubjectMgrPtr& sub_mgr,
        pt0::Material& mat);

    void RefreshView();

private:
    void InitLayout();

    void EnterTextValue(wxCommandEvent& event);

private:
    ee0::SubjectMgrPtr m_sub_mgr;

    pt0::Material& m_mat;

    std::vector<std::vector<wxControl*>> m_ctrls;

}; // WxMaterialCtrl

}