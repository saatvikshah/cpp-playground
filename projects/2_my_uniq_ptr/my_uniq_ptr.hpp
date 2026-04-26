#pragma once

#include <algorithm>

/**
 * TODO:
 * - Custom deleter
 * - release/reset/swap
 * - make_unique
 * - Array specialization
 * - operator*, operator->, operator bool
 * - get() const correctness
 * - Converting move support
 * - Benchmark
 */
namespace code {
template <typename T>
struct unique_ptr {

    explicit unique_ptr(T* ptr): ptr_(ptr) {}

    T* get() {
        return ptr_;
    }

    explicit unique_ptr(unique_ptr&& other) noexcept
    {
        std::swap(ptr_, other.ptr_);
    }

    unique_ptr& operator=(unique_ptr&& other) noexcept
    {
        if (this == &other) {
            return *this;
        }
        delete ptr_;
        ptr_ = nullptr;
        std::swap(ptr_, other.ptr_);
        return *this;
    }

    unique_ptr(const unique_ptr& other) = delete;
    unique_ptr& operator=(const unique_ptr& other) = delete;

    ~unique_ptr() {
        delete ptr_;
    }

private:
    T* ptr_{nullptr};
};
}  // namespace my_uniq_ptr
