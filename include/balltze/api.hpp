// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API_HPP
#define BALLTZE_API_HPP

#ifdef BALLTZE_EXPORTS
#define BALLTZE_API __declspec(dllexport)
#else
#define BALLTZE_API __declspec(dllimport)
#endif

namespace Balltze {
    enum BalltzeSide {
        BALLTZE_SIDE_CLIENT,
        BALLTZE_SIDE_DEDICATED_SERVER,
    };

    /**
     * Get whether the game is running on the client or the dedicated server
     */
    BALLTZE_API BalltzeSide get_balltze_side() noexcept;
}

#endif

