#include "ee0/SettingCfg.h"

namespace ee0
{

CU_SINGLETON_DEFINITION(SettingCfg)

SettingCfg::SettingCfg()
	: m_edit_op_type(EditOpType::EDIT_SHARED)
{
}

}