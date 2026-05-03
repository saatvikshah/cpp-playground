#include "event_loop.hpp"
#include <chrono>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace std::chrono_literals;
using namespace ::testing;

TEST(event_loop, EventLoopSanity) {
  code::event_loop ev;
  ev.post([&] {
    std::cout << "hello world!" << std::endl;
    ev.post([] {
      std::cout << "added after run_for!" << std::endl;
    });
  });
  ev.run_for(100ms);
}

TEST(event_loop, FIFOOrdering) {
  code::event_loop ev;
  std::vector<int> order;
  ev.post([&] { order.push_back(1); });
  ev.post([&] { order.push_back(2); });
  ev.post([&] { order.push_back(3); });
  ev.run_for(100ms);
  EXPECT_THAT(order, ElementsAre(1, 2, 3));
}

TEST(event_loop, DeferredTaskOrdering) {
  code::event_loop ev;
  std::vector<int> order;
  ev.post([&] { order.push_back(1); ev.post([&] { order.push_back(3); }); });
  ev.post([&] { order.push_back(2); ev.post([&] { order.push_back(4); }); });
  ev.run_for(100ms);
  EXPECT_THAT(order, ElementsAre(1, 2, 3, 4));
}

TEST(event_loop, DeeplyNestedPosting) {
  code::event_loop ev;
  std::vector<int> order;
  ev.post([&] { order.push_back(1); ev.post([&] { order.push_back(2); ev.post([&] { order.push_back(3); }); }); });
  ev.run_for(100ms);
  EXPECT_THAT(order, ElementsAre(1, 2, 3));
}
