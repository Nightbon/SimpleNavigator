#ifndef STACK_H
#define STACK_H
#include "s21_stack.h"

namespace s21 {

class Stack {
 public:
  Stack();
  void Push(int value);
  int Pop();
  int Top() const;
  int Empty() const noexcept;
  int Size() const noexcept;

 private:
  s21::stack<int> storage_;
};

}  // namespace s21

#endif  // STACK_H