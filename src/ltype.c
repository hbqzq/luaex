#include <string.h>

#include "lstate.h"
#include "ltype.h"
#include "lstring.h"
#include "lgc.h"
#include "lauxlib.h"

#ifdef LUAEX_TYPECHECK

static void luaT_error(lua_State* L, int required_id, int got_id, int parmidx) {
	const char* got = got_id < 0 ? "unknown" : luaT_getTypename(L, got_id);
	const char* expected = luaT_getTypename(L, required_id);
	if (parmidx > 0)
		luaL_error(L, "invalid type of param[%d], <%s> expected, got <%s>", parmidx, expected, got);
	else
		luaL_error(L, "invalid type of return value, <%s> expected, got <%s>", expected, got);
}

const char* luaT_getTypename(lua_State* L, int id) {
	id &= LUA_TYPE_MASK;
	global_State *g = L->l_G;
	if (id < g->tc_size)
		return g->tc[id];
	return NULL;
}

int luaT_mapTypename(lua_State* L, const char* name) {
	if (name == NULL) return 0;

	global_State *g = L->l_G;

	int* val = lmap_get(&g->tc_map, name);
	if (val) return *val;

	int id = g->tc_size++;
	if (g->tc_size > g->tc_cap) {
		int newcap = g->tc_cap << 1;
		g->tc = luaM_reallocvector(L, g->tc, g->tc_cap, newcap, char*);
		g->tc_cap = newcap;
	}
	lmap_set(&g->tc_map, name, id);

	size_t sizename = strlen(name);
	char* tcname = luaM_malloc(L, sizename + 1);
	memcpy(tcname, name, sizename + 1);
	g->tc[id] = tcname;

	return id;
}

int luaT_matchType(lua_State* L, int required, int idx, int parmidx) {
	int nilable;
	int got;

	if ((required & LUA_TYPE_MASK) == 0) return 1; //any

	nilable = (required & LUA_TYPE_NILABLE) != 0;
	if (lua_isnil(L, idx)) {
		if (nilable) return 1;
		else return 0;
	}

	required &= LUA_TYPE_MASK;
	got = lua_type(L, idx);

	/* Custom type-matching codes here */

	if (required == got) return 1;
	luaT_error(L, required, got, parmidx);
	return 0;
}

void luaT_typeInit(lua_State* L) {
	global_State* g = L->l_G;
	g->tc_cap = 64;
	g->tc_size = 0;
	g->tc = luaM_newvector(L, g->tc_cap, char*);
	lmap_init(&g->tc_map);

	luaT_mapTypename(L, "nil");
	luaT_mapTypename(L, "bool");
	luaT_mapTypename(L, "userdata");
	luaT_mapTypename(L, "number");
	luaT_mapTypename(L, "string");
	luaT_mapTypename(L, "table");
	luaT_mapTypename(L, "function");
	luaT_mapTypename(L, "lightuserdata");
	luaT_mapTypename(L, "thread");

	lmap_set(&g->tc_map, "any", LUA_TNIL); // aka nil
	lmap_set(&g->tc_map, "boolean", LUA_TBOOLEAN); // aka bool
}

void luaT_typeDeinit(lua_State* L) {
	global_State* g = L->l_G;
	for (int i = 0; i < g->tc_size; i++)
		luaM_free(L, g->tc[i]);
	luaM_freearray(L, g->tc, g->tc_cap);
	lmap_deinit(&g->tc_map);
	g->tc_size = 0;
	g->tc_cap = 0;
}
#else /* LUAEX_TYPECHECK */

void luaT_no_warning() {
}

#endif /* LUAEX_TYPECHECK */
