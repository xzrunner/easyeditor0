#pragma once

#include "ee0/Observer.h"
#include "ee0/WxEditPanel.h"
#include "ee0/SelectionSet.h"
#include "ee0/NodeContainer.h"
#include "ee0/typedef.h"
#include "ee0/Config.h"

namespace ee0
{

class WxStagePage : public WxEditPanel, public Observer, public NodeContainer
{
public:
	enum TraverseType
	{
		TRAV_DRAW = 0,
		TRAV_QUERY,
	};

public:
	WxStagePage(wxWindow* parent);
	// todo
//	virtual ~WxStagePage();

	virtual void OnNotify(uint32_t msg, const VariantSet& variants) override;

	const SelectionSet<GameObjWithPos>& GetSelection() const {
		return m_selection;
	}
	SelectionSet<GameObjWithPos>& GetSelection() {
		return m_selection;
	}

	bool IsEditDirty() const { return m_edit_dirty; }

private:
	void SelectionInsert(const VariantSet& variants);
	void SelectionDelete(const VariantSet& variants);

	void SetEditorDirty(const ee0::VariantSet& variants);

protected:
	SelectionSet<GameObjWithPos> m_selection;

private:
	bool m_edit_dirty;

}; // WxStagePage

}