#pragma once

#include <cu/cu_macro.h>

namespace ee0
{

namespace EditOpType
{
	enum {
		EDIT_SHARED,
		EDIT_SHARED_PATCH,
		EDIT_UNIQUE,
	};
}

class SettingCfg
{
public:
	int GetEditOpType() const { return m_edit_op_type; }
	void SetEditOpType(int type) { m_edit_op_type = type; }
	
private:
	int m_edit_op_type;

	CU_SINGLETON_DECLARATION(SettingCfg);

}; // SettingCfg

}