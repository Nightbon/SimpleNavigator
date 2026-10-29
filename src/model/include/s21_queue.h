#ifndef S21_QUEUE_h
#define S21_QUEUE_h

#include "s21_list.h"

namespace s21 {
template <typename T, class Container = s21::list<T>>
class queue {
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
  queue() = default;

  explicit queue(const container_type& cont) : c_(cont) {}

  explicit queue(container_type&& cont) : c_(std::move(cont)) {}

  queue(const queue& other) : c_(other.c_) {}

  queue(queue&& other) noexcept : c_(std::move(other.c_)) {}

  queue(std::initializer_list<value_type> const& items) : c_(items) {}

  // Операторы присваивания
  queue& operator=(const queue& other) {
    if (this != &other) {
      c_ = other.c_;
    }
    return *this;
  }

  queue& operator=(queue&& other) noexcept {
    if (this != &other) {
      c_ = std::move(other.c_);
    }
    return *this;
  }

  ~queue() = default;

  // Доступ к элементам
  reference front() { return c_.front(); }

  const_reference front() const { return c_.front(); }

  reference back() { return c_.back(); }

  const_reference back() const { return c_.back(); }

  // наполнение
  bool empty() const noexcept { return c_.empty(); }

  size_type size() const noexcept { return c_.size(); }

  size_type max_size() const noexcept { return c_.max_size(); }

  // Модификаторы
  void push(const_reference value) { c_.push_back(value); }

  void push(value_type&& value) { c_.push_back(std::move(value)); }

  void pop() { c_.pop_front(); }

  void swap(queue& other) noexcept { c_.swap(other.c_); }

  template <typename... Args>
  void insert_many_back(Args&&... args) {
    c_.insert_many_back(std::forward<Args>(args)...);
  }

  // Получение базового контейнера
  const container_type& get_container() const noexcept { return c_; }
};

// Не-член функция swap для ADL
template <typename T, class Container>
void swap(queue<T, Container>& lhs,
          queue<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
  lhs.swap(rhs);
}

}  // namespace s21

#endif