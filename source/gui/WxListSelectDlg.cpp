#include "ee0/WxListSelectDlg.h"

#include <guard/check.h>

#include <wx/sizer.h>
#include <wx/button.h>

namespace ee0
{

WxListSelectDlg::WxListSelectDlg(wxWindow* parent, const std::string& title,
	                             const std::vector<std::pair<uint32_t, std::string>>& list,
	                             const wxPoint& pos, const wxSize& size)
	: wxDialog(parent, wxID_ANY, title, pos, size)
{
	InitLayout(list);

	for (auto& item : list) {
		m_name2id.insert(std::make_pair(item.second, item.first));
	}
}

uint32_t WxListSelectDlg::GetSelectedID() const
{
	auto name = m_tree->GetItemText(m_tree->GetSelection()).ToStdString();
	auto itr = m_name2id.find(name);
	GD_ASSERT(itr != m_name2id.end(), "err name");
	return itr->second;
}

void WxListSelectDlg::InitLayout(const std::vector<std::pair<uint32_t, std::string>>& list)
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	m_tree = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(200, 200),
		wxTR_HIDE_ROOT | /*wxTR_NO_LINES | */wxTR_DEFAULT_STYLE);
	Bind(wxEVT_TREE_SEL_CHANGED, &WxListSelectDlg::OnSelChanged, this, m_tree->GetId());
	Bind(wxEVT_TREE_ITEM_ACTIVATED, &WxListSelectDlg::OnDoubleClick, this, m_tree->GetId());

	auto root = m_tree->AddRoot("ROOT");
	for (auto& item : list) {
		m_tree->InsertItem(root, -1, item.second);
	}

	sizer->Add(m_tree);

	wxBoxSizer* btn_sizer = new wxBoxSizer(wxHORIZONTAL);
	btn_sizer->Add(new wxButton(this, wxID_OK), wxALL, 5);
	btn_sizer->Add(new wxButton(this, wxID_CANCEL), wxALL, 5);
	sizer->Add(btn_sizer, 0, wxALL, 5);

	SetSizer(sizer);
	sizer->Fit(this);
}

void WxListSelectDlg::OnSelChanged(wxTreeEvent& event)
{
	auto id = event.GetItem();
	if (!id.IsOk()) {
		return;
	}

	auto text = m_tree->GetItemText(id);
}

void WxListSelectDlg::OnDoubleClick(wxTreeEvent& event)
{
	assert(IsModal());
	EndModal(wxID_OK);
}

}