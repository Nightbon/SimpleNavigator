#ifndef QUEUE_H
#define QUEUE_H
#include "s21_queue.h"

namespace s21 {

class Queue {
 public:
  Queue();
  void Push(int value);
  int Pop();
  int Front() const;
  int Back() const;
  int Empty() const noexcept;
  int Size() const noexcept;

 private:
  s21::queue<int> storage_;
};

}  // namespace s21

#endif  // QUEUE_H