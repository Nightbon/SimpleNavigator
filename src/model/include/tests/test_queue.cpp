#include <gtest/gtest.h>

#include <queue>  // Для сравнения со std::queue

#include "../s21_queue.h"

// Тесты для конструкторов
TEST(QueueTest, DefaultConstructor) {
  s21::queue<int> s21_queue;
  std::queue<int> std_queue;
  EXPECT_TRUE(s21_queue.empty());
  EXPECT_EQ(s21_queue.size(), std_queue.size());
}

TEST(QueueTest, InitializerListConstructor) {
  s21::queue<int> s21_queue = {1, 2, 3};
  EXPECT_FALSE(s21_queue.empty());
  EXPECT_EQ(s21_queue.size(), 3);
  EXPECT_EQ(s21_queue.front(), 1);
  EXPECT_EQ(s21_queue.back(), 3);
}

TEST(QueueTest, InitializerListConstructorEmpty) {
  s21::queue<int> s21_queue = {};
  EXPECT_TRUE(s21_queue.empty());
  EXPECT_EQ(s21_queue.size(), 0);
}

TEST(QueueTest, CopyConstructor) {
  s21::queue<int> original = {1, 2, 3};
  s21::queue<int> copied = original;
  EXPECT_EQ(copied.size(), original.size());
  EXPECT_EQ(copied.front(), original.front());
  EXPECT_EQ(copied.back(), original.back());

  // Проверка глубокой копии
  original.push(4);
  EXPECT_NE(copied.size(), original.size());
}

TEST(QueueTest, MoveConstructor) {
  s21::queue<int> original = {1, 2, 3};
  s21::queue<int> moved = std::move(original);
  EXPECT_TRUE(original.empty());  // Оригинал должен быть пуст
  EXPECT_EQ(original.size(), 0);
  EXPECT_FALSE(moved.empty());
  EXPECT_EQ(moved.size(), 3);
  EXPECT_EQ(moved.front(), 1);
  EXPECT_EQ(moved.back(), 3);
}

// Тесты для операторов присваивания
TEST(QueueTest, CopyAssignmentOperator) {
  s21::queue<int> q1 = {1, 2};
  s21::queue<int> q2 = {4, 5, 6};
  s21::queue<int> q_expected = {4, 5, 6};
  q1 = q2;
  EXPECT_EQ(q1.size(), q_expected.size());
  EXPECT_EQ(q1.front(), q_expected.front());
  EXPECT_EQ(q1.back(), q_expected.back());

  // Проверка глубокой копии
  q2.push(7);
  EXPECT_NE(q1.size(), q2.size());
}

TEST(QueueTest, CopyAssignmentOperatorSelfAssignment) {
  s21::queue<int> q = {1, 2, 3};
  q = q;  // Самоприсваивание
  EXPECT_EQ(q.size(), 3);
  EXPECT_EQ(q.front(), 1);
  EXPECT_EQ(q.back(), 3);
}

TEST(QueueTest, MoveAssignmentOperator) {
  s21::queue<int> q1 = {1, 2};
  s21::queue<int> q2 = {4, 5, 6};
  q1 = std::move(q2);
  EXPECT_EQ(q1.size(), 3);
  EXPECT_EQ(q1.front(), 4);
  EXPECT_EQ(q1.back(), 6);
}

// Тесты для методов доступа к элементам
TEST(QueueTest, Front) {
  s21::queue<int> q = {10, 20, 30};
  EXPECT_EQ(q.front(), 10);
  q.push(40);
  EXPECT_EQ(q.front(), 10);
}

TEST(QueueTest, FrontEmptyThrows) {
  s21::queue<int> q;
  EXPECT_THROW(q.front(), std::out_of_range);
}

TEST(QueueTest, Back) {
  s21::queue<int> q = {10, 20, 30};
  EXPECT_EQ(q.back(), 30);
  q.push(40);
  EXPECT_EQ(q.back(), 40);
}

TEST(QueueTest, BackEmptyThrows) {
  s21::queue<int> q;
  EXPECT_THROW(q.back(), std::out_of_range);
}

// Тесты для методов емкости
TEST(QueueTest, Empty) {
  s21::queue<int> q;
  EXPECT_TRUE(q.empty());
  q.push(1);
  EXPECT_FALSE(q.empty());
}

TEST(QueueTest, Size) {
  s21::queue<int> q;
  EXPECT_EQ(q.size(), 0);
  q.push(1);
  EXPECT_EQ(q.size(), 1);
  q.push(2);
  EXPECT_EQ(q.size(), 2);
  q.pop();
  EXPECT_EQ(q.size(), 1);
}

// Тесты для модификаторов
TEST(QueueTest, Push) {
  s21::queue<int> q;
  q.push(10);
  EXPECT_EQ(q.size(), 1);
  EXPECT_EQ(q.front(), 10);
  EXPECT_EQ(q.back(), 10);
  q.push(20);
  EXPECT_EQ(q.size(), 2);
  EXPECT_EQ(q.front(), 10);
  EXPECT_EQ(q.back(), 20);
}

TEST(QueueTest, Pop) {
  s21::queue<int> q = {10, 20, 30};
  q.pop();
  EXPECT_EQ(q.size(), 2);
  EXPECT_EQ(q.front(), 20);
  EXPECT_EQ(q.back(), 30);
  q.pop();
  EXPECT_EQ(q.size(), 1);
  EXPECT_EQ(q.front(), 30);
  EXPECT_EQ(q.back(), 30);
  q.pop();
  EXPECT_EQ(q.size(), 0);
  EXPECT_TRUE(q.empty());
}

TEST(QueueTest, PopEmptyThrows) {
  s21::queue<int> q;
  EXPECT_THROW(q.pop(), std::out_of_range);
}

TEST(QueueTest, Swap) {
  s21::queue<int> q1 = {1, 2, 3};
  s21::queue<int> q2 = {4, 5};

  q1.swap(q2);

  EXPECT_EQ(q1.size(), 2);
  EXPECT_EQ(q1.front(), 4);
  EXPECT_EQ(q1.back(), 5);
  EXPECT_EQ(q2.size(), 3);
  EXPECT_EQ(q2.front(), 1);
  EXPECT_EQ(q2.back(), 3);
}

TEST(QueueTest, SwapEmpty) {
  s21::queue<int> q1;
  s21::queue<int> q2 = {1, 2};

  q1.swap(q2);

  EXPECT_EQ(q1.size(), 2);
  EXPECT_EQ(q1.front(), 1);
  EXPECT_EQ(q1.back(), 2);
  EXPECT_TRUE(q2.empty());
}

// Тесты для insert_many_back (для очереди это будет обычный push)
TEST(QueueTest, InsertManyBackZeroArgs) {
  s21::queue<int> q = {1, 2};
  q.insert_many_back();
  EXPECT_EQ(q.size(), 2);
  EXPECT_EQ(q.front(), 1);
  EXPECT_EQ(q.back(), 2);
}

TEST(QueueTest, InsertManyBackOneArg) {
  s21::queue<int> q;
  q.insert_many_back(10);
  EXPECT_EQ(q.size(), 1);
  EXPECT_EQ(q.front(), 10);
  EXPECT_EQ(q.back(), 10);
}

TEST(QueueTest, InsertManyBackMultipleArgs) {
  s21::queue<int> q;
  q.insert_many_back(10, 20, 30);
  EXPECT_EQ(q.size(), 3);
  EXPECT_EQ(q.front(), 10);
  EXPECT_EQ(q.back(), 30);
}

TEST(QueueTest, MaxSize) {
  s21::queue<int> q;
  EXPECT_GE(q.max_size(), 0);  // max_size всегда неотрицательный
}

TEST(QueueTest, AssignmentOperatorEmptySource) {
  s21::queue<int> q1 = {1, 2, 3};
  s21::queue<int> q2;
  q1 = q2;
  EXPECT_TRUE(q1.empty());
  EXPECT_EQ(q1.size(), 0);
}

TEST(QueueTest, MoveAssignmentSelfViaAlias) {
  s21::queue<int> q = {1, 2, 3};
  s21::queue<int>& alias = q;

  q = std::move(alias);  // this == &other, НО без self-move warning

  EXPECT_EQ(q.size(), 3);
  EXPECT_EQ(q.front(), 1);
  EXPECT_EQ(q.back(), 3);
}
