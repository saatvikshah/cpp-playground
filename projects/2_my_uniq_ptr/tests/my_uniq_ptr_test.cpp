#include "my_uniq_ptr.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <type_traits>

using namespace testing;

struct TestStub {
  bool& m_destroyed_;

  explicit TestStub(bool& destroyed): m_destroyed_(destroyed) {
    m_destroyed_ = false;
  }

  ~TestStub() {
    m_destroyed_ = true;
  }
};

struct TestDeleter {
  bool& called_;
  explicit TestDeleter(bool& called): called_(called) {
    called_ = false;
  }

  void operator()(int* ptr) const {
    called_ = true;
    delete ptr;
  }

};

TEST(my_uniq_ptr, HoldsUnderlyingPointer) {
  auto* elem = new int(10);
  {
    code::unique_ptr elem_ptr{elem};
    EXPECT_TRUE(elem_ptr);
    EXPECT_THAT(elem_ptr.get(), Eq(elem));
  }
}

TEST(my_uniq_ptr, DestroyedWhenOutOfScope) {
  bool check_destroyed;
  auto* elem = new TestStub(check_destroyed);
  EXPECT_THAT(check_destroyed, false);
  {
    code::unique_ptr elem_ptr{elem};
  }
  EXPECT_THAT(check_destroyed, true);
}

TEST(my_uniq_ptr, DestroyedByCustomDeleter) {
  bool check_destroyed;
  auto* elem = new int(3);
  {
    code::unique_ptr elem_ptr{elem, TestDeleter{check_destroyed}};
    EXPECT_FALSE(check_destroyed);
  }
  EXPECT_TRUE(check_destroyed);
}


TEST(my_uniq_ptr, CopyCtorDisallowed) {
  EXPECT_FALSE((std::is_copy_constructible_v<code::unique_ptr<int>>));
}

TEST(my_uniq_ptr, CopyAssignmentDisallowed) {
  EXPECT_FALSE((std::is_copy_assignable_v<code::unique_ptr<int>>));
}

TEST(my_uniq_ptr, MoveCtorMovesPointer) {
  auto* elem = new int(10);
  {
    code::unique_ptr elem_ptr{elem};
    code::unique_ptr elem_ptr2{std::move(elem_ptr)};
    EXPECT_EQ(elem_ptr.get(), nullptr);
    EXPECT_EQ(elem_ptr2.get(), elem);
  }
}

TEST(my_uniq_ptr, MoveAssignmentMovesPointer_DeletesOld) {
  bool e1destroyed;
  bool e2destroyed;
  auto* elem1 = new TestStub(e1destroyed);
  auto* elem2 = new TestStub(e2destroyed);
  {
    EXPECT_FALSE(e1destroyed);
    EXPECT_FALSE(e2destroyed);
    code::unique_ptr elem_ptr1{elem1};
    code::unique_ptr elem_ptr2{elem2};
    elem_ptr2 = std::move(elem_ptr1);
    EXPECT_FALSE(e1destroyed);
    EXPECT_TRUE(e2destroyed);
    EXPECT_EQ(elem_ptr1.get(), nullptr);
    EXPECT_EQ(elem_ptr2.get(), elem1);
  }
  EXPECT_TRUE(e1destroyed);
}

TEST(my_uniq_ptr, MoveAssignmentPreventsSelfMove) {
  bool e1destroyed;
  auto* elem1 = new TestStub(e1destroyed);
  {
    EXPECT_FALSE(e1destroyed);
    code::unique_ptr elem_ptr1{elem1};
    elem_ptr1 = std::move(elem_ptr1);
    EXPECT_FALSE(e1destroyed);
    EXPECT_EQ(elem_ptr1.get(), elem1);
  }
  EXPECT_TRUE(e1destroyed);
}

TEST(my_uniq_ptr, ResetSwitchesOwnership_DestroysOld) {
  bool e1destroyed;
  bool e2destroyed;
  auto* elem1 = new TestStub(e1destroyed);
  auto* elem2 = new TestStub(e2destroyed);
  {
    EXPECT_FALSE(e1destroyed);
    EXPECT_FALSE(e2destroyed);
    code::unique_ptr elem_ptr{elem1};
    EXPECT_EQ(elem_ptr.get(), elem1);
    elem_ptr.reset(elem2);
    EXPECT_TRUE(e1destroyed);
    EXPECT_FALSE(e2destroyed);
    EXPECT_EQ(elem_ptr.get(), elem2);
  }
  EXPECT_TRUE(e2destroyed);
}

TEST(my_uniq_ptr, DefaultDeleterSizeMatchedIntPtr) {
  auto* elem = new int(10);
  {
    code::unique_ptr elem_ptr{elem};
    EXPECT_THAT(sizeof(elem_ptr), Eq(sizeof(elem)));
  }
}
