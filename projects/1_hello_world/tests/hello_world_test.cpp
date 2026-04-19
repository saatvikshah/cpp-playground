#include "hello_world/hello_world.hpp"

#include <gtest/gtest.h>

TEST(HelloWorld, GreetsNamedCaller) {
  EXPECT_EQ(hello_world::greet("world"), "Hello, world!");
}

TEST(HelloWorld, HandlesEmptyName) {
  EXPECT_EQ(hello_world::greet(""), "Hello, !");
}
