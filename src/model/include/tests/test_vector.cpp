#include <iostream>
#include <vector>

#include "gtest/gtest.h"
#include "s21_vector.h"

template <typename value_type>
bool compare_vectors(s21::vector<value_type> &v1, std::vector<value_type> &v2) {
  if (v1.size() != v2.size()) {
    std::cout << "v1.size = " << v1.size() << " v2.size = " << v2.size()
              << '\n';
    return false;
  }
  if (v1.capacity() != v2.capacity()) {
    std::cout << "v1.capacity = " << v1.capacity()
              << " v2.capacity = " << v2.capacity() << '\n';
    return false;
  }
  for (size_t i = 0; i < v1.size(); i++) {
    if (v1[i] != v2[i]) {
      std::cout << "v1[" << i << "] = " << v1[i] << " v2[" << i
                << "] = " << v2[i] << '\n';
      return false;
    }
  }
  return true;
}

TEST(TestVector, Constructor_1) {
  s21::vector<int> vector1;
  std::vector<int> vector2;
  EXPECT_TRUE(compare_vectors(vector1, vector2));
}

TEST(TestVector, Constructor_2) {
  s21::vector<long double> vector1;
  std::vector<long double> vector2;
  EXPECT_TRUE(compare_vectors(vector1, vector2));
}

TEST(TestVector, Constructor_3) {
  s21::vector<char> vector1;
  std::vector<char> vector2;
  EXPECT_TRUE(compare_vectors(vector1, vector2));
}

TEST(TestVector, Constructor_4) {
  s21::vector<int> vector1(1);
  std::vector<int> vector2(1);
  EXPECT_TRUE(compare_vectors(vector1, vector2));
}

TEST(TestVector, Constructor_5) {
  s21::vector<long double> vector1(35);
  std::vector<long double> vector2(35);
  EXPECT_TRUE(compare_vectors(vector1, vector2));
}

TEST(TestVector, Constructor_6) {
  s21::vector<char> vector1(0);
  std::vector<char> vector2(0);
  EXPECT_TRUE(compare_vectors(vector1, vector2));
}

TEST(TestVector, Constructor_7) {
  s21::vector<int> vector1 = {1, 2, 3, 4, 5, 6};
  std::vector<int> vector2 = {1, 2, 3, 4, 5, 6};
  EXPECT_TRUE(compare_vectors(vector1, vector2));
}

TEST(TestVector, Constructor_8) {
  s21::vector<long double> vector1 = {1.1, -222.22, 0.123};
  std::vector<long double> vector2 = {1.1, -222.22, 0.123};
  EXPECT_TRUE(compare_vectors(vector1, vector2));
}

TEST(TestVector, Constructor_9) {
  s21::vector<char> vector1 = {'A', ' ', '(', 'r'};
  std::vector<char> vector2 = {'A', ' ', '(', 'r'};
  EXPECT_TRUE(compare_vectors(vector1, vector2));
}

TEST(TestVector, Constructor_10) {
  s21::vector<int> vector1{1, 2, 3, 4, 5, 6};
  s21::vector<int> vector2(vector1);
  std::vector<int> vector3{1, 2, 3, 4, 5, 6};
  std::vector<int> vector4(vector3);
  EXPECT_TRUE(compare_vectors(vector2, vector4));
}

TEST(TestVector, Constructor_11) {
  s21::vector<int> vector1(0);
  s21::vector<int> vector2(vector1);
  std::vector<int> vector3(0);
  std::vector<int> vector4(vector3);
  EXPECT_TRUE(compare_vectors(vector2, vector4));
}

TEST(TestVector, Constructor_12) {
  s21::vector<int> vector1{1, 2, 3, 4, 5, 6};
  s21::vector<int> vector2(std::move(vector1));
  std::vector<int> vector3{1, 2, 3, 4, 5, 6};
  std::vector<int> vector4(std::move(vector3));
  EXPECT_TRUE(compare_vectors(vector2, vector4));
}

TEST(TestVector, Constructor_13) {
  s21::vector<int> vector1(0);
  s21::vector<int> vector2(std::move(vector1));
  std::vector<int> vector3(0);
  std::vector<int> vector4(std::move(vector3));
  EXPECT_TRUE(compare_vectors(vector2, vector4));
}

TEST(TestVector, Assignment_1) {
  s21::vector<long double> vector1 = {1.1, -222.22, 0.123};
  s21::vector<long double> vector2;
  std::vector<long double> vector3 = {1.1, -222.22, 0.123};
  std::vector<long double> vector4;
  vector2 = std::move(vector1);
  vector4 = std::move(vector3);
  EXPECT_TRUE(compare_vectors(vector2, vector4));
}

TEST(TestVector, Assignment_2) {
  s21::vector<long double> vector1 = {1.1, -222.22, 0.123};
  std::vector<long double> vector2 = {1.1, -222.22, 0.123};
  EXPECT_TRUE(compare_vectors(vector1, vector2));
}

TEST(TestVector, Assignment_3) {
  const s21::vector<long double> vector1 = {1.1, -222.22, 0.123};
  const std::vector<long double> vector2 = {1.1, -222.22, 0.123};
  s21::vector<long double> vector3;
  std::vector<long double> vector4;
  vector3 = vector1;
  vector4 = vector2;
  EXPECT_TRUE(compare_vectors(vector3, vector4));
}

TEST(TestVector, At_1) {
  s21::vector<int> vector1{1, 2, 3, 4, 5, 6};
  EXPECT_THROW(vector1.at(6),
               std::out_of_range);  // заменили operator[] на at()
}

TEST(TestVector, At_2) {
  const s21::vector<int> vector1{1, 2, 3, 4, 5, 6};
  EXPECT_EQ(vector1[5], 6);
}

TEST(TestVector, Front_1) {
  s21::vector<int> vector1;
  EXPECT_THROW(vector1.front(), std::logic_error);
}

TEST(TestVector, Front_2) {
  s21::vector<long double> vector1 = {1.1, -222.22, 0.123};
  std::vector<long double> vector2 = {1.1, -222.22, 0.123};
  EXPECT_EQ(vector1.front(), vector2.front());
}

TEST(TestVector, Back_1) {
  s21::vector<int> vector1;
  EXPECT_THROW(vector1.back(), std::logic_error);
}

TEST(TestVector, Back_2) {
  s21::vector<long double> vector1 = {1.1, -222.22, 0.123};
  std::vector<long double> vector2 = {1.1, -222.22, 0.123};
  EXPECT_EQ(vector1.back(), vector2.back());
}

TEST(TestVector, Data_1) {
  s21::vector<long double> vector1 = {1.1, -222.22, 0.123};
  std::vector<long double> vector2 = {1.1, -222.22, 0.123};
  EXPECT_EQ(*vector1.data(), *vector2.data());
}

TEST(TestVector, Empty_1) {
  s21::vector<long double> vector1 = {1.1, -222.22, 0.123};
  EXPECT_FALSE(vector1.empty());
}

TEST(TestVector, Empty_2) {
  s21::vector<long double> vector1;
  EXPECT_TRUE(vector1.empty());
}

TEST(TestVector, MaxSize_1) {
  s21::vector<int> vector1 = {1, 2, 3};
  EXPECT_GT(vector1.max_size(), 0);
}

TEST(TestVector, Reserve_1) {
  s21::vector<int> vector1 = {1, 2, 3};
  std::vector<int> vector2 = {1, 2, 3};
  vector1.reserve(50);
  vector2.reserve(50);
  EXPECT_TRUE(compare_vectors(vector1, vector2));
}

TEST(TestVector, Reserve_2) {
  s21::vector<int> vector1 = {1, 2, 3, 4, 5, 6};
  std::vector<int> vector2 = {1, 2, 3, 4, 5, 6};
  vector1.reserve(1);
  vector2.reserve(1);
  EXPECT_TRUE(compare_vectors(vector1, vector2));
}

TEST(TestVector, Shrink_1) {
  s21::vector<int> vector1 = {1, 2, 3};
  std::vector<int> vector2 = {1, 2, 3};
  vector1.reserve(50);
  vector2.reserve(50);
  vector1.shrink_to_fit();
  vector2.shrink_to_fit();
  EXPECT_TRUE(compare_vectors(vector1, vector2));
}

TEST(TestVector, Shrink_2) {
  s21::vector<int> vector1 = {1, 2, 3, 4, 5, 6};
  std::vector<int> vector2 = {1, 2, 3, 4, 5, 6};
  vector1.reserve(1);
  vector2.reserve(1);
  vector1.shrink_to_fit();
  vector2.shrink_to_fit();
  EXPECT_TRUE(compare_vectors(vector1, vector2));
}

TEST(TestVector, Clear_1) {
  s21::vector<int> vector1{1, 2, 3};
  std::vector<int> vector2{1, 2, 3};
  vector1.clear();
  vector2.clear();
  EXPECT_TRUE(compare_vectors(vector1, vector2));
}

TEST(TestVector, Insert_1) {
  s21::vector<int> vector1{1, 2, 3};
  EXPECT_THROW(vector1.insert(vector1.begin() - 1, 7), std::out_of_range);
}

TEST(TestVector, Insert_2) {
  s21::vector<int> vector1{1, 2, 3};
  EXPECT_THROW(vector1.insert(vector1.end() + 1, 7), std::out_of_range);
}

TEST(TestVector, Insert_3) {
  s21::vector<int> vector1 = {1, 2, 3, 4, 5, 6};
  std::vector<int> vector2 = {1, 2, 3, 4, 5, 6};
  vector1.insert(vector1.end(), 7);
  vector2.insert(vector2.end(), 7);
  EXPECT_TRUE(compare_vectors(vector1, vector2));
}

TEST(TestVector, Insert_4) {
  s21::vector<int> vector1 = {1, 2, 3, 4, 5, 6};
  std::vector<int> vector2 = {1, 2, 3, 4, 5, 6};
  vector1.insert(vector1.begin(), 7);
  vector2.insert(vector2.begin(), 7);
  EXPECT_TRUE(compare_vectors(vector1, vector2));
}

TEST(TestVector, Insert_5) {
  s21::vector<int> vector1 = {1, 2, 3, 4, 5, 6};
  std::vector<int> vector2 = {1, 2, 3, 4, 5, 6};
  vector1.reserve(1);
  vector2.reserve(1);
  vector1.insert(vector1.begin(), 777);
  vector2.insert(vector2.begin(), 777);
  EXPECT_TRUE(compare_vectors(vector1, vector2));
}

TEST(TestVector, Erase_1) {
  s21::vector<int> vector1{1, 2, 3};
  EXPECT_THROW(vector1.erase(vector1.begin() - 1), std::out_of_range);
}

TEST(TestVector, Erase_2) {
  s21::vector<int> vector1{1, 2, 3};
  EXPECT_THROW(vector1.erase(vector1.end() + 1), std::out_of_range);
}

TEST(TestVector, Erase_3) {
  s21::vector<int> vector1 = {1, 2, 3, 4, 5, 6};
  std::vector<int> vector2 = {1, 2, 3, 4, 5, 6};
  vector1.erase(vector1.end() - 1);
  vector2.erase(vector2.end() - 1);
  EXPECT_TRUE(compare_vectors(vector1, vector2));
}

TEST(TestVector, Erase_4) {
  s21::vector<int> vector1 = {1, 2, 3, 4, 5, 6};
  std::vector<int> vector2 = {1, 2, 3, 4, 5, 6};
  vector1.erase(vector1.begin());
  vector2.erase(vector2.begin());
  EXPECT_TRUE(compare_vectors(vector1, vector2));
}

TEST(TestVector, PushBack_1) {
  s21::vector<int> vector1 = {1, 2, 3, 4, 5, 6};
  std::vector<int> vector2 = {1, 2, 3, 4, 5, 6};
  vector1.push_back(777);
  vector2.push_back(777);
  EXPECT_TRUE(compare_vectors(vector1, vector2));
}

TEST(TestVector, PopBack_1) {
  s21::vector<int> vector1 = {1, 2, 3, 4, 5, 6};
  std::vector<int> vector2 = {1, 2, 3, 4, 5, 6};
  vector1.pop_back();
  vector2.pop_back();
  EXPECT_TRUE(compare_vectors(vector1, vector2));
}

TEST(TestVector, PopBack_2) {
  s21::vector<int> vector1;
  EXPECT_THROW(vector1.pop_back(), std::out_of_range);
}

TEST(TestVector, Swap_1) {
  s21::vector<int> vector1 = {1, 2, 3, 4, 5, 6};
  s21::vector<int> vector2 = {1, 2, 3};
  std::vector<int> vector3 = {1, 2, 3, 4, 5, 6};
  std::vector<int> vector4 = {1, 2, 3};
  vector1.swap(vector2);
  vector3.swap(vector4);
  EXPECT_TRUE(compare_vectors(vector1, vector3));
  EXPECT_TRUE(compare_vectors(vector2, vector4));
}

TEST(TestVector, Swap_2) {
  s21::vector<int> vector1 = {1, 2, 3, 4, 5, 6};
  s21::vector<int> vector2 = {1, 2, 3};
  std::vector<int> vector3 = {1, 2, 3, 4, 5, 6};
  std::vector<int> vector4 = {1, 2, 3};
  vector2.swap(vector1);
  vector4.swap(vector3);
  EXPECT_TRUE(compare_vectors(vector1, vector3));
  EXPECT_TRUE(compare_vectors(vector2, vector4));
}

TEST(TestVector, Insert_many_1) {
  s21::vector<int> vector1 = {1, 2, 3, 4, 5, 6};
  vector1.insert_many(vector1.begin(), -1);
  EXPECT_EQ(vector1[0], -1);
  EXPECT_EQ(vector1[1], 1);
  EXPECT_EQ(vector1.size(), (size_t)7);
}

TEST(TestVector, Insert_many_2) {
  s21::vector<int> vector1 = {1, 2, 3, 4, 5, 6};
  vector1.insert_many(vector1.begin(), -2, -1);
  EXPECT_EQ(vector1[0], -2);
  EXPECT_EQ(vector1[1], -1);
  EXPECT_EQ(vector1[2], 1);
  EXPECT_EQ(vector1.size(), (size_t)8);
}

TEST(TestVector, Insert_many_3) {
  s21::vector<int> vector1 = {1, 2, 3, 4, 5, 6};
  std::vector<int> vector2 = {1, 2, 3, 4, 5, 6};
  vector1.insert_many(vector1.begin());
  EXPECT_TRUE(compare_vectors(vector1, vector2));
}

TEST(TestVector, Insert_many_4) {
  s21::vector<int> vector1 = {1, 2, 3, 4, 5, 6};
  std::vector<int> vector2 = {1, 2, 3, 11, 12, 13, 14, 15, 16, 4, 5, 6};
  vector1.insert_many(vector1.begin() + 3, 11, 12, 13, 14, 15, 16);
  EXPECT_TRUE(compare_vectors(vector1, vector2));
}

TEST(TestVector, Insert_many_Back_1) {
  s21::vector<int> vector1 = {1, 2, 3, 4, 5, 6};
  std::vector<int> vector2 = {1, 2, 3, 4, 5, 6, 6, 5, 4, 3, 2, 1};
  vector1.insert_many_back(6, 5, 4, 3, 2, 1);
  EXPECT_TRUE(compare_vectors(vector1, vector2));
}

TEST(TestVector, Insert_many_Back_2) {
  s21::vector<int> v = {1, 2, 3};
  v.insert_many_back(4, 5);
  EXPECT_EQ(v.size(), 5);
  EXPECT_EQ(v[3], 4);
  EXPECT_EQ(v[4], 5);
}

TEST(TestVector, ConstructorZeroSize) {
  s21::vector<int> v(0);
  EXPECT_TRUE(v.empty());
  EXPECT_EQ(v.size(), 0);
  EXPECT_EQ(v.capacity(), 0);
  EXPECT_EQ(v.data(), nullptr);
}

TEST(TestVector, AtOutOfRange) {
  s21::vector<int> v = {1, 2, 3};
  EXPECT_THROW({ v.at(3); }, std::out_of_range);
  EXPECT_THROW({ v.at(100); }, std::out_of_range);
}

TEST(TestVector, ReserveMaxSize) {
  s21::vector<int> v;
  EXPECT_THROW(
      { v.reserve(std::numeric_limits<s21::vector<int>::size_type>::max()); },
      std::length_error);
}

TEST(TestVector, ReserveSmallerCapacity) {
  s21::vector<int> v(5);
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  size_t old_capacity = v.capacity();
  v.reserve(old_capacity - 1);  // Should do nothing
  EXPECT_EQ(v.capacity(), old_capacity);
  EXPECT_EQ(v.size(), 8);
}

TEST(TestVector, ShrinkToFitNoShrink) {
  s21::vector<int> v(3);
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  size_t old_capacity = v.capacity();  // Should be 6
  v.shrink_to_fit();                   // Should do nothing as size == capacity
  EXPECT_EQ(v.capacity(), old_capacity);
  EXPECT_EQ(v.size(), 6);
}

TEST(TestVector, InsertInvalidIterator) {
  s21::vector<int> v = {1, 2, 3};
  EXPECT_THROW(
      {
        v.insert(v.begin() - 1, 0);  // Before begin
      },
      std::out_of_range);
  EXPECT_THROW(
      {
        v.insert(v.end() + 1, 0);  // After end
      },
      std::out_of_range);
}

TEST(TestVector, EraseInvalidIterator) {
  s21::vector<int> v = {1, 2, 3};
  EXPECT_THROW(
      {
        v.erase(v.begin() - 1);  // Before begin
      },
      std::out_of_range);
  EXPECT_THROW(
      {
        v.erase(v.end());  // At end (invalid for erase)
      },
      std::out_of_range);
}

TEST(TestVector, PopBackEmptyVector) {
  s21::vector<int> v;
  EXPECT_THROW({ v.pop_back(); }, std::out_of_range);
}

TEST(TestVector, AtBoundsChecking) {
  s21::vector<int> vec = {10, 20, 30};
  EXPECT_THROW(vec.at(3), std::out_of_range);
  EXPECT_NO_THROW(vec.at(0));
  EXPECT_EQ(vec.at(0), 10);
  EXPECT_EQ(vec.at(1), 20);
  EXPECT_EQ(vec.at(2), 30);
}

TEST(VectorBranchExtra, ClearEmptyVector) {
  s21::vector<int> v;
  v.clear();  // size_ == 0 branch
  EXPECT_TRUE(v.empty());
}

TEST(VectorFunctions, ConstAccess) {
  const s21::vector<int> v = {1, 2, 3};
  EXPECT_EQ(v.front(), 1);
  EXPECT_EQ(v.back(), 3);
  EXPECT_EQ(v.at(1), 2);

  auto it = v.begin();
  EXPECT_EQ(*it, 1);
}

TEST(VectorFunctions, PushBackMove) {
  s21::vector<std::string> v;
  std::string s = "hello";
  v.push_back(std::move(s));
  EXPECT_EQ(v[0], "hello");
}

TEST(VectorFunctions, MoveAssignDifferent) {
  s21::vector<int> a = {1, 2};
  s21::vector<int> b;
  b = std::move(a);
  EXPECT_EQ(b.size(), 2);
}

TEST(VectorCoverage, CharSizeConstructor) {
  s21::vector<char> v(5);
  EXPECT_EQ(v.size(), 5);
  EXPECT_EQ(v.capacity(), 5);
}

// TEST(VectorBranchExtra, ShrinkToFitEmptyVector) {
//   s21::vector<int> v;
//   v.shrink_to_fit();  // size == 0 && capacity == 0
//   EXPECT_EQ(v.capacity(), 0u);
// }

// TEST(VectorBranchExtra, ReserveEqualCapacity) {
//   s21::vector<int> v(5);
//   size_t cap = v.capacity();
//   v.reserve(cap);  // new_cap == capacity_
//   EXPECT_EQ(v.capacity(), cap);
// }

// TEST(VectorBranchExtra, InsertAtEndWhenFull) {
//   s21::vector<int> v(1);  // size == capacity == 1
//   v[0] = 42;
//   v.insert(v.end(), 100);  // reserve + index_pos == size_
//   EXPECT_EQ(v.size(), 2u);
//   EXPECT_EQ(v.back(), 100);
// }

// TEST(VectorBranchExtra, ShrinkAfterClear) {
//   s21::vector<int> v;
//   v.clear();          // data_ == nullptr
//   v.shrink_to_fit();  // destroy_and_deallocate skipped
//   EXPECT_TRUE(v.empty());
// }

// TEST(TestVector, Constructor_Fill) {
//   s21::vector<int> v1(5, 42);
//   std::vector<int> v2(5, 42);
//   EXPECT_TRUE(compare_vectors(v1, v2));
// }
