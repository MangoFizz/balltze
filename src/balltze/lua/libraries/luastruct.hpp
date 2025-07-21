#ifndef LUASTRUCT_HELPERS_HPP
#define LUASTRUCT_HELPERS_HPP

#include <balltze/helpers/string_literal.hpp>
#include <luastruct/luastruct.h>

enum {
	LUAS_FIELD_READONLY = 0x01,
	LUAS_FIELD_POINTER = 0x02
};

#define LUAS_STRUCT_FIELD(type, field) (((type *)NULL)->field)
#define LUAS_SIZEOF_ARRAY(type, field) (sizeof(((type *)NULL)->field) / sizeof(((type *)NULL)->field[0]))

#ifndef DONT_FUCK_WITH_INTELLISENSE
#define SNAKE_TO_CAMEL(s) snake_to_camel_case(s).data
#else
#define SNAKE_TO_CAMEL(s) s
#endif

#define LUAS_ENUM(state, type) { \
	{ type t; } \
	luastruct_new_enum(state, #type, sizeof(type)); \
}

#define LUAS_ENUM_VARIANT(state, type, variant, value) { \
	{ type t; } \
	luastruct_new_enum_variant(state, variant, value); \
}

#define LUAS_STRUCT(state, type) { \
	{ type t; } \
	luastruct_new_struct(state, #type, NULL, sizeof(type)); \
}

#define LUAS_STRUCT_EXTENDS(state, type, super_type) { \
	{ type t; } \
	{ super_type s; } \
	luastruct_new_struct(state, #type, #super_type, sizeof(type)); \
}

#define LUAS_PRIMITIVE_FIELD(state, type, field, field_type, flags) { \
	{ type t; } \
	luastruct_new_struct_field(state, SNAKE_TO_CAMEL(#field), field_type, NULL, offsetof(type, field), flags & LUAS_FIELD_POINTER, flags & LUAS_FIELD_READONLY); \
}

#define LUAS_PRIMITIVE_ARRAY_FIELD(state, type, field, elements_type, elements_flags) { \
	{ type t; } \
	LuastructArrayDesc array_desc; \
	luastruct_new_static_array_desc(state, elements_type, NULL, LUAS_SIZEOF_ARRAY(type, field), elements_flags & LUAS_FIELD_POINTER, elements_flags & LUAS_FIELD_READONLY, &array_desc); \
	luastruct_new_struct_array_field(state, SNAKE_TO_CAMEL(#field), &array_desc, offsetof(type, field), false, false); \
}

#define LUAS_PRIMITIVE_DYNAMIC_ARRAY_FIELD(state, type, field, array_size_counter, elements_type, elements_flags) { \
	{ type t; } \
	LuastructArrayDesc array_desc; \
	luastruct_new_dynamic_array_desc(state, elements_type, NULL, array_size_counter, elements_flags & LUAS_FIELD_POINTER, elements_flags & LUAS_FIELD_READONLY, &array_desc); \
	luastruct_new_struct_array_field(state, SNAKE_TO_CAMEL(#field), &array_desc, offsetof(type, field), true, false); \
}

#define LUAS_OBJREF_FIELD(state, type, field, field_type, flags) { \
	{ type t; } \
	{ field_type f; } \
	luastruct_new_struct_field(state, SNAKE_TO_CAMEL(#field), LUAST_STRUCT, #field_type, offsetof(type, field), flags & LUAS_FIELD_POINTER, flags & LUAS_FIELD_READONLY); \
}

#define LUAS_OBJREF_ARRAY_FIELD(state, type, field, elements_type, elements_flags) { \
	{ type t; } \
	LuastructArrayDesc array_desc; \
	luastruct_new_static_array_desc(state, LUAST_STRUCT, #elements_type, LUAS_SIZEOF_ARRAY(type, field), elements_flags & LUAS_FIELD_POINTER, elements_flags & LUAS_FIELD_READONLY, &array_desc); \
	luastruct_new_struct_array_field(state, SNAKE_TO_CAMEL(#field), &array_desc, offsetof(type, field), false, false); \
}

#define LUAS_OBJREF_DYNAMIC_ARRAY_FIELD(state, type, field, array_size_counter, elements_type, elements_flags) { \
	{ type t; } \
	LuastructArrayDesc array_desc; \
	luastruct_new_dynamic_array_desc(state, LUAST_STRUCT, #elements_type, array_size_counter, elements_flags & LUAS_FIELD_POINTER, elements_flags & LUAS_FIELD_READONLY, &array_desc); \
	luastruct_new_struct_array_field(state, SNAKE_TO_CAMEL(#field), &array_desc, offsetof(type, field), true, false); \
}

#define LUAS_ENUM_FIELD(state, type, field, field_type, flags) { \
	{ type t; } \
	{ field_type f; } \
	luastruct_new_struct_field(state, SNAKE_TO_CAMEL(#field), LUAST_ENUM, #field_type, offsetof(type, field), flags & LUAS_FIELD_POINTER, flags & LUAS_FIELD_READONLY); \
}

#define LUAS_METHOD_FIELD(state, type, name, method) { \
	{ type t; } \
	luastruct_new_struct_method(state, name, method); \
}

#define LUAS_PUSH_OBJECT(state, type, data, read_only) { \
	{ type t; } \
	luastruct_new_object(state, #type, (void *)data, read_only); \
}

#define LUAS_NEW_OBJECT(state, type, read_only) { \
	{ type t; } \
	luastruct_new_object(state, #type, NULL, read_only); \
}

#define LUAS_CHECK_OBJECT(state, idx, type) \
	reinterpret_cast<type *>(luastruct_check_object(state, idx, #type)) \

#define LUAS_GET_ENUM(state, type) \
	luastruct_get_enum(state, #type)

#define LUAS_PUSH_ENUM_VARIANT(state, type, variant) { \
	{ type t; } \
	LuastructEnum *enum_type = luastruct_get_enum(state, #type); \
	luastruct_get_enum_variant_by_value(state, enum_type, variant); \
}

#endif
