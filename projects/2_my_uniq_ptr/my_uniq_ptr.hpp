#pragma once

#include <memory>
#include <utility>
#include "deleter_storage.hpp"

/**
 * TODO:
 * - Array specialization
 * - Converting move support
 * - Benchmark (complete)
 *
 * Lessons:
 * - `delete <x>` does not need a check if it is nullptr or not for default_delete / recommended for others
 * - Templated deleter type:
 *   - functor providing an operator()(T*), useful for file closing as an example.
 *   - Can use EBO to avoid space of deleter (sizeof(unique_ptr<int>) == sizeof(int*))
 * - Testing: type trait checks for compile-time behavior, destruction checked with stubs
 * - Benchmarking lessons(!)
 */
namespace code {
template <typename T, typename DeleterT = std::default_delete<T>>
struct unique_ptr: private deleter_storage_t<DeleterT> {
    using BaseT = deleter_storage_t<DeleterT>;

    explicit unique_ptr(T* ptr): ptr_(ptr) {}
    explicit unique_ptr(T* ptr, DeleterT deleter): ptr_(ptr), BaseT(std::move(deleter)) {}

    T* release() {
        return std::exchange(ptr_ , nullptr);
    }

    void reset(T* ptr) {
        if (ptr_ == ptr) {
            return;
        }
        T* old = std::exchange(ptr_, ptr);
        this->get_deleter()(old);
    }

    T* get() {
        return ptr_;
    }

    explicit unique_ptr(unique_ptr&& other) noexcept
    : BaseT(std::move(other.get_deleter())),
      ptr_(other.release())
    {
    }

    operator bool() const noexcept
    {
        return ptr_ != nullptr;
    }

    T& operator*()
    {
        return *ptr_;
    }

    T* operator->()
    {
        return ptr_;
    }

    unique_ptr& operator=(unique_ptr&& other) noexcept
    {
        if (this == &other) {
            return *this;
        }
        T* old = std::exchange(ptr_, other.ptr_);
        this->get_deleter()(old);
        other.ptr_ = nullptr;
        this->get_deleter() = std::move(other.get_deleter());
        // std::swap(this->get_deleter(), other.get_deleter());
        return *this;
    }

    unique_ptr(const unique_ptr& other) = delete;
    unique_ptr& operator=(const unique_ptr& other) = delete;

    ~unique_ptr() {
        this->get_deleter()(ptr_);
    }

private:
    T* ptr_{nullptr};
};

template <typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args)
{
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}
}  // namespace my_uniq_ptr
