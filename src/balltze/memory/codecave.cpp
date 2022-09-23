// SPDX-License-Identifier: GPL-3.0-only

#include <windows.h>
#include <balltze/memory/codecave.hpp>

namespace Balltze::Memory {
    
    std::byte &Codecave::get_top() const noexcept {
        return this->data[this->top - 1];
    }

    std::byte *Codecave::get_data() const noexcept {
        return this->data.get();
    }

    bool Codecave::empty() const noexcept {
        return this->top == 0;
    }

    void Codecave::lock() noexcept {
        this->locked = true;
    }

    void Codecave::unlock() noexcept {
        this->locked = false;
    }

    void Codecave::set_access_protection(bool setting) noexcept {
        if(setting) {
            // enable code execution
            VirtualProtect(this->data.get(), this->top, PAGE_EXECUTE_READWRITE, &this->original_page_protection);
        }
        else {
            VirtualProtect(this->data.get(), this->top, this->original_page_protection, NULL);
        }
    }

    Codecave::Codecave() noexcept {
        this->data = std::make_unique<std::byte[]>(DEFAULT_CAVE_SIZE);
    }
}