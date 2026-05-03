#pragma once

#include <functional>
#include <vector>

namespace code {

class event_loop {
public:
    void run_for(std::chrono::microseconds microseconds) {
        auto end = std::chrono::high_resolution_clock::now() + microseconds;
        while (true) {
            if (auto time_now = std::chrono::high_resolution_clock::now(); time_now >= end) {
                return;
            }
            for (auto& runnable: runnables_) {
                runnable();
            }
            runnables_ = std::exchange(posted_runnables_, {});
        }
    }

    void post(std::function<void()> runnable) {
        posted_runnables_.emplace_back(std::move(runnable));
    }

private:
    std::vector<std::function<void()>> runnables_;
    std::vector<std::function<void()>> posted_runnables_;
};

}  // namespace code
