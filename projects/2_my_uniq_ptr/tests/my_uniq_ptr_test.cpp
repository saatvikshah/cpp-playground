#include "my_uniq_ptr.hpp"

#include <gtest/gtest.h>

TEST(my_uniq_ptr, AnswerIs42) {
  EXPECT_EQ(code::answer(), 42);
}
