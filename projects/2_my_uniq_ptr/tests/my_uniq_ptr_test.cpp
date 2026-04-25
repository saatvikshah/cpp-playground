#include "my_uniq_ptr.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

struct TestStub {
  inline static bool m_destroyed_ = true;

  static bool is_destroyed() {
    return m_destroyed_;
  }

  TestStub() {
    m_destroyed_ = false;
  }

  ~TestStub() {
    m_destroyed_ = true;
  }
};

TEST(my_uniq_ptr, DestroyedWhenOutOfScope) {
  auto* elem = new TestStub();
  EXPECT_THAT(elem->is_destroyed(), false);
  {
    code::unique_ptr elem_ptr{elem};
  }
  EXPECT_THAT(elem->is_destroyed(), true);
  // EXPECT_N
}
