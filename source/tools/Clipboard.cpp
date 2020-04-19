#include "ee0/Clipboard.h"

#include <node0/SceneNode.h>
#include <node2/AABBSystem.h>
#include <node2/CompBoundingBox.h>
#include <ns/NodeSerializer.h>
#include <js/RapidJsonHelper.h>

#include <wx/clipbrd.h>
#include <rapidjson/document.h>

namespace ee0
{

CU_SINGLETON_DEFINITION(Clipboard)

Clipboard::Clipboard()
{
}

// fixme: no use
//void Clipboard::Clear()
//{
//    wxTheClipboard->Clear();
//}

void Clipboard::StoreNodesToCB(const std::vector<n0::SceneNodePtr>& nodes)
{
    rapidjson::Document doc;
    doc.SetObject();

    auto& alloc = doc.GetAllocator();

    rapidjson::Value nodes_val;
    nodes_val.SetArray();
    for (auto& n : nodes)
    {
        rapidjson::Value node_val;
        ns::NodeSerializer::StoreToJson(n, "", node_val, alloc);
        nodes_val.PushBack(node_val, alloc);
    }
//    doc.AddMember("nodes", nodes_val, alloc);

//    SetValue("nodes", js::RapidJsonHelper::ValueToString(doc));

    FromString(js::RapidJsonHelper::ValueToString(nodes_val));
}

void Clipboard::LoadNodesFromCB(const ur2::Device& dev, std::vector<n0::SceneNodePtr>& nodes)
{
    nodes.clear();

    auto str = ToString();
//    auto str = GetValue("nodes");

    rapidjson::Document doc;
    if (doc.Parse<0>(str.c_str()).HasParseError()) {
        return;
    }

    for (auto itr = doc.Begin(); itr != doc.End(); ++itr)
    {
        auto node = std::make_shared<n0::SceneNode>();
        ns::NodeSerializer::LoadFromJson(dev, node, "", *itr);

        auto aabb = n2::AABBSystem::Instance()->GetBounding(*node);
        node->AddUniqueComp<n2::CompBoundingBox>(aabb);

        nodes.push_back(node);
    }
}

//void Clipboard::SetValue(const std::string& key, const std::string& val)
//{
//    auto str = GetAll();
//
//    rapidjson::Document doc;
//    if (doc.Parse<0>(str.c_str()).HasParseError()) {
//        return;
//    }
//
//    auto& alloc = doc.GetAllocator();
//    doc.AddMember(rapidjson::Value(key.c_str(), alloc),
//        rapidjson::Value(val.c_str(), alloc), alloc);
//}
//
//std::string Clipboard::GetValue(const std::string& key) const
//{
//    auto str = GetAll();
//
//    rapidjson::Document doc;
//    if (doc.Parse<0>(str.c_str()).HasParseError()) {
//        return "";
//    }
//
//    return js::RapidJsonHelper::ValueToString(doc[key.c_str()]);
//}

void Clipboard::FromString(const std::string& all)
{
    wxTheClipboard->SetData(new wxTextDataObject(all));
    wxTheClipboard->Close();
}

std::string Clipboard::ToString() const
{
    std::string ret;

    if (!wxTheClipboard->Open()) {
        return ret;
    }

    if (!wxTheClipboard->IsSupported(wxDF_TEXT)) {
        wxTheClipboard->Close();
        return ret;
    }

    wxTextDataObject data;
    wxTheClipboard->GetData(data);

    //std::string str = data.GetText().ToStdString();
    //if (str.find("{") == std::string::npos) {
    //    wxTheClipboard->Close();
    //    return;
    //}

    ret = data.GetText().ToStdString();

    wxTheClipboard->Close();

    return ret;
}

}