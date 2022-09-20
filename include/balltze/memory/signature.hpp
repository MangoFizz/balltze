// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__MEMORY__SIGNATURE_HPP
#define BALLTZE__MEMORY__SIGNATURE_HPP

#include <windows.h>
#include <string>
#include <vector>
#include <cstddef>

namespace Balltze::Memory {
    class Signature {
        public:
            /**
             * Get the signature name
             */
            std::string name() const noexcept;

            /**
             * Get the signature data
             */
            std::byte *data() const noexcept;

            /**
             * Restore the signature original data
             */
            void restore() noexcept;

            /**
             * Constructor for Signature
             * @param name      Name for signature
             * @param signature Signature bytes
             * @param lenght    Number of bytes
             */
            Signature(const char *name, const short *signature, std::size_t lenght, std::uint16_t offset);

        private:
            /** Signature name */
            std::string m_name;

            /** Pointer to the signature data */
            std::byte *m_data;

            /** Original signature data */
            std::vector<std::byte> m_original_data;
    };

    class SignatureManager {
        public:
            /**
             * Get the signature with the given name
             * @param name Name of the signature
             */
            Signature *get(std::string name) noexcept;

            /**
             * Load all signatures 
             */
            void load();

        private:
            /** Signatures */
            std::vector<Signature> m_signatures;

            /**
             * Find core signatures
             * This is needed to load engine-specific signatures
             */
            void find_core_signatures();

            /**
             * Find current engine signatures
             */
            void find_engine_signatures();
    };
}

#endif
