#pragma once

#define GAME_OBJ_ECS

#ifdef GAME_OBJ_ECS
#define ECS_WORLD_VAR world,
#define ECS_WORLD_SELF_VAR m_world,
#define ECS_WORLD_PARAM ecsx::World& world,
#define ECS_WORLD_DECL namespace ecsx { class World; }
#else
#define ECS_WORLD_VAR
#define ECS_WORLD_SELF_VAR
#define ECS_WORLD_PARAM
#define ECS_WORLD_DECL
#endif // GAME_OBJ_ECS

#ifndef GAME_OBJ_ECS
#define GAME_OBJ_VALID(obj) (obj)
#else
#define GAME_OBJ_VALID(obj) (!obj.IsNull())
#endif // GAME_OBJ_ECS