#pragma once

#include <rapidjson/document.h>
#include <wx/wx.h>

#include <map>

namespace ee0
{

class EditPanelImpl;

class RightPopupMenu
{
public:
	RightPopupMenu(wxWindow* parent, EditPanelImpl* stage);

	void SetRightPopupMenu(wxMenu& menu, int x, int y);
	void OnRightPopupMenu(int id);

private:
	void LoadMenu(wxMenu& parent, const rapidjson::Value& children);

private:
	wxWindow* m_parent;

	EditPanelImpl* m_stage;

	std::map<int, std::string> m_id2path;

}; // RightPopupMenu

}