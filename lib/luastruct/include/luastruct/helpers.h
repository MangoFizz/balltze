#ifndef LUASTRUCT_HELPERS_H
#define LUASTRUCT_HELPERS_H

#include "luastruct.h"

enum {
	LUAS_FIELD_READONLY = 0x01,
	LUAS_FIELD_POINTER = 0x02
};

#define LUAS_STRUCT_FIELD(type, field) (((struct type *)NULL)->field)
#define LUAS_SIZEOF_ARRAY(type, field) (sizeof(((struct type *)NULL)->field) / sizeof(((struct type *)NULL)->field[0]))

#define LUAS_ENUM(state, type, size) { \
	{ struct type; } \
	luastruct_new_enum(state, #type, size); \
}

#define LUAS_ENUM_VARIANT(state, type, variant) { \
	{ struct type; } \
	luastruct_new_enum_variant(state, #variant, variant); \
}

#define LUAS_STRUCT(state, type) { \
	luastruct_new_struct(state, #type, NULL, sizeof(type)); \
}

#define LUAS_STRUCT_EXTENDS(state, type, super_type) { \
	{ struct type; } \
	{ struct super_type; } \
	luastruct_new_struct(state, #type, #super_type, sizeof(struct type)); \
}

#define LUAS_PRIMITIVE_FIELD(state, type, field, field_type, flags) { \
	{ struct type; } \
	luastruct_new_struct_field(state, #field, field_type, NULL, offsetof(struct type, field), flags & LUAS_FIELD_POINTER, flags & LUAS_FIELD_READONLY); \
}

#define LUAS_PRIMITIVE_ARRAY_FIELD(state, type, field, elements_type, elements_flags) { \
	{ struct type; } \
	LuastructArrayDesc array_desc; \
	luastruct_new_static_array_desc(state, elements_type, NULL, LUAS_SIZEOF_ARRAY(type, field), elements_flags & LUAS_FIELD_POINTER, elements_flags & LUAS_FIELD_READONLY, &array_desc); \
	luastruct_new_struct_array_field(state, #field, &array_desc, offsetof(struct type, field), false, false); \
}

#define LUAS_PRIMITIVE_DYNAMIC_ARRAY_FIELD(state, type, field, array_size_counter, elements_type, elements_flags) { \
	{ struct type; } \
	LuastructArrayDesc array_desc; \
	luastruct_new_dynamic_array_desc(state, elements_type, NULL, array_size_counter, elements_flags & LUAS_FIELD_POINTER, elements_flags & LUAS_FIELD_READONLY, &array_desc); \
	luastruct_new_struct_array_field(state, #field, &array_desc, offsetof(struct type, field), true, false); \
}

#define LUAS_OBJREF_FIELD(state, type, field, field_type, flags) { \
	{ struct type; } \
	{ struct field_type; } \
	luastruct_new_struct_field(state, #field, LUAST_STRUCT, #field_type, offsetof(struct type, field), flags & LUAS_FIELD_POINTER, flags & LUAS_FIELD_READONLY); \
}

#define LUAS_OBJREF_ARRAY_FIELD(state, type, field, elements_type, elements_flags) { \
	{ struct type; } \
	LuastructArrayDesc array_desc; \
	luastruct_new_static_array_desc(state, LUAST_STRUCT, #elements_type, LUAS_SIZEOF_ARRAY(type, field), elements_flags & LUAS_FIELD_POINTER, elements_flags & LUAS_FIELD_READONLY, &array_desc); \
	luastruct_new_struct_array_field(state, #field, &array_desc, offsetof(struct type, field), false, false); \
}

#define LUAS_OBJREF_DYNAMIC_ARRAY_FIELD(state, type, field, array_size_counter, elements_type, elements_flags) { \
	{ struct type; } \
	LuastructArrayDesc array_desc; \
	luastruct_new_dynamic_array_desc(state, LUAST_STRUCT, #elements_type, array_size_counter, elements_flags & LUAS_FIELD_POINTER, elements_flags & LUAS_FIELD_READONLY, &array_desc); \
	luastruct_new_struct_array_field(state, #field, &array_desc, offsetof(struct type, field), true, false); \
}

#define LUAS_ENUM_FIELD(state, type, field, field_type, flags) { \
	{ struct type; } \
	{ struct field_type; } \
	luastruct_new_struct_field(state, #field, LUAST_ENUM, #field_type, offsetof(struct type, field), flags & LUAS_FIELD_POINTER, flags & LUAS_FIELD_READONLY); \
}

#define LUAS_METHOD_FIELD(state, type, name, method) { \
	{ struct type; } \
	luastruct_new_struct_method(state, name, method); \
}

#define LUAS_PUSH_OBJECT(state, type, data, read_only) { \
	{ struct type; } \
	luastruct_new_object(state, #type, (void *)data, read_only); \
}

#define LUAS_CHECK_OBJECT(state, idx, type) \
	luastruct_check_object(state, idx, #type); \

#endif
