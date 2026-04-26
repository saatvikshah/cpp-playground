#pragma once

#include <algorithm>

/**
 * TODO:
 * - make_unique
 * - Array specialization
 * - operator*, operator->
 * - Converting move support
 * - Benchmark
 *
 * Lessons:
 * - `delete <x>` does not need a check if it is nullptr or not
 * - Templated deleter type: functor providing an operator()(T*), useful for file closing as an example.
 * - Testing: type trait checks for compile-time behavior, destruction checked with stubs
 */
namespace code {
template <typename T, typename Deleter = std::default_delete<T>>
struct unique_ptr {

    explicit unique_ptr(T* ptr): ptr_(ptr) {}
    explicit unique_ptr(T* ptr, Deleter deleter): ptr_(ptr), deleter_(deleter) {}

    T* release() {
        return std::exchange(ptr_ , nullptr);
    }

    void reset(T* ptr) {
        if (ptr_ == ptr) {
            return;
        }
        T* old = std::exchange(ptr_, ptr);
        deleter_(old);
    }

    T* get() {
        return ptr_;
    }

    explicit unique_ptr(unique_ptr&& other) noexcept
    {
        std::swap(ptr_, other.ptr_);
    }

    operator bool() const noexcept
    {
        return ptr_ != nullptr;
    }

    T& operator *()
    {
        return *ptr_;
    }

    T* operator ->()
    {
        return ptr_;
    }

    unique_ptr& operator=(unique_ptr&& other) noexcept
    {
        if (this == &other) {
            return *this;
        }
        T* old = std::exchange(ptr_, other.ptr_);
        deleter_(old);
        other.ptr_ = nullptr;
        std::swap(deleter_, other.deleter_);
        return *this;
    }

    unique_ptr(const unique_ptr& other) = delete;
    unique_ptr& operator=(const unique_ptr& other) = delete;

    ~unique_ptr() {
        deleter_(ptr_);
    }

private:
    T* ptr_{nullptr};
    Deleter deleter_;
};
}  // namespace my_uniq_ptr
