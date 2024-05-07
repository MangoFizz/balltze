// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__HELPERS__LUACSTRUCT_HPP
#define BALLTZE__PLUGINS__LUA__HELPERS__LUACSTRUCT_HPP

/**
 * This is a modified version of the luacstruct header that 
 * converts the field names from snake_case to camelCase.
 */

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <lua.hpp>
#include <string>
#include <cctype>
#include <balltze/helpers/string_literal.hpp>

#define LUACS_FREADONLY		0x01
#define LUACS_FENDIANBIG	0x02
#define LUACS_FENDIANLITTLE	0x04
#define LUACS_FENDIAN		(LUACS_FENDIANBIG | LUACS_FENDIANLITTLE)


#ifndef DONT_FUCK_WITH_INTELLISENSE
#define SNAKE_TO_CAMEL(s) snake_to_camel_case(s).data
#else 
#define SNAKE_TO_CAMEL(s) s
#endif

extern "C" {
    enum luacstruct_type {
        LUACS_TINT8,
        LUACS_TINT16,
        LUACS_TINT32,
        LUACS_TINT64,
        LUACS_TUINT8,
        LUACS_TUINT16,
        LUACS_TUINT32,
        LUACS_TUINT64,
        LUACS_FLOAT,
        LUACS_TENUM,
        LUACS_TBOOL,
        LUACS_TSTRING,
        LUACS_TSTRPTR,
        LUACS_TBYTEARRAY,
        LUACS_TOBJREF,
        LUACS_TOBJENT,
        LUACS_TEXTREF,
        LUACS_TARRAY,
        LUACS_TMETHOD,
        LUACS_TCONST
    };

    int luacs_newstruct0(lua_State *, const char *, const char *);
    bool luacs_ctype_exists0(lua_State *L, const char *tname);
    int luacs_declare_method(lua_State *, const char *, int (*)(lua_State *));
    int luacs_declare_const(lua_State *, const char *, int);
    int luacs_delstruct(lua_State *, const char *);
    int luacs_declare_field(lua_State *, enum luacstruct_type, const char *, const char *, size_t, int, int, unsigned);
    int luacs_newobject0(lua_State *, const char *, void *);
    void *luacs_object_pointer(lua_State *, int, const char *);
    int luacs_object_typename(lua_State *);
    void *luacs_checkobject(lua_State *, int, const char *);
    int luacs_newenum0(lua_State *, const char *, size_t);
    int luacs_newenumval(lua_State *, const char *, intmax_t);
    int luacs_delenum(lua_State *, const char *);
    int luacs_enum_declare_value(lua_State *, const char *, intmax_t);
    int luacs_checkenumval(lua_State *, int, const char *);
    int luacs_newarray(lua_State *, enum luacstruct_type, const char *, size_t, int, unsigned, void *);
    int luacs_newarraytype(lua_State *, const char *, enum luacstruct_type, const char *, size_t, int, unsigned);

    #define OFFSET_OF(_type, _field) (int32_t)((uintptr_t)&((_type *)0)->_field)

    #define luacs_newstruct(_L, _typename) luacs_newstruct0((_L), #_typename, NULL);

    #define luacs_newderivedstruct(_L, _typename, _base) luacs_newstruct0((_L), #_typename, #_base); 

    #define luacs_newenum(_L, _enumname) luacs_newenum0((_L), #_enumname, sizeof(_enumname));

    #define validintwidth(_w) ((_w) == 1 || (_w) == 2 || (_w) == 4 || (_w) == 8)

    #define luacs_int_field(_L, _type, _field, _flags) { \
        static_assert(validintwidth(sizeof((_type *)0)->_field), "`"#_field"' is an unsupported int type");	\
        luacstruct_type _itype; \
        switch(sizeof(((_type *)0)->_field)) {	\
            case 1:	_itype = LUACS_TINT8; break; \
            case 2:	_itype = LUACS_TINT16; break; \
            case 4:	_itype = LUACS_TINT32; break; \
            case 8:	_itype = LUACS_TINT64; break; \
        } \
        luacs_declare_field((_L), _itype, NULL,	SNAKE_TO_CAMEL(#_field), sizeof(((_type *)0)->_field), OFFSET_OF(_type, _field), 0, _flags); \
    }

    #define luacs_unsigned_field(_L, _type, _field, _flags) { \
        static_assert(validintwidth(sizeof((_type *)0)->_field), "`"#_field"' is an unsupported int type"); \
        luacstruct_type _itype; \
        switch (sizeof(((_type *)0)->_field)) {	\
            case 1:	_itype = LUACS_TUINT8; break; \
            case 2:	_itype = LUACS_TUINT16; break; \
            case 4:	_itype = LUACS_TUINT32; break; \
            case 8:	_itype = LUACS_TUINT64; break; \
        } \
        luacs_declare_field((_L), _itype, NULL,	SNAKE_TO_CAMEL(#_field), sizeof(((_type *)0)->_field), OFFSET_OF(_type, _field), 0, _flags); \
    } 

    #define luacs_float_field(_L, _type, _field, _flags) { \
        static_assert(sizeof(float) == sizeof(((_type *)0)->_field), "`"#_field"' is not a float value"); \
        luacs_declare_field((_L), LUACS_FLOAT, NULL, SNAKE_TO_CAMEL(#_field), sizeof(((_type *)0)->_field), OFFSET_OF(_type, _field), 0, _flags);	\
    }

    #define luacs_enum_field(_L, _type, _etype, _field, _flags)	{ \
        static_assert(validintwidth(sizeof((_type *)0)->_field), "`"#_field"' is an unsupported int type"); \
        luacs_declare_field((_L), LUACS_TENUM, #_etype,	#_field, sizeof(((_type *)0)->_field), OFFSET_OF(_type, _field), 0, _flags); \
    }

    #define luacs_bool_field(_L, _type, _field, _flags)	{ \
        static_assert(sizeof(bool) == sizeof(((_type *)0)->_field), "`"#_field"' is not a bool value"); \
        luacs_declare_field((_L), LUACS_TBOOL, NULL, SNAKE_TO_CAMEL(#_field), sizeof(((_type *)0)->_field), OFFSET_OF(_type, _field), 0, _flags); \
    }

    #define luacs_bytearray_field(_L, _type, _field, _flags) { \
        luacs_declare_field((_L), LUACS_TBYTEARRAY, NULL, SNAKE_TO_CAMEL(#_field), sizeof(((_type *)0)->_field), OFFSET_OF(_type, _field), 0, _flags); \
    }

    #define luacs_string_field(_L, _type, _field, _flags) { \
        luacs_declare_field((_L), LUACS_TSTRING, NULL, SNAKE_TO_CAMEL(#_field), sizeof(((_type *)0)->_field), OFFSET_OF(_type, _field), 0, _flags); \
    }

    #define luacs_strptr_field(_L, _type, _field, _flags) { \
        static_assert(sizeof(char *) ==	sizeof(((_type *)0)->_field), "`"#_field"' is not a pointer value"); \
        luacs_declare_field((_L), LUACS_TSTRPTR, NULL, SNAKE_TO_CAMEL(#_field), sizeof(((_type *)0)->_field), OFFSET_OF(_type, _field), 0, _flags); \
    } 

    #define luacs_objref_field(_L, _type, _tname, _field, _flags) { \
        static_assert(sizeof(void *) ==	sizeof(((_type *)0)->_field), "`"#_field"' is not a pointer value"); \
        luacs_declare_field((_L), LUACS_TOBJREF, #_tname, SNAKE_TO_CAMEL(#_field), sizeof(((_type *)0)->_field), OFFSET_OF(_type, _field), 0, _flags); \
    } 

    #define luacs_nested_field(_L, _type, _tname, _field, _flags) { \
        luacs_declare_field((_L), LUACS_TOBJENT, #_tname, SNAKE_TO_CAMEL(#_field), sizeof(((_type *)0)->_field), OFFSET_OF(_type, _field), 0, _flags); \
    } 

    #define luacs_extref_field(_L, _type, _field, _flags) { \
        luacs_declare_field((_L), LUACS_TEXTREF, NULL, SNAKE_TO_CAMEL(#_field), sizeof(((_type *)0)->_field), OFFSET_OF(_type, _field), 0, _flags); \
    } 

    #define luacs_pseudo_field(_L, _type, _field, _flags) { \
        luacs_declare_field((_L), LUACS_TEXTREF, NULL, SNAKE_TO_CAMEL(#_field), 0, 0, 0, _flags); \
    } 

    #define	_nitems(_x) (sizeof(_x) / sizeof((_x)[0]))

    #define luacs_int_array_field(_L, _type, _field, _flags) { \
        static_assert(validintwidth(sizeof((_type *)0)->_field[0]), "`"#_field"' is an unsupported int type");	\
        luacstruct_type _itype;			\
        switch(sizeof(((_type *)0)->_field[0])) { \
            case 1:	_itype = LUACS_TINT8; break; \
            case 2:	_itype = LUACS_TINT16; break; \
            case 4:	_itype = LUACS_TINT32; break; \
            case 8:	_itype = LUACS_TINT64; break; \
        } \
        luacs_declare_field((_L), _itype, NULL,	SNAKE_TO_CAMEL(#_field), sizeof(((_type *)0)->_field[0]), OFFSET_OF(_type, _field), _nitems(((_type *)0)->_field), _flags); \
    } 

    #define luacs_unsigned_array_field(_L, _type, _field, _flags) { \
        static_assert(validintwidth(sizeof((_type *)0)->_field[0]), "`"#_field"' is an unsupported int type");	\
        luacstruct_type _itype;			\
        switch (sizeof(((_type *)0)->_field[0])){\
            case 1:	_itype = LUACS_TUINT8; break; \
            case 2:	_itype = LUACS_TUINT16; break; \
            case 4:	_itype = LUACS_TUINT32; break; \
            case 8:	_itype = LUACS_TUINT64; break; \
        } \
        luacs_declare_field((_L), _itype, NULL,	SNAKE_TO_CAMEL(#_field), sizeof(((_type *)0)->_field[0]), OFFSET_OF(_type, _field), _nitems(((_type *)0)->_field), _flags); \
    } 

    #define luacs_float_array_field(_L, _type, _field, _flags) { \
        static_assert(sizeof(float) == sizeof(((_type *)0)->_field[0]), "`"#_field"' is not a float value"); \
        luacs_declare_field((_L), LUACS_FLOAT, NULL, SNAKE_TO_CAMEL(#_field), sizeof(((_type *)0)->_field[0]), OFFSET_OF(_type, _field), _nitems(((_type *)0)->_field), _flags); \
    } 

    #define luacs_enum_array_field(_L, _type, _etype, _field, _flags) { \
        static_assert(validintwidth(sizeof((_type *)0)->_field[0]), "`"#_field"' is an unsupported int type"); \
        luacs_declare_field((_L), LUACS_TENUM, #_etype,	SNAKE_TO_CAMEL(#_field), sizeof(((_type *)0)->_field), OFFSET_OF(_type, _field), _nitems(((_type *)0)->_field), _flags); \
    } 

    #define luacs_bool_array_field(_L, _type, _field, _flags) { \
        static_assert(sizeof(bool) == sizeof(((_type *)0)->_field[0]), "`"#_field"' is not a bool value");		\
        luacs_declare_field((_L), LUACS_TBOOL, NULL, SNAKE_TO_CAMEL(#_field), sizeof(((_type *)0)->_field[0]), OFFSET_OF(_type, _field), _nitems(((_type *)0)->_field), _flags); \
    } 

    #define luacs_bytearray_array_field(_L, _type, _field, _flags) { \
        luacs_declare_field((_L), LUACS_TBYTEARRAY, NULL, SNAKE_TO_CAMEL(#_field), sizeof(((_type *)0)->_field[0]), OFFSET_OF(_type, _field),	_nitems(((_type *)0)->_field), _flags); \
    } 

    #define luacs_string_array_field(_L, _type, _field, _flags) { \
        luacs_declare_field((_L), LUACS_TSTRING, NULL, SNAKE_TO_CAMEL(#_field), sizeof(((_type *)0)->_field[0]), OFFSET_OF(_type, _field), _nitems(((_type *)0)->_field), _flags); \
    } 

    #define luacs_strptr_array_field(_L, _type, _field, _flags) { \
        static_assert(sizeof(char *) == sizeof(((_type *)0)->_field[0]),	"`"#_field"' is not a pointer value");	\
        luacs_declare_field((_L), LUACS_TSTRPTR, NULL, SNAKE_TO_CAMEL(#_field), sizeof(((_type *)0)->_field[0]), OFFSET_OF(_type, _field), _nitems(((_type *)0)->_field), _flags);\
    } 

    #define luacs_objref_array_field(_L, _type, _tname, _field, _flags) { \
        static_assert(sizeof(void *) == sizeof(((_type *)0)->_field[0]),	"`"#_field"' is not a pointer value");	\
        luacs_declare_field((_L), LUACS_TOBJREF, #_tname, SNAKE_TO_CAMEL(#_field), sizeof(((_type *)0)->_field[0]), OFFSET_OF(_type, _field),	_nitems(((_type *)0)->_field), _flags); \
    } 

    #define luacs_nested_array_field(_L, _type, _tname, _field, _flags) { \
        luacs_declare_field((_L), LUACS_TOBJENT, #_tname, SNAKE_TO_CAMEL(#_field), sizeof(((_type *)0)->_field[0]), OFFSET_OF(_type, _field), _nitems(((_type *)0)->_field), _flags); \
    } 

    #define luacs_extref_array_field(_L, _type, _field, _flags) { \
        luacs_declare_field((_L), LUACS_TEXTREF, NULL, SNAKE_TO_CAMEL(#_field), sizeof(((_type *)0)->_field[0]), OFFSET_OF(_type, _field), _nitems(((_type *)0)->_field), _flags); \
    } 

    #define luacs_array_array_field(_L, _type, _tname, _field, _flags) { \
        luacs_declare_field((_L), LUACS_TARRAY, #_tname, SNAKE_TO_CAMEL(#_field), sizeof(((_type *)0)->_field[0]), OFFSET_OF(_type, _field), _nitems(((_type *)0)->_field), _flags); \
    } 

    #define luacs_newobject(_L, _typename, _ptr) luacs_newobject0((_L), #_typename, _ptr); 

    #define luacs_ctype_exists(_L, _typename) luacs_ctype_exists0((_L), #_typename)

    #define luacs_to_object(_L, _idx, _typename) ((_typename *)luacs_checkobject((_L), (_idx), #_typename))

    #define luacs_checkenumvalue(_L, _idx, _typename) static_cast<_typename>(luacs_checkenumval((_L), (_idx), #_typename))
}

#endif
