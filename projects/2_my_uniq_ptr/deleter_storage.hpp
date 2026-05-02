#pragma once

#include <type_traits>
#include <utility>

namespace code {
    template <typename DeleterT, bool UseEboV>
struct deleter_storage;

    template <typename DeleterT>
    struct deleter_storage<DeleterT, true>: private DeleterT{
        deleter_storage() = default;

        explicit deleter_storage(DeleterT deleter)
            : DeleterT(std::move(deleter)) {}

        DeleterT& get_deleter() {
            return *this;
        }

        const DeleterT& get_deleter() const {
            return *this;
        }
    };

    template <typename DeleterT>
    struct deleter_storage<DeleterT, false>{
        deleter_storage() = default;

        explicit deleter_storage(DeleterT deleter)
            : deleter_(std::move(deleter)) {}

        DeleterT& get_deleter() {
            return deleter_;
        }

        const DeleterT& get_deleter() const {
            return deleter_;
        }
    private:
        DeleterT deleter_;
    };

template <typename DeleterT>
using deleter_storage_t =
    deleter_storage<
        DeleterT,
        std::is_empty_v<DeleterT> && !std::is_final_v<DeleterT>
    >;
}
