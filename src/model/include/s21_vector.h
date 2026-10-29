#ifndef S21_VECTOR_H
#define S21_VECTOR_H

#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace s21 {
template <typename T>
class vector {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = T *;
  using const_iterator = const T *;
  using size_type = size_t;
  using allocator_type = std::allocator<value_type>;
  using alloc_traits = std::allocator_traits<allocator_type>;

 private:
  iterator data_;
  size_type size_;
  size_type capacity_;
  allocator_type alloc_;

 public:
  // Конструкторы
  vector() : data_(nullptr), size_(0), capacity_(0) {}

  explicit vector(size_type n) : size_(n), capacity_(n) {
    if (n > 0) {
      data_ = allocate_and_construct(n);
    } else {
      data_ = nullptr;
    }
  }

  vector(size_type n, const_reference value) : size_(n), capacity_(n) {
    if (n > 0) {
      data_ = allocate_and_fill(n, value);
    } else {
      data_ = nullptr;
    }
  }

  vector(std::initializer_list<value_type> const &items)
      : data_(nullptr), size_(items.size()), capacity_(items.size()) {
    if (capacity_ > 0) {
      data_ = allocate_initializer_list(items);
    } else {
      data_ = nullptr;
    }
  }

  vector(const vector &v)
      : data_(nullptr), size_(v.size_), capacity_(v.capacity_) {
    if (capacity_ > 0) {
      data_ = allocate_and_copy(v.data_, size_, capacity_);
    } else {
      data_ = nullptr;
    }
  }

  vector(vector &&v) noexcept
      : data_(v.data_),
        size_(v.size_),
        capacity_(v.capacity_),
        alloc_(std::move(v.alloc_)) {
    v.data_ = nullptr;
    v.size_ = 0;
    v.capacity_ = 0;
  }

  ~vector() {
    clear();
    if (data_) {
      alloc_traits::deallocate(alloc_, data_, capacity_);
    }
  }

  // Операторы присваивания
  vector &operator=(const vector &v) {
    if (this != &v) {
      clear();
      if (capacity_ < v.size_) {
        if (data_) {
          alloc_traits::deallocate(alloc_, data_, capacity_);
        }
        capacity_ = v.size_;
        data_ = alloc_traits::allocate(alloc_, capacity_);
      }

      size_ = v.size_;
      for (size_type i = 0; i < size_; ++i) {
        alloc_traits::construct(alloc_, data_ + i, v.data_[i]);
      }
    }
    return *this;
  }

  vector &operator=(vector &&v) noexcept {
    if (this != &v) {
      clear();
      if (data_) {
        alloc_traits::deallocate(alloc_, data_, capacity_);
      }

      data_ = v.data_;
      size_ = v.size_;
      capacity_ = v.capacity_;
      alloc_ = std::move(v.alloc_);

      v.data_ = nullptr;
      v.size_ = 0;
      v.capacity_ = 0;
    }
    return *this;
  }

  // Доступ к элементам
  reference operator[](size_type pos) { return data_[pos]; }
  const_reference operator[](size_type pos) const { return data_[pos]; }

  reference at(size_type pos) {
    if (pos >= size_) {
      throw std::out_of_range("Vector: Index out of range");
    }
    return data_[pos];
  }

  const_reference at(size_type pos) const {
    if (pos >= size_) {
      throw std::out_of_range("Vector: Index out of range");
    }
    return data_[pos];
  }

  reference front() {
    if (empty()) throw std::out_of_range("Vector: vector is empty");
    return data_[0];
  }

  const_reference front() const {
    if (empty()) throw std::out_of_range("Vector: vector is empty");
    return data_[0];
  }

  reference back() {
    if (empty()) throw std::out_of_range("Vector: vector is empty");
    return data_[size_ - 1];
  }

  const_reference back() const {
    if (empty()) throw std::out_of_range("Vector: vector is empty");
    return data_[size_ - 1];
  }

  iterator data() noexcept { return data_; }
  const_iterator data() const noexcept { return data_; }

  // Итераторы
  iterator begin() noexcept { return data_; }
  iterator end() noexcept { return data_ + size_; }
  const_iterator begin() const noexcept { return data_; }
  const_iterator end() const noexcept { return data_ + size_; }

  // Емкость
  bool empty() const noexcept { return size_ == 0; }
  size_type size() const noexcept { return size_; }
  size_type capacity() const noexcept { return capacity_; }

  size_type max_size() const noexcept { return alloc_traits::max_size(alloc_); }

  void reserve(size_type new_cap) {
    if (new_cap > max_size()) {
      throw std::length_error("Vector: reserve exceeds max_size()");
    }
    if (new_cap <= capacity_) return;

    iterator new_data = alloc_traits::allocate(alloc_, new_cap);
    size_type i = 0;
    try {
      for (; i < size_; ++i) {
        alloc_traits::construct(alloc_, new_data + i,
                                std::move_if_noexcept(data_[i]));
      }
    } catch (...) {
      for (size_type j = 0; j < i; ++j) {
        alloc_traits::destroy(alloc_, new_data + j);
      }
      alloc_traits::deallocate(alloc_, new_data, new_cap);
      throw;
    }

    // Уничтожаем старые данные
    for (size_type j = 0; j < size_; ++j) {
      alloc_traits::destroy(alloc_, data_ + j);
    }
    if (data_) {
      alloc_traits::deallocate(alloc_, data_, capacity_);
    }

    data_ = new_data;
    capacity_ = new_cap;
  }

  void shrink_to_fit() {
    if (size_ < capacity_) {
      if (size_ > 0) {
        iterator new_data = alloc_traits::allocate(alloc_, size_);
        size_type i = 0;
        try {
          for (; i < size_; ++i) {
            alloc_traits::construct(alloc_, new_data + i,
                                    std::move_if_noexcept(data_[i]));
          }
        } catch (...) {
          for (size_type j = 0; j < i; ++j) {
            alloc_traits::destroy(alloc_, new_data + j);
          }
          alloc_traits::deallocate(alloc_, new_data, size_);
          throw;
        }

        // Уничтожаем старые данные
        for (size_type j = 0; j < size_; ++j) {
          alloc_traits::destroy(alloc_, data_ + j);
        }
        alloc_traits::deallocate(alloc_, data_, capacity_);

        data_ = new_data;
        capacity_ = size_;
      } else {
        // size_ == 0
        if (data_) {
          alloc_traits::deallocate(alloc_, data_, capacity_);
          data_ = nullptr;
        }
        capacity_ = 0;
      }
    }
  }

  // Модификаторы
  void clear() noexcept {
    for (size_type i = 0; i < size_; ++i) {
      alloc_traits::destroy(alloc_, data_ + i);
    }
    size_ = 0;
  }

  // Вставка одного элемента
  iterator insert(iterator pos, const_reference value) {
    return insert(pos, 1, value);
  }

  // Вставка нескольких элементов
  iterator insert(iterator pos, size_type n, const_reference value) {
    if (pos < begin() || pos > end()) {
      throw std::out_of_range(
          "Vector: Pos must point to vector element or end()");
    }

    if (n == 0) return pos;

    size_type index = pos - begin();

    // Если нужно расширить capacity
    if (size_ + n > capacity_) {
      size_type new_capacity = capacity_ > 0 ? capacity_ * 2 : 1;
      while (size_ + n > new_capacity) {
        new_capacity *= 2;
      }
      if (new_capacity > max_size()) {
        new_capacity = max_size();
      }
      if (size_ + n > new_capacity) {
        throw std::length_error("Vector: cannot allocate memory");
      }

      iterator new_data = alloc_traits::allocate(alloc_, new_capacity);
      size_type new_size = size_ + n;
      size_type i = 0;

      try {
        // Копируем элементы до позиции вставки
        for (; i < index; ++i) {
          alloc_traits::construct(alloc_, new_data + i,
                                  std::move_if_noexcept(data_[i]));
        }

        // Вставляем новые элементы
        for (size_type j = 0; j < n; ++j, ++i) {
          alloc_traits::construct(alloc_, new_data + i, value);
        }

        // Копируем элементы после позиции вставки
        for (size_type j = index; j < size_; ++j, ++i) {
          alloc_traits::construct(alloc_, new_data + i,
                                  std::move_if_noexcept(data_[j]));
        }
      } catch (...) {
        // В случае исключения очищаем все что успели создать
        for (size_type j = 0; j < i; ++j) {
          alloc_traits::destroy(alloc_, new_data + j);
        }
        alloc_traits::deallocate(alloc_, new_data, new_capacity);
        throw;
      }

      // Уничтожаем старые данные
      for (size_type j = 0; j < size_; ++j) {
        alloc_traits::destroy(alloc_, data_ + j);
      }
      if (data_) {
        alloc_traits::deallocate(alloc_, data_, capacity_);
      }

      data_ = new_data;
      size_ = new_size;
      capacity_ = new_capacity;
    } else {
      // Есть достаточно места

      // Сдвигаем существующие элементы вправо
      for (size_type i = size_; i > index; --i) {
        if (i < size_) {
          alloc_traits::destroy(alloc_, data_ + i + n - 1);
        }
        alloc_traits::construct(alloc_, data_ + i + n - 1,
                                std::move_if_noexcept(data_[i - 1]));
        alloc_traits::destroy(alloc_, data_ + i - 1);
      }

      // Вставляем новые элементы
      for (size_type i = 0; i < n; ++i) {
        if (index + i < size_) {
          alloc_traits::destroy(alloc_, data_ + index + i);
        }
        alloc_traits::construct(alloc_, data_ + index + i, value);
      }

      size_ += n;
    }

    return begin() + index;
  }

  void erase(const iterator pos) {
    if (pos < begin() || pos >= end()) {
      throw std::out_of_range("Vector: Pos must point to vector element");
    }

    size_type index = pos - begin();

    // Сдвигаем элементы влево
    alloc_traits::destroy(alloc_, data_ + index);
    for (size_type i = index; i < size_ - 1; ++i) {
      data_[i] = std::move(data_[i + 1]);
    }
    --size_;
    if (size_ > 0) {
      alloc_traits::destroy(alloc_, data_ + size_);
    }
  }

  void push_back(const_reference value) { insert(end(), value); }

  void pop_back() {
    if (empty()) {
      throw std::out_of_range("Vector: cannot pop_back from empty vector");
    }
    --size_;
    alloc_traits::destroy(alloc_, data_ + size_);
  }

  void swap(vector &other) noexcept {
    using std::swap;
    swap(data_, other.data_);
    swap(size_, other.size_);
    swap(capacity_, other.capacity_);
    swap(alloc_, other.alloc_);
  }

  // Методы insert_many
  template <typename... Args>
  iterator insert_many(const_iterator pos, Args &&...args) {
    if constexpr (sizeof...(args) == 0) return const_cast<iterator>(pos);

    s21::vector<value_type> values = {std::forward<Args>(args)...};
    iterator tmp_pos = const_cast<iterator>(pos);

    for (const auto &value : values) {
      tmp_pos = insert(tmp_pos, value);
      ++tmp_pos;
    }

    return tmp_pos - values.size();
  }

  template <typename... Args>
  void insert_many_back(Args &&...args) {
    insert_many(end(), std::forward<Args>(args)...);
  }

 private:
  // Вспомогательные методы
  iterator allocate_and_construct(size_type n) {
    iterator new_data = alloc_traits::allocate(alloc_, n);
    size_type i = 0;
    try {
      for (; i < n; ++i) {
        alloc_traits::construct(alloc_, new_data + i);
      }
    } catch (...) {
      for (size_type j = 0; j < i; ++j) {
        alloc_traits::destroy(alloc_, new_data + j);
      }
      alloc_traits::deallocate(alloc_, new_data, n);
      throw;
    }
    return new_data;
  }

  iterator allocate_and_fill(size_type n, const_reference value) {
    iterator new_data = alloc_traits::allocate(alloc_, n);
    size_type i = 0;
    try {
      for (; i < n; ++i) {
        alloc_traits::construct(alloc_, new_data + i, value);
      }
    } catch (...) {
      for (size_type j = 0; j < i; ++j) {
        alloc_traits::destroy(alloc_, new_data + j);
      }
      alloc_traits::deallocate(alloc_, new_data, n);
      throw;
    }
    return new_data;
  }

  iterator allocate_initializer_list(
      std::initializer_list<value_type> const &items) {
    iterator new_data = alloc_traits::allocate(alloc_, capacity_);
    size_type i = 0;
    try {
      for (const auto &item : items) {
        alloc_traits::construct(alloc_, new_data + i, item);
        ++i;
      }
    } catch (...) {
      for (size_type j = 0; j < i; ++j) {
        alloc_traits::destroy(alloc_, new_data + j);
      }
      alloc_traits::deallocate(alloc_, new_data, capacity_);
      throw;
    }
    return new_data;
  }

  iterator allocate_and_copy(const_iterator src, size_type size,
                             size_type capacity) {
    iterator new_data = alloc_traits::allocate(alloc_, capacity);
    size_type i = 0;
    try {
      for (; i < size; ++i) {
        alloc_traits::construct(alloc_, new_data + i, src[i]);
      }
    } catch (...) {
      for (size_type j = 0; j < i; ++j) {
        alloc_traits::destroy(alloc_, new_data + j);
      }
      alloc_traits::deallocate(alloc_, new_data, capacity);
      throw;
    }
    return new_data;
  }
};

}  // namespace s21

#endif  // S21_VECTOR_H