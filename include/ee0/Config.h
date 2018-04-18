#pragma once

#define GAME_OBJ_ECS

#ifdef GAME_OBJ_ECS
#define ECS_WORLD_DECL        namespace e0 { class World; }
#define ECS_WORLD_VAR         world,
#define ECS_WORLD_PARAM       e0::World& world,
#define ECS_WORLD_SELF_VAR    m_world,
#define ECS_WORLD_SELF_DEF    e0::World& m_world;
#define ECS_WORLD_SELF_ASSIGN ,m_world(world)
#else
#define ECS_WORLD_DECL
#define ECS_WORLD_VAR
#define ECS_WORLD_PARAM
#define ECS_WORLD_SELF_VAR
#define ECS_WORLD_SELF_DEF
#define ECS_WORLD_SELF_ASSIGN
#endif // GAME_OBJ_ECS

#ifndef GAME_OBJ_ECS
#define GAME_OBJ_VALID(obj) (obj)
#define GAME_OBJ_NULL nullptr
#else
#define GAME_OBJ_VALID(obj) (!(obj).IsNull())
#define GAME_OBJ_NULL ee0::GameObj()
#endif // GAME_OBJ_ECS