#pragma once

#include "ee0/EditOP.h"
#include "ee0/GameObj.h"
#include "ee0/typedef.h"

#include <SM_Vector.h>

#include <vector>

namespace ee0
{

class WxStagePage;
class VariantSet;

class NodeSelectOP : public EditOP
{
public:
	NodeSelectOP(const std::shared_ptr<pt0::Camera>& cam,
		WxStagePage& stage);

	virtual bool OnKeyDown(int key_code) override;
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
    virtual bool OnMouseMove(int x, int y) override;

protected:
	virtual GameObj QueryByPos(int screen_x, int screen_y) const = 0;
	virtual void QueryByRect(const sm::ivec2& p0, const sm::ivec2& p1,
		bool contain, std::vector<GameObj>& result) const = 0;

    // callback func
    virtual void AfterInsertSelected(const n0::SceneNodePtr& node) const {}
    virtual void AfterDeleteSelected(const n0::SceneNodePtr& node) const {}
    virtual void AfterClearSelection() const {}

private:
    void DeleteSelection();
    void CopySelectionToClipboard();
    void PasteSelectionFromClipboard();

    void InsertSelected(const SubjectMgrPtr& sub_mgr,
        const n0::SceneNodePtr& node, const VariantSet& variants) const;
    void DeleteSelected(const SubjectMgrPtr& sub_mgr,
        const n0::SceneNodePtr& node, const VariantSet& variants) const;
    void ClearSelection(const SubjectMgrPtr& sub_mgr) const;

protected:
	WxStagePage& m_stage;

private:
	sm::ivec2 m_last_pos;

}; // NodeSelectOP

}
