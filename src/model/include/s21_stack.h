#ifndef S21_STACK_H
#define S21_STACK_H

#include "s21_list.h"

namespace s21 {
template <typename T, class Container = s21::list<T>>
class stack {
 public:
  using value_type = typename Container::value_type;
  using reference = typename Container::reference;
  using const_reference = typename Container::const_reference;
  using size_type = typename Container::size_type;
  using container_type = Container;

 private:
  Container c_;  // Базовый контейнер

 public:
  // Конструкторы
  stack() = default;

  explicit stack(const container_type& cont) : c_(cont) {}

  explicit stack(container_type&& cont) : c_(std::move(cont)) {}

  stack(const stack& other) : c_(other.c_) {}

  stack(stack&& other) noexcept : c_(std::move(other.c_)) {}

  stack(std::initializer_list<value_type> const& items) : c_(items) {}

  // Операторы присваивания
  stack& operator=(const stack& other) {
    if (this != &other) {
      c_ = other.c_;
    }
    return *this;
  }

  stack& operator=(stack&& other) noexcept {
    if (this != &other) {
      c_ = std::move(other.c_);
    }
    return *this;
  }

  ~stack() = default;

  // Доступ к элементам
  reference top() { return c_.back(); }

  const_reference top() const { return c_.back(); }

  // наполнение
  bool empty() const noexcept { return c_.empty(); }

  size_type size() const noexcept { return c_.size(); }

  size_type max_size() const noexcept { return c_.max_size(); }

  // Модификаторы
  void push(const_reference value) { c_.push_back(value); }

  void push(value_type&& value) { c_.push_back(std::move(value)); }

  void pop() { c_.pop_back(); }

  void swap(stack& other) noexcept { c_.swap(other.c_); }

  template <typename... Args>
  void insert_many_back(Args&&... args) {
    c_.insert_many_back(std::forward<Args>(args)...);
  }

  // Получение базового контейнера
  const container_type& get_container() const noexcept { return c_; }
};

// Не-член функция swap для ADL
template <typename T, class Container>
void swap(stack<T, Container>& lhs,
          stack<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
  lhs.swap(rhs);
}

}  // namespace s21

#endif