#include "Queue.h"

namespace s21 {

Queue::Queue() : storage_() {}

void Queue::Push(int value) { storage_.push(value); }

int Queue::Pop() {
  int value = storage_.front();
  storage_.pop();
  return value;
}

int Queue::Front() const { return storage_.front(); }

int Queue::Back() const { return storage_.back(); }

int Queue::Empty() const noexcept { return storage_.empty(); }

int Queue::Size() const noexcept { return storage_.size(); }

}  // namespace s21