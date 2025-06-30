#include <stdio.h>

#include <lua.h>
#include <lapi.h>
#include <lgc.h>
#include <lobject.h>
#include <ltable.h>
#include <lstate.h>
#include <lfunc.h>

#include "lua_memory_snapshot.h"

// use bit 6 to mark traversed object
#define TRAVERSEDBIT		6
#define is_traversed(x)		testbit((x)->marked, TRAVERSEDBIT)
#define set_traversed(x)	l_setbit((x)->marked, TRAVERSEDBIT)
#define reset_traversed(x)	resetbit((x)->marked, TRAVERSEDBIT)


// defined in lapi.c
TValue *index2addr(lua_State *L, int idx);


static size_t get_string_size(const TString *s)
{
	size_t size = sizeof(UTString)
				+ tsslen(s) + 1;
	return size;
}

static size_t get_table_size(const Table *t)
{
	size_t size = sizeof(Table)
				+ sizeof(TValue) * t->sizearray
				+ sizeof(Node) * cast(size_t, allocsizenode(t));
	return size;
}

static size_t get_userdata_size(const Udata *ud)
{
	size_t size = sizeof(UUdata)
				+ ud->len;
	return size;
}

static size_t get_thread_size(const lua_State *th)
{
	size_t size = LUA_EXTRASPACE
				+ sizeof(lua_State)
				+ sizeof(TValue) * th->stacksize
				+ sizeof(CallInfo) * th->nci
				+ sizeof(UpVal) * th->nliveupval;
	return size;
}

static size_t get_lua_closure_size(const LClosure *cl)
{
	size_t size = sizeof(LClosure)
				+ sizeof(UpVal*) * (cl->nupvalues - 1);
	return size;
}

static size_t get_c_closure_size(const CClosure *cl)
{
	size_t size = sizeof(CClosure)
				+ sizeof(TValue) * (cl->nupvalues - 1);
	return size;
}

static size_t get_proto_size(const Proto *p)
{
	size_t size = sizeof(Proto)
				+ sizeof(Instruction) * p->sizecode
				+ sizeof(Proto*) * p->sizep
				+ sizeof(TValue) * p->sizek
				+ sizeof(int) * p->sizelineinfo
				+ sizeof(LocVar) * p->sizelocvars
				+ sizeof(Upvaldesc) * p->sizeupvalues;
	return size;
}

static GCObject *get_gc_object(lua_State *L, int index)
{
	TValue *v = index2addr(L, -1);
	if (v != NULL && iscollectable(v))
	{
		return gcvalue(v);
	}
	return NULL;
}

static const char *get_key_string(TValue *key, char *buffer, size_t size)
{
	int type = ttnov(key);
	switch (type)
	{
	case LUA_TNIL:
		return "[nil]";
	case LUA_TBOOLEAN:
		return bvalue(key) ? "[true]" : "[false]";
	case LUA_TNUMBER:
		snprintf(buffer, size, "[%lg]", nvalue(key));
		break;
	case LUA_TSTRING:
		return getstr(tsvalue(key));
	default:
		snprintf(buffer, size, "[%s:%p]", ttypename(type), gcvalue(key));
		break;
	}
	return buffer;
}

typedef LuaSnapshotCallback snapshot_callback;

static void traverse_object(lua_State *L, GCObject *o, const char *name, const char *desc, const void *parent, snapshot_callback cb);

static void traverse_string(lua_State *L, GCObject *o, const char *name, const char *desc, const void *parent, snapshot_callback cb)
{
	TString *s = gco2ts(o);
	size_t size = get_string_size(s);
	const char *value = getstr(s);
	cb(o, LUA_TSTRING, size, name, desc, value, parent);
}

static void traverse_table(lua_State *L, GCObject *o, const char *name, const char *desc, const void *parent, snapshot_callback cb)
{
	Table *t = gco2t(o);
	size_t size = get_table_size(t);
	cb(o, LUA_TTABLE, size, name, desc, NULL, parent);

	if (t->metatable != NULL)
	{
		traverse_object(L, (GCObject*)t->metatable, "__metatable", "metatable", o, cb);
	}

	char buffer[256];

	for (unsigned int i = 0; i < t->sizearray; i++)
	{
		TValue *item = &t->array[i];
		if (iscollectable(item))
		{
			snprintf(buffer, sizeof(buffer), "[%d]", i + 1);
			traverse_object(L, gcvalue(item), buffer, "array item", o, cb);
		}
	}

	Node *last = gnode(t, cast(size_t, sizenode(t)));
	for (Node *n = gnode(t, 0); n < last; n++)
	{
		TValue *key = (TValue*)gkey(n);
		if (iscollectable(key))
		{
			traverse_object(L, gcvalue(key), "<key>", "key", o, cb);
		}
		
		TValue *value = gval(n);
		if (iscollectable(value))
		{
			const char *value_name = get_key_string(key, buffer, sizeof(buffer));
			traverse_object(L, gcvalue(value), value_name, "value", o, cb);
		}
	}
}

static void traverse_userdata(lua_State *L, GCObject *o, const char *name, const char *desc, const void *parent, snapshot_callback cb)
{
	Udata *ud = gco2u(o);
	size_t size = get_userdata_size(ud);
	cb(o, LUA_TUSERDATA, size, name, desc, NULL, parent);

	if (ud->metatable != NULL)
	{
		traverse_object(L, (GCObject*)ud->metatable, "__metatable", "metatable", o, cb);
	}

	if (ud->ttuv_ & BIT_ISCOLLECTABLE)
	{
		traverse_object(L, ud->user_.gc, "<uservalue>", "user value", o, cb);
	}
}

static void traverse_thread(lua_State *L, GCObject *o, const char *name, const char *desc, const void *parent, snapshot_callback cb)
{
	lua_State *th = gco2th(o);
	size_t size = get_thread_size(th);
	cb(o, LUA_TTHREAD, size, name, desc, NULL, parent);

	char buffer1[256];
	char buffer2[256];

	// stack variables
	int top = lua_gettop(th);
	for (int i = 1; i <= top; i++)
	{
		GCObject *v = get_gc_object(th, i);
		if (v != NULL)
		{
			snprintf(buffer1, sizeof(buffer1), "<stack %d>", i);
			snprintf(buffer2, sizeof(buffer2), "stack variable %d", i);
			traverse_object(L, v, buffer1, buffer2, o, cb);
		}
	}

	// local variables
	lua_Debug ar;
	int level = th == L ? 1 : 0;
	while (lua_getstack(th, level, &ar))
	{
		lua_getinfo(th, "Sl", &ar);
		for (int j = 1; j >= -1; j -= 2)
		{
			for (int i = j; ; i += j)
			{
				const char *local_name = lua_getlocal(th, &ar, i);
				if (local_name == NULL)
					break;

				GCObject *v = get_gc_object(th, -1);
				if (v != NULL)
				{
					snprintf(buffer1, sizeof(buffer1), "<local %d>", i);
					snprintf(buffer2, sizeof(buffer2), "local variable %d: %s @ %s:%d", i, local_name, ar.short_src, ar.currentline);
					traverse_object(L, v, local_name[0] ? local_name : buffer1, buffer2, o, cb);
				}

				// pop local var
				lua_pop(th, 1);
			}
		}
		++level;
	}
}

static void traverse_lua_closure(lua_State *L, GCObject *o, const char *name, const char *desc, const void *parent, snapshot_callback cb)
{
	LClosure *cl = gco2lcl(o);
	size_t size = get_lua_closure_size(cl);

	char buffer1[256];
	char buffer2[256];

	// push closure
	lua_lock(L);
	setclLvalue(L, L->top, cl);
	api_incr_top(L);
	lua_unlock(L);

	// get desc
	lua_Debug ar;
	lua_pushvalue(L, -1);
	lua_getinfo(L, ">S", &ar);
	snprintf(buffer1, sizeof(buffer1), "lua function @ %s:%d", ar.short_src, ar.linedefined);

	cb(o, LUA_TFUNCTION, size, name, desc, buffer1, parent);

	// upvalues
	for (int i = 1; ; i++)
	{
		const char *uv_name = lua_getupvalue(L, -1, i);
		if (uv_name == NULL)
			break;

		GCObject *uv = get_gc_object(L, -1);
		if (uv != NULL)
		{
			snprintf(buffer1, sizeof(buffer1), "<upvalue %d>", i);
			snprintf(buffer2, sizeof(buffer2), "upvalue %d: %s", i, uv_name);
			traverse_object(L, uv, uv_name[0] ? uv_name : buffer1, buffer2, o, cb);
		}

		// pop upvalue
		lua_pop(L, 1);
	}

	// pop closure
	lua_pop(L, 1);
}

static void traverse_c_closure(lua_State *L, GCObject *o, const char *name, const char *desc, const void *parent, snapshot_callback cb)
{
	CClosure *cl = gco2ccl(o);
	size_t size = get_c_closure_size(cl);
	cb(o, LUA_TFUNCTION, size, name, desc, "C function", parent);

	char buffer1[256];
	char buffer2[256];

	// push closure
	lua_lock(L);
	setclCvalue(L, L->top, cl);
	api_incr_top(L);
	lua_unlock(L);

	// upvalues
	for (int i = 1; ; i++)
	{
		const char *uv_name = lua_getupvalue(L, -1, i);
		if (uv_name == NULL)
			break;

		GCObject *uv = get_gc_object(L, -1);
		if (uv != NULL)
		{
			snprintf(buffer1, sizeof(buffer1), "<upvalue %d>", i);
			snprintf(buffer2, sizeof(buffer2), "upvalue %d: %s", i, uv_name);
			traverse_object(L, uv, uv_name[0] ? uv_name : buffer1, buffer2, o, cb);
		}

		// pop upvalue
		lua_pop(L, 1);
	}

	// pop closure
	lua_pop(L, 1);
}

static void traverse_function(lua_State *L, GCObject *o, const char *name, const char *desc, const void *parent, snapshot_callback cb)
{
	if (o->tt == LUA_TLCL)
	{
		traverse_lua_closure(L, o, name, desc, parent, cb);
	}
	else if (o->tt == LUA_TCCL)
	{
		traverse_c_closure(L, o, name, desc, parent, cb);
	}
}

static void traverse_proto(lua_State *L, GCObject *o, const char *name, const char *desc, const void *parent, snapshot_callback cb)
{
	Proto *p = gco2p(o);
	size_t size = get_proto_size(p);

	char buffer[256];
	snprintf(buffer, sizeof(buffer), "proto @ %s:%d", getstr(p->source), p->linedefined);

	cb(o, LUA_TPROTO, size, name, desc, buffer, parent);
}

static void traverse_object(lua_State *L, GCObject *o, const char *name, const char *desc, const void *parent, snapshot_callback cb)
{
	if (o == NULL)
		return;

	int type = novariant(o->tt);
	if (type < LUA_TSTRING)
		return;

	if (type > LUA_TPROTO)
	{
		// pointer messed up!!!
		api_check(L, 0, "invalid object pointer");
		return;
	}

	if (is_traversed(o))
	{
		if (parent != NULL)
		{
			// report traversed object with size 0, just show relationship with parent
			cb(o, type, 0, name, desc, NULL, parent);
		}
		return;
	}

	set_traversed(o);

	switch (type)
	{
	case LUA_TSTRING:
		traverse_string(L, o, name, desc, parent, cb);
		break;
	case LUA_TTABLE:
		traverse_table(L, o, name, desc, parent, cb);
		break;
	case LUA_TUSERDATA:
		traverse_userdata(L, o, name, desc, parent, cb);
		break;
	case LUA_TTHREAD:
		traverse_thread(L, o, name, desc, parent, cb);
		break;
	case LUA_TFUNCTION:
		traverse_function(L, o, name, desc, parent, cb);
		break;
	case LUA_TPROTO:
		traverse_proto(L, o, name, desc, parent, cb);
		break;
	}
}


// take memory snapshot by traversing all living objects
// cb: callback called on each object and relationship
LUA_API void lua_memory_snapshot(lua_State *L, snapshot_callback cb)
{
	// global state
	size_t size = sizeof(global_State) + sizeof(TString*) * G(L)->strt.size;
	cb(G(L), LUA_TNONE, size, "<global state>", "global state and string table", NULL, NULL);

	// registry
	GCObject *registry = (GCObject*)lua_topointer(L, LUA_REGISTRYINDEX);
	traverse_object(L, registry, "registry", "registry table", NULL, cb);

	// main thread
	traverse_object(L, (GCObject*)G(L)->mainthread, "<main thread>", "main thread", NULL, cb);

	// all objects
	GCObject *list[4] = { G(L)->allgc, G(L)->finobj, G(L)->tobefnz, G(L)->fixedgc };
	for (int i = 0; i < 4; i++)
	{
		for (GCObject *o = list[i]; o != NULL; o = o->next)
		{
			traverse_object(L, o, NULL, NULL, NULL, cb);
		}
	}

	// clear traversed mark
	reset_traversed(registry);
	reset_traversed(G(L)->mainthread);
	for (int i = 0; i < 4; i++)
	{
		for (GCObject *o = list[i]; o != NULL; o = o->next)
		{
			reset_traversed(o);
		}
	}
}
