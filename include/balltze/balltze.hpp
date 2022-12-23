// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_HPP
#define BALLTZE_HPP

#include <memory>
#include <balltze/event/event.hpp>
#include <balltze/memory/signature.hpp>

namespace Balltze {
    class Ini;

    class Balltze {
        public:
            /**
             * Get the signature manager
             */
            Memory::SignatureManager &signature_manager() noexcept;

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
            /** Signature manager */
            Memory::SignatureManager m_signature_manager;

            /** Chimera ini file */
            std::unique_ptr<Ini> m_chimera_ini;

            /** Balltze instance */
            static Balltze *m_instance;

            /** Balltze instance */
            static void first_tick(TickEvent const &context);
    };
}

#endif
