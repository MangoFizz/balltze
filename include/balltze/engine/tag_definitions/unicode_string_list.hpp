#ifndef BALLTZE__ENGINE__TAG_DEFINITIONS__UNICODE_STRING_LIST_HPP
#define BALLTZE__ENGINE__TAG_DEFINITIONS__UNICODE_STRING_LIST_HPP

#include <cstdint>
#include "types.hpp"

namespace Balltze::Engine { 
	struct UnicodeStringListString {
		TagDataOffset string;
	};
	struct UnicodeStringList {
		TagReflexive<UnicodeStringListString> strings;
	};
}

#endif
