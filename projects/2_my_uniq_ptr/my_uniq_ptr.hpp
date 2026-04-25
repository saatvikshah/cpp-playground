#pragma once

namespace code {
template <typename T>
struct unique_ptr {

    explicit unique_ptr(T* ptr): ptr_(ptr) {}

    explicit unique_ptr(unique_ptr&& other) noexcept
    {
        ptr_ = other.ptr_;
    }

    unique_ptr& operator=(unique_ptr&& other) noexcept
    {
        if (this == &other) {
            return *this;
        }
        T* old = ptr_;
        ptr_ = other.ptr_;
        other.ptr_ = nullptr;
        delete old;
        return *this;
    }

    unique_ptr(const unique_ptr& other) = delete;
    unique_ptr& operator=(const unique_ptr& other) = delete;

    ~unique_ptr() {
        delete ptr_;
    }

private:
    T* ptr_;
};

}  // namespace my_uniq_ptr
