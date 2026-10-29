#include <gtest/gtest.h>

#include <stack>  // Для сравнения со std::stack

#include "../s21_stack.h"

// Тесты для конструкторов
TEST(StackTest, DefaultConstructor) {
  s21::stack<int> s21_stack;
  std::stack<int> std_stack;
  EXPECT_TRUE(s21_stack.empty());
  EXPECT_EQ(s21_stack.size(), std_stack.size());
}

TEST(StackTest, InitializerListConstructor) {
  s21::stack<int> s21_stack = {1, 2, 3};
  EXPECT_FALSE(s21_stack.empty());
  EXPECT_EQ(s21_stack.size(), 3);
  EXPECT_EQ(s21_stack.top(), 3);
}

TEST(StackTest, InitializerListConstructorEmpty) {
  s21::stack<int> s21_stack = {};
  EXPECT_TRUE(s21_stack.empty());
  EXPECT_EQ(s21_stack.size(), 0);
}

TEST(StackTest, CopyConstructor) {
  s21::stack<int> original = {1, 2, 3};
  s21::stack<int> copied = original;
  EXPECT_EQ(copied.size(), original.size());
  EXPECT_EQ(copied.top(), original.top());

  // Проверка глубокой копии
  original.push(4);
  EXPECT_NE(copied.size(), original.size());
}

TEST(StackTest, MoveConstructor) {
  s21::stack<int> original = {1, 2, 3};
  s21::stack<int> moved = std::move(original);
  EXPECT_TRUE(original.empty());  // Оригинал должен быть пуст
  EXPECT_EQ(original.size(), 0);
  EXPECT_FALSE(moved.empty());
  EXPECT_EQ(moved.size(), 3);
  EXPECT_EQ(moved.top(), 3);
}

// Тесты для операторов присваивания
TEST(StackTest, CopyAssignmentOperator) {
  s21::stack<int> s1 = {1, 2};
  s21::stack<int> s2 = {4, 5, 6};
  s21::stack<int> s_expected = {4, 5, 6};
  s1 = s2;
  EXPECT_EQ(s1.size(), s_expected.size());
  EXPECT_EQ(s1.top(), s_expected.top());

  // Проверка глубокой копии
  s2.push(7);
  EXPECT_NE(s1.size(), s2.size());
}

TEST(StackTest, CopyAssignmentOperatorSelfAssignment) {
  s21::stack<int> s = {1, 2, 3};
  s = s;  // Самоприсваивание
  EXPECT_EQ(s.size(), 3);
  EXPECT_EQ(s.top(), 3);
}

// Тесты для методов доступа к элементам
TEST(StackTest, Top) {
  s21::stack<int> s = {10, 20, 30};
  EXPECT_EQ(s.top(), 30);
  s.push(40);
  EXPECT_EQ(s.top(), 40);
}

TEST(StackTest, TopEmptyThrows) {
  s21::stack<int> s;
  EXPECT_THROW(s.top(), std::out_of_range);
}

// Тесты для методов емкости
TEST(StackTest, Empty) {
  s21::stack<int> s;
  EXPECT_TRUE(s.empty());
  s.push(1);
  EXPECT_FALSE(s.empty());
}

TEST(StackTest, Size) {
  s21::stack<int> s;
  EXPECT_EQ(s.size(), 0);
  s.push(1);
  EXPECT_EQ(s.size(), 1);
  s.push(2);
  EXPECT_EQ(s.size(), 2);
  s.pop();
  EXPECT_EQ(s.size(), 1);
}

// Тесты для модификаторов
TEST(StackTest, Push) {
  s21::stack<int> s;
  s.push(10);
  EXPECT_EQ(s.size(), 1);
  EXPECT_EQ(s.top(), 10);
  s.push(20);
  EXPECT_EQ(s.size(), 2);
  EXPECT_EQ(s.top(), 20);
}

TEST(StackTest, Pop) {
  s21::stack<int> s = {10, 20, 30};
  s.pop();
  EXPECT_EQ(s.size(), 2);
  EXPECT_EQ(s.top(), 20);
  s.pop();
  EXPECT_EQ(s.size(), 1);
  EXPECT_EQ(s.top(), 10);
  s.pop();
  EXPECT_EQ(s.size(), 0);
  EXPECT_TRUE(s.empty());
}

TEST(StackTest, PopEmptyThrows) {
  s21::stack<int> s;
  EXPECT_THROW(s.pop(), std::out_of_range);
}

TEST(StackTest, Swap) {
  s21::stack<int> s1 = {1, 2, 3};
  s21::stack<int> s2 = {4, 5};

  s1.swap(s2);

  EXPECT_EQ(s1.size(), 2);
  EXPECT_EQ(s1.top(), 5);
  EXPECT_EQ(s2.size(), 3);
  EXPECT_EQ(s2.top(), 3);
}

TEST(StackTest, SwapEmpty) {
  s21::stack<int> s1;
  s21::stack<int> s2 = {1, 2};

  s1.swap(s2);

  EXPECT_EQ(s1.size(), 2);
  EXPECT_EQ(s1.top(), 2);
  EXPECT_TRUE(s2.empty());
}

// Тесты для insert_many_front (для стека это будет обычный push)
TEST(StackTest, InsertManyFrontZeroArgs) {
  s21::stack<int> s = {1, 2};
  s.insert_many_back();
  EXPECT_EQ(s.size(), 2);
  EXPECT_EQ(s.top(), 2);
}

TEST(StackTest, InsertManyFrontOneArg) {
  s21::stack<int> s;
  s.insert_many_back(10);
  EXPECT_EQ(s.size(), 1);
  EXPECT_EQ(s.top(), 10);
}

TEST(StackTest, InsertManyFrontMultipleArgs) {
  s21::stack<int> s;
  s.insert_many_back(10, 20, 30);
  EXPECT_EQ(s.size(), 3);
  EXPECT_EQ(s.top(), 30);  // 10 -> 20 -> 30 (top)
}

TEST(StackTest, MaxSize) {
  s21::stack<int> s;
  EXPECT_GE(s.max_size(), 0);  // max_size всегда неотрицательный
}

TEST(StackTest, AssignmentOperatorEmptySource) {
  s21::stack<int> s1 = {1, 2, 3};
  s21::stack<int> s2;
  s1 = s2;
  EXPECT_TRUE(s1.empty());
  EXPECT_EQ(s1.size(), 0);
}
