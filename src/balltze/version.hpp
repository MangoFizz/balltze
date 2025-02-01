#ifndef BALLTZE_VERSION
#define BALLTZE_VERSION "1.3.3"
#ifdef __cplusplus

#include <semver.hpp>

namespace Balltze {
    constexpr semver::version balltze_version{BALLTZE_VERSION};
}

#endif
#endif
