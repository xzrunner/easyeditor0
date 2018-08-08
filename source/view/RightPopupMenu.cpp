#include "ee0/RightPopupMenu.h"
#include "ee0/EditPanelImpl.h"

#include <js/RapidJsonHelper.h>
#include <moon/ScriptHelper.h>
#include <moon/Blackboard.h>
#include <moon/Context.h>

#include <boost/filesystem.hpp>

namespace
{

const int MENU_ID_START = 10000;

const char* CFG_DIR  = "script/action";
const char* CFG_PATH = "script/action/layout.json";

}

namespace ee0
{

RightPopupMenu::RightPopupMenu(wxWindow* parent, EditPanelImpl* stage)
	: m_parent(parent)
	, m_stage(stage)
{
}

void RightPopupMenu::SetRightPopupMenu(wxMenu& menu, int x, int y)
{
	m_id2path.clear();

	rapidjson::Document doc;
	if (js::RapidJsonHelper::ReadFromFile(CFG_PATH, doc)) {
		LoadMenu(menu, doc["children"]);
	}
}

void RightPopupMenu::OnRightPopupMenu(int id)
{
	auto itr = m_id2path.find(id);
	if (itr == m_id2path.end()) {
		return;
	}

	auto L = moon::Blackboard::Instance()->GetContext()->GetState();
	auto path = boost::filesystem::absolute(itr->second, CFG_DIR);
	auto err = moon::ScriptHelper::DoFile(L, path.string().c_str());
	if (err) {
		printf("lua err: %s\n", err);
	}
}

void RightPopupMenu::LoadMenu(wxMenu& parent, const rapidjson::Value& children)
{
	for (auto& child : children.GetArray())
	{
		auto type = child["type"].GetString();
		if (strcmp(type, "menu") == 0)
		{
			auto sub = new wxMenu;
			LoadMenu(*sub, child["children"]);
			parent.Append(MENU_ID_START, child["name"].GetString(), sub);
		}
		else if (strcmp(type, "action") == 0)
		{
			int id = MENU_ID_START + m_id2path.size();
			m_parent->Bind(wxEVT_COMMAND_MENU_SELECTED, 
				&EditPanelImpl::OnRightPopupMenu, m_stage, id);
			parent.Append(id, child["name"].GetString(), child["desc"].GetString());
			m_id2path.insert(std::make_pair(id, child["file"].GetString()));
		}
	}
}

}