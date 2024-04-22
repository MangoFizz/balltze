// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__FEATURES__TAGS_HANDLING_HPP
#define BALLTZE__FEATURES__TAGS_HANDLING_HPP

#include <optional>
#include <functional>
#include <cstdint>
#include <balltze/engine/tag.hpp>
#include <balltze/features/tags_handling.hpp>

namespace Balltze::Features {
    using resolve_tag_dependency_t = std::function<Engine::TagHandle(Engine::TagHandle)>;
    using resolve_external_tag_data_t = std::function<std::uint32_t(std::uint32_t)>;
    using allocate_tag_data_t = std::function<std::byte *(std::byte *, std::size_t)>;

    /**
     * Fix tag offsets with a new data address 
     * @param tag                           Tag to fix offsets
     * @param new_tag_data_address          New data address for tag data
     * @param external_data_offset_resolver Function to resolve external data offsets
     */
    void rebase_tag_data_offsets(Engine::Tag *tag, std::byte *new_tag_data_address, std::optional<resolve_external_tag_data_t> external_data_offset_resolver = {});

    /**
     * Fix tag dependencies
     * @param tag                   Tag to fix dependencies
     * @param dependency_resolver   A function that resolves tag handles from tag dependencies
     */
    void resolve_tag_dependencies(Engine::Tag *tag, resolve_tag_dependency_t dependency_resolver);

    /**
     * Copy tag data to a new location
     * @param  tag              Tag to copy data from
     * @param  data_allocator   A function that allocates memory for the copied structures from the original data and returns a pointer to the reserved memory
     * @return                  Pointer to copied data
     */
    std::byte *copy_tag_data(Engine::Tag *tag, allocate_tag_data_t data_allocator);
}

#endif
