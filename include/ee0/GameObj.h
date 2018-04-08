#pragma once

#include "ee0/Config.h"

#ifndef EONE_ECS
#include <node0/typedef.h>
#else
#include <ecsx/Entity.h>
#endif // EONE_ECS

namespace ee0
{

#ifndef EONE_ECS
	typedef n0::SceneNodePtr GameObj;
#else
	typedef ecsx::Entity GameObj;
#endif // EONE_ECS

}