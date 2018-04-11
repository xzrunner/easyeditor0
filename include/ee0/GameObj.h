#pragma once

#include "ee0/Config.h"

#ifndef GAME_OBJ_ECS
#include <node0/typedef.h>
#else
#include <ecsx/Entity.h>
#endif // GAME_OBJ_ECS

namespace ee0
{

#ifndef GAME_OBJ_ECS
	typedef n0::SceneNodePtr GameObj;
	typedef n0::NodeWithPos  GameObjWithPos;
#else
	typedef ecsx::Entity GameObj;
	typedef ecsx::Entity GameObjWithPos;
#endif // GAME_OBJ_ECS

}