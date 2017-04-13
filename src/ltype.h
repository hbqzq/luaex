#ifndef ltype_h
#define ltype_h

#ifdef LUAEX_TYPECHECK

#include "ltable.h"
#include "lobject.h"
#include "lmap.h"

#define LUA_TYPE_NILABLE (1 << 30)
#define LUA_TYPE_MASK 0x3fffffff

const char* luaT_getTypename(lua_State* L, int id);
int luaT_mapTypename(lua_State* L, const char* name);

int luaT_matchType(lua_State* L, int required, int idx, int parmidx);

int inline luaT_setNillable(int id, int nilable) { return id | ((nilable != 0) * LUA_TYPE_NILABLE); }
int inline luaT_isNillable(int id) { return (id & LUA_TYPE_NILABLE) != 0; }

void luaT_typeInit(lua_State* L);
void luaT_typeDeinit(lua_State* L);

#endif /* LUAEX_TYPECHECK */

#endif /* ltype_h */