#include "Stack.h"

namespace s21 {

Stack::Stack() : storage_() {}

void Stack::Push(int value) { storage_.push(value); }

int Stack::Pop() {
  int value = storage_.top();
  storage_.pop();
  return value;
}

int Stack::Top() const { return storage_.top(); }

int Stack::Empty() const noexcept { return storage_.empty(); }

int Stack::Size() const noexcept { return storage_.size(); }

}  // namespace s21