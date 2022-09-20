// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_HPP
#define BALLTZE_HPP

#include <balltze/memory/signature.hpp>

namespace Balltze {
    class Balltze {
        public:
            /**
             * Get the signature manager
             */
            Memory::SignatureManager &signature_manager() noexcept;

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

            /** Balltze instance */
            static Balltze *m_instance;
    };
}

#endif
