#ifndef BALLTZE_VERSION
#define BALLTZE_VERSION "1.0.0-rc.1"
#ifdef __cplusplus

#include <semver.hpp>

namespace Balltze {
    constexpr semver::version balltze_version{BALLTZE_VERSION};
}

#endif
#endif
