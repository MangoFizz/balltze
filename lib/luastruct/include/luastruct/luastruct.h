#ifndef LUASTRUCT_H
#define LUASTRUCT_H

#ifndef __cplusplus
#include <lua.h>
#else
#include <lua.hpp>
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define LUASTRUCT_TYPENAME_LENGTH 64

static const char *STRUCT_METATABLE_NAME = "luastruct_struct";

typedef enum LuastructType {
	LUAST_STRUCT,
	LUAST_ENUM,
	LUAST_INT8,
	LUAST_INT16,
	LUAST_INT32,
	LUAST_INT64,
	LUAST_UINT8,
	LUAST_UINT16,
	LUAST_UINT32,
	LUAST_UINT64,
	LUAST_FLOAT,
	LUAST_BOOL,
	LUAST_STRING,
	LUAST_BITFIELD,
	LUAST_EXTREF,
	LUAST_OBJREF,
	LUAST_ARRAY,
	LUAST_METHOD
} LuastructType;

typedef struct LuastructTypeInfo {
	LuastructType type;
	char name[LUASTRUCT_TYPENAME_LENGTH];
} LuastructTypeInfo;

typedef struct LuastructArrayDesc {
	/** 
	 * A function that can count the elements in the array.
	 * This also implies that the array is dynamic and the 
	 * way to access the elements is through a pointer.
	 */
	lua_CFunction count_getter; 
	/**
	 * If the count_getter is NULL then the array is static
	 * and this field will be used to determine the size 
	 * of the array.
	 */
	size_t array_size; 
	/**
	 * The size of each element in the array.
	 * We need this to calculate the offset of each element.
	 */
	size_t elements_size;
	LuastructType elements_type;
	void *elements_type_info;
	bool elements_are_pointers;
	bool elements_are_readonly;
} LuastructArrayDesc;

typedef struct LuastructStructField {
	char field_name[LUASTRUCT_TYPENAME_LENGTH];
	LuastructType type;
	void *type_info;
	uint32_t offset;
	bool pointer;
	bool readonly;
	struct LuastructStructField *next_by_offset;
	struct LuastructStructField *next_by_name;
	union {
		struct {
			uint8_t size;
			uint8_t offset;
		} bitfield;
		LuastructArrayDesc array;
	};
} LuastructStructField;

typedef struct LuastructStruct {
	LuastructTypeInfo type_info;
	struct LuastructStruct *super;
	size_t size;
	LuastructStructField *fields;
	LuastructStructField *fields_by_name;
} LuastructStruct;

typedef enum LuastructEnumSize {
	LUAS_ENUM_INT8,
	LUAS_ENUM_INT16,
	LUAS_ENUM_INT32
} LuastructEnumSize;

typedef struct LuastructEnumVariant {
	char name[LUASTRUCT_TYPENAME_LENGTH];
	intmax_t value;
	struct LuastructEnumVariant *next;
} LuastructEnumVariant;

typedef struct LuastructEnum {
	LuastructTypeInfo type_info;
	LuastructEnumSize size;
	intmax_t max_value;
	LuastructEnumVariant *variants;
} LuastructEnum;

typedef struct LuastructStructObject {
	void *type;
	void *data;
	bool readonly;
	bool invalid;
	bool delete_on_gc;
} LuastructStructObject;

typedef struct LuastructArray {
	void *data;
	LuastructArrayDesc *array_info;
} LuastructArray;

/**
 * Get the types registry.
 * @param state Lua state.
 */
int luastruct_get_types_registry(lua_State *state);

/**
 * Create a new enum type.
 * @param state Lua state.
 * @param name Name of the enum type.
 * @param size Size of the enum type (LUAS_ENUM_INT8, LUAS_ENUM_INT16, LUAS_ENUM_INT32).
 * @return the number of values pushed onto the stack.
 */
int luastruct_new_enum(lua_State *state, const char *name, size_t size);

/**
 * Create a new enum variant.
 * @param state Lua state.
 * @param name Name of the enum variant.
 * @param value Value of the enum variant.
 * @return the number of values pushed onto the stack.
 */
int luastruct_new_enum_variant(lua_State *state, const char *name, int32_t value);

/**
 * Create a new struct type.
 * @param state Lua state.
 * @param name Name of the struct type.
 * @param super_name Name of the super struct type.
 * @param size Size of the struct.
 * @return the number of values pushed onto the stack.
 */
int luastruct_new_struct(lua_State *state, const char *name, const char *super_name, uint32_t size);

/**
 * Check if the object at the given index is a struct.
 * @param state Lua state.
 * @param index Index of the object.
 * @return The struct object or NULL if the object is not a struct.
 */
LuastructStruct *luastruct_check_struct(lua_State *state, int index);

/**
 * Create a new field in a struct.
 * @param state Lua state.
 * @param name Name of the field.
 * @param type Type of the field.
 * @param type_name Name of the type of the field.
 * @param offset Offset of the field in the struct.
 * @param pointer Whether the field is a pointer.
 * @param readonly Whether the field is read-only.
 */
void luastruct_new_struct_field(lua_State *state, const char *name, LuastructType type, const char *type_name, uint32_t offset, bool pointer, bool readonly);

/**
 * Create a new bit field in a struct.
 * @param state Lua state.
 * @param name Name of the field.
 * @param type Type of the field.
 * @param offset Offset of the field in the struct.
 * @param bit_offset Bit offset of the field in the struct.
 * @param pointer Whether the field is a pointer.
 * @param readonly Whether the field is read-only.
 */
void luastruct_new_struct_bit_field(lua_State *state, const char *name, LuastructType type, uint32_t offset, uint32_t bit_offset, bool pointer, bool readonly);

/**
 * Creates a new dynamic array descriptor.
 * @param state The Lua state.
 * @param type The type of the elements in the array.
 * @param type_name The name of the type of the elements in the array.
 * @param count_getter A Lua C function to retrieve the count of elements in the array.
 * @param elements_are_pointers Whether the elements in the array are pointers.
 * @param readonly Whether the array is read-only.
 * @param desc A pointer to the LuastructArrayDesc structure to populate with the array descriptor.
 */
void luastruct_new_dynamic_array_desc(lua_State *state, LuastructType type, const char *type_name, lua_CFunction count_getter, bool elements_are_pointers, bool readonly, LuastructArrayDesc *desc);

/**
 * Creates a new static array descriptor.
 * @param state The Lua state.
 * @param type The type of the elements in the array.
 * @param type_name The name of the type of the elements in the array.
 * @param size The fixed size of the array.
 * @param elements_are_pointers Whether the elements in the array are pointers.
 * @param readonly Whether the array is read-only.
 * @param desc A pointer to the LuastructArrayDesc structure to populate with the array descriptor.
 */
void luastruct_new_static_array_desc(lua_State *state, LuastructType type, const char *type_name, size_t size, bool elements_are_pointers, bool readonly, LuastructArrayDesc *desc);

/**
 * Creates a new struct field that represents an array in a Lua structure.
 * @param state The Lua state.
 * @param name The name of the field.
 * @param array_info A pointer to the LuastructArrayDesc structure describing the array.
 * @param offset The offset of the field within the structure.
 * @param pointer Whether the field is a pointer to the array.
 * @param readonly Whether the field is read-only.
 */
void luastruct_new_struct_array_field(lua_State *state, const char *name, LuastructArrayDesc *array_info, uint32_t offset, bool pointer, bool readonly);

/**
 * Create a new object.
 * @param state Lua state.
 * @param type_name Name of the type of the object.
 * @param data Pointer to the data of the object.
 * @param readonly Whether the object is read-only.
 * @return The number of values pushed onto the stack.
 */
int luastruct_new_object(lua_State *state, const char *type_name, void *data, bool readonly);

#ifdef __cplusplus
}
#endif

#endif 
