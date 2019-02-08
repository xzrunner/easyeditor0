#pragma once

#include <wx/collpane.h>
#include <rttr/type>

namespace ee0
{

struct UIMetaInfo;

class WxCompPanel : public wxCollapsiblePane
{
public:
	WxCompPanel(wxWindow* parent, const std::string& title);

    virtual void RefreshNodeComp();

private:
	enum
	{
		ID_NODE_COMP_PANEL = 2000
	};

protected:
    void InitControl(wxSizer* top_sizer, const UIMetaInfo& info, rttr::instance obj, rttr::property prop);

private:
	void OnCollapsChanged(wxCollapsiblePaneEvent& event);

    void SetTextValue(wxCommandEvent& event);
    void SetCheckboxValue(wxCommandEvent& event);

private:
    struct Control
    {
        std::string flag;
        rttr::instance obj;
        rttr::property prop;
        wxControl* control = nullptr;
    };

private:
    std::vector<Control> m_controls;

	DECLARE_EVENT_TABLE()

}; // WxCompPanel

}