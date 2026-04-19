#include "hello_world/hello_world.hpp"

#include <format>

namespace hello_world {

std::string greet(std::string_view name) {
  return std::format("Hello, {}!", name);
}

}  // namespace hello_world
