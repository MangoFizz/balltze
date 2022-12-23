// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_HPP
#define BALLTZE_HPP

#include <memory>

namespace Balltze {
    class Ini;

    class Balltze {
        public:
            /**
             * Get the Chimera INI
             */
            Ini &chimera_ini() noexcept;

            /**
             * Constructor for Balltze
             */
            Balltze() noexcept;

            /**
             * Get balltze instance
             */
            static Balltze &get() noexcept;

        private:
            /** Chimera ini file */
            std::unique_ptr<Ini> m_chimera_ini;

            /** Balltze instance */
            static Balltze *m_instance;
    };
}

#endif
