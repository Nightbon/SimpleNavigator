#include <gtest/gtest.h>

#include "s21_list.h"

// ===== Constructors =====
TEST(ListMinimal, DefaultCtor) {
  s21::list<int> l;
  EXPECT_TRUE(l.empty());
}

TEST(ListMinimal, SizeCtorZero) {
  s21::list<int> l(0);
  EXPECT_TRUE(l.empty());
}

TEST(ListMinimal, SizeCtorThrow) {
  EXPECT_THROW(s21::list<int> l(std::numeric_limits<size_t>::max()),
               std::length_error);
}

TEST(ListMinimal, InitList) {
  s21::list<int> l{1, 2, 3};
  EXPECT_EQ(l.size(), 3);
  EXPECT_EQ(l.front(), 1);
  EXPECT_EQ(l.back(), 3);
}

TEST(ListMinimal, CopyCtor) {
  s21::list<int> a{1, 2};
  s21::list<int> b(a);
  EXPECT_EQ(b.size(), 2);
}

TEST(ListMinimal, MoveCtor) {
  s21::list<int> a{1, 2};
  s21::list<int> b(std::move(a));
  EXPECT_EQ(b.size(), 2);
  EXPECT_TRUE(a.empty());
}

// ===== Assignment =====
TEST(ListMinimal, CopyAssignSelf) {
  s21::list<int> l{1, 2};
  l = l;
  EXPECT_EQ(l.size(), 2);
}

TEST(ListMinimal, MoveAssign) {
  s21::list<int> a{1, 2};
  s21::list<int> b;
  b = std::move(a);
  EXPECT_EQ(b.size(), 2);
  EXPECT_TRUE(a.empty());
}

// ===== Access =====
TEST(ListMinimal, FrontBackThrow) {
  s21::list<int> l;
  EXPECT_THROW(l.front(), std::out_of_range);
  EXPECT_THROW(l.back(), std::out_of_range);
}

// ===== Modifiers =====
TEST(ListMinimal, PushPop) {
  s21::list<int> l;
  l.push_back(1);
  l.push_front(0);
  EXPECT_EQ(l.front(), 0);
  l.pop_front();
  l.pop_back();
  EXPECT_TRUE(l.empty());
}

TEST(ListMinimal, PopThrow) {
  s21::list<int> l;
  EXPECT_THROW(l.pop_front(), std::out_of_range);
}

TEST(ListMinimal, EraseInvalid) {
  s21::list<int> l;
  EXPECT_THROW(l.erase(l.begin()), std::out_of_range);
}

// ===== Reverse =====
TEST(ListMinimal, ReverseCases) {
  s21::list<int> l1;
  l1.reverse();
  EXPECT_TRUE(l1.empty());

  s21::list<int> l2{1};
  l2.reverse();
  EXPECT_EQ(l2.front(), 1);

  s21::list<int> l3{1, 2, 3};
  l3.reverse();
  EXPECT_EQ(l3.front(), 3);
}

// ===== Unique =====
TEST(ListMinimal, UniqueCases) {
  s21::list<int> l1;
  l1.unique();
  EXPECT_TRUE(l1.empty());

  s21::list<int> l2{1};
  l2.unique();
  EXPECT_EQ(l2.size(), 1);

  s21::list<int> l3{1, 1, 2, 2, 3};
  l3.unique();
  EXPECT_EQ(l3.size(), 3);
}

// ===== Sort =====
TEST(ListMinimal, SortCases) {
  s21::list<int> l1;
  l1.sort();

  s21::list<int> l2{3, 2, 1};
  l2.sort();
  EXPECT_EQ(l2.front(), 1);
}

// ===== Splice =====
TEST(ListMinimal, SpliceCases) {
  s21::list<int> a{1, 2};
  s21::list<int> b{3, 4};
  a.splice(a.begin(), b);
  EXPECT_EQ(a.size(), 4);
  EXPECT_TRUE(b.empty());

  a.splice(a.begin(), a);  // self
  EXPECT_EQ(a.size(), 4);
}

// ===== Merge =====
TEST(ListMinimal, MergeCases) {
  s21::list<int> a{1, 3};
  s21::list<int> b{2, 4};
  a.merge(b);
  EXPECT_EQ(a.size(), 4);
  EXPECT_TRUE(b.empty());

  a.merge(a);  // self
  EXPECT_EQ(a.size(), 4);
}

// ===== insert_many =====
TEST(ListMinimal, InsertMany) {
  s21::list<int> l{1, 4};
  auto it = ++l.begin();
  l.insert_many(it, 2, 3);
  EXPECT_EQ(l.size(), 4);

  l.insert_many(l.begin());
  EXPECT_EQ(l.size(), 4);
}
