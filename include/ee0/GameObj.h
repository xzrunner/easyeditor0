#pragma once

#include "ee0/Config.h"

#ifndef GAME_OBJ_ECS
#include <node0/typedef.h>
#include <node0/NodeWithPos.h>
#else
#include <entity0/Entity.h>
#endif // GAME_OBJ_ECS

namespace ee0
{

#ifndef GAME_OBJ_ECS
	typedef n0::SceneNodePtr GameObj;
	typedef n0::NodeWithPos  GameObjWithPos;
#else
	typedef e0::Entity GameObj;
	typedef e0::Entity GameObjWithPos;
#endif // GAME_OBJ_ECS

}