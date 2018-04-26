#pragma once

#include "ee0/Observer.h"
#include "ee0/WxEditPanel.h"
#include "ee0/SelectionSet.h"
#include "ee0/NodeContainer.h"
#include "ee0/EditRecord.h"
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

	EditRecord& GetEditRecord() { return m_edit_record; }

	uint32_t FetchObjID() { return m_next_obj_id++; }

private:
	void RegisterMoonCallback();

	void SelectionInsert(const VariantSet& variants);
	void SelectionDelete(const VariantSet& variants);
	void SelectionClear();

	void SetEditorDirty(const ee0::VariantSet& variants);

	void AddAtomicOp(const ee0::VariantSet& variants);
	void OnEditOpUndo();
	void OnEditOpRedo();

protected:
	SelectionSet<GameObjWithPos> m_selection;

private:
	bool m_edit_dirty;

	EditRecord m_edit_record;

	uint32_t m_next_obj_id;

}; // WxStagePage

}