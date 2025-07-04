// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/legacy_api/event.hpp>
#include <balltze/hook.hpp>
#include "../config/config.hpp"
#include "../logger.hpp"

namespace Balltze::Features {
    extern "C" {
        void *reserve_read_region_permutations_local_vars_size_return = nullptr;
        void *free_read_region_permutations_local_vars_size_return = nullptr;
        void reserve_read_region_permutations_local_vars_size_asm();
        void free_read_region_permutations_local_vars_size_asm();
        void read_region_permutations_param_1_read_1_asm();
        void read_region_permutations_param_1_read_2_asm();
    }

    static void extend_region_permutation_limit() {
        auto *reserve_local_vars_space_sig = Memory::get_signature("read_region_permutation_function_reserve_local_vars_space");
        auto *free_local_vars_space_sig = Memory::get_signature("read_region_permutation_function_free_local_vars_space");
        auto *param_1_read_1_sig = Memory::get_signature("read_region_permutation_function_param_1_read_1");
        auto *param_1_read_2_sig = Memory::get_signature("read_region_permutation_function_param_1_read_2");
        if(!reserve_local_vars_space_sig || !free_local_vars_space_sig || !param_1_read_1_sig || !param_1_read_2_sig) {
            logger.error("Failed to find signatures for expanding region permutation limit");
            return;
        }
        Memory::override_function(reserve_local_vars_space_sig->data(), reserve_read_region_permutations_local_vars_size_asm, &reserve_read_region_permutations_local_vars_size_return);
        Memory::override_function(free_local_vars_space_sig->data(), free_read_region_permutations_local_vars_size_asm, &free_read_region_permutations_local_vars_size_return);
        Memory::replace_function_call(param_1_read_1_sig->data(), read_region_permutations_param_1_read_1_asm);
        Memory::replace_function_call(param_1_read_2_sig->data(), read_region_permutations_param_1_read_2_asm);
    }

    void set_up_extended_limits() {
        extend_region_permutation_limit();
    }
}
