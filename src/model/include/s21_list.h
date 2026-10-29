#ifndef S21_LIST_h
#define S21_LIST_h

#include <initializer_list>
#include <stdexcept>

#include "s21_vector.h"

namespace s21 {
template <typename T>
class list {
 public:
  class ListIterator;
  class ListConstIterator;

  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using iterator = ListIterator;
  using const_iterator = ListConstIterator;
  using size_type = size_t;
  using allocator_type = std::allocator<value_type>;

 private:
  struct Node {
    value_type data_;
    Node* prev_ = nullptr;
    Node* next_ = nullptr;

    template <typename... Args>
    Node(Args&&... args)
        : data_(std::forward<Args>(args)...), prev_(nullptr), next_(nullptr) {}
  };

  using node_allocator = typename std::allocator_traits<
      allocator_type>::template rebind_alloc<Node>;
  using node_alloc_traits = std::allocator_traits<node_allocator>;

  Node* head_;
  Node* tail_;
  size_type list_size_;
  node_allocator node_alloc_;

  // Вспомогательные методы для работы с аллокатором
  Node* allocate_node() { return node_alloc_traits::allocate(node_alloc_, 1); }

  template <typename... Args>
  Node* create_node(Args&&... args) {
    Node* node = allocate_node();
    try {
      node_alloc_traits::construct(node_alloc_, node,
                                   std::forward<Args>(args)...);
    } catch (...) {
      node_alloc_traits::deallocate(node_alloc_, node, 1);
      throw;
    }
    return node;
  }

  void destroy_node(Node* node) {
    if (node) {
      node_alloc_traits::destroy(node_alloc_, node);
      node_alloc_traits::deallocate(node_alloc_, node, 1);
    }
  }

  // Безопасное копирование из другого списка
  void copy_nodes(const list& other) {
    for (Node* curr = other.head_->next_; curr != other.tail_;
         curr = curr->next_) {
      push_back(curr->data_);
    }
  }

 public:
  // Конструкторы
  list() : list_size_(0) {
    // Создаем фиктивные узлы
    head_ = create_node();
    tail_ = create_node();
    head_->next_ = tail_;
    tail_->prev_ = head_;
  }

  explicit list(size_type n) : list() {
    if (n > max_size())
      throw std::length_error("list: requested size exceeds max_size()");

    try {
      for (size_type i = 0; i < n; ++i) {
        push_back(T{});
      }
    } catch (...) {
      clear();
      destroy_node(head_);
      destroy_node(tail_);
      throw;
    }
  }

  list(const std::initializer_list<value_type>& items) : list() {
    try {
      for (const auto& item : items) {
        push_back(item);
      }
    } catch (...) {
      clear();
      destroy_node(head_);
      destroy_node(tail_);
      throw;
    }
  }

  // Копирующий конструктор
  list(const list& other) : list() {
    try {
      copy_nodes(other);
    } catch (...) {
      clear();
      destroy_node(head_);
      destroy_node(tail_);
      throw;
    }
  }

  list& operator=(const list& l) {
    list tmp(l);
    swap(tmp);
    return *this;
  }

  // Конструктор переноса
  list(list&& l)
      : head_(l.head_),
        tail_(l.tail_),
        list_size_(l.list_size_),
        node_alloc_(std::move(l.node_alloc_)) {
    l.head_ = nullptr;
    l.tail_ = nullptr;
    l.list_size_ = 0;
  }

  // Перегрузка оператора, перемещающая
  list& operator=(list&& l) {
    if (this != &l) {
      clear();
      destroy_node(head_);
      destroy_node(tail_);

      head_ = l.head_;
      tail_ = l.tail_;
      list_size_ = l.list_size_;
      node_alloc_ = std::move(l.node_alloc_);

      l.head_ = nullptr;
      l.tail_ = nullptr;
      l.list_size_ = 0;
    }
    return *this;
  }

  // Деструктор
  ~list() {
    clear();
    if (head_) destroy_node(head_);
    if (tail_) destroy_node(tail_);
  }

  class ListIterator {
   private:
    friend list;
    Node* ptr_;
    explicit ListIterator(Node* ptr) : ptr_(ptr) {}

   public:
    ListIterator() : ptr_(nullptr) {}
    ~ListIterator() = default;

    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;

    reference operator*() { return ptr_->data_; }
    pointer operator->() { return &ptr_->data_; }  // Добавить

    iterator& operator++() {
      ptr_ = ptr_->next_;
      return *this;
    }

    iterator operator++(int) {  // Постфиксный инкремент
      iterator old = *this;
      ++(*this);
      return old;
    }

    iterator& operator--() {
      ptr_ = ptr_->prev_;
      return *this;
    }

    iterator operator--(int) {  // Постфиксный декремент
      iterator old = *this;
      --(*this);
      return old;
    }

    bool operator==(const iterator& other) const { return ptr_ == other.ptr_; }
    bool operator!=(const iterator& other) const { return ptr_ != other.ptr_; }
  };

  class ListConstIterator {
   private:
    friend list;
    const Node* ptr_;
    explicit ListConstIterator(const Node* ptr) noexcept : ptr_(ptr) {}

   public:
    ListConstIterator() : ptr_(nullptr) {}
    ListConstIterator(const iterator& other) noexcept : ptr_(other.ptr_) {}
    ~ListConstIterator() = default;

    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    const_reference operator*() const { return ptr_->data_; }
    const_reference operator->() const { return ptr_->data_; }

    const_iterator& operator++() {
      ptr_ = ptr_->next_;
      return *this;
    }

    const_iterator operator++(int) {  // Постфиксный инкремент
      const_iterator old = *this;
      ++(*this);
      return old;
    }

    const_iterator& operator--() {
      ptr_ = ptr_->prev_;
      return *this;
    }

    const_iterator operator--(int) {  // Постфиксный декремент
      const_iterator old = *this;
      --(*this);
      return old;
    }

    bool operator==(const const_iterator& other) const {
      return ptr_ == other.ptr_;
    }
    bool operator!=(const const_iterator& other) const {
      return ptr_ != other.ptr_;
    }
  };

  // Итераторы
  iterator begin() noexcept { return iterator(head_->next_); }
  iterator end() noexcept { return iterator(tail_); }
  const_iterator begin() const noexcept { return const_iterator(head_->next_); }
  const_iterator end() const noexcept { return const_iterator(tail_); }

  // Доступ к элементам
  const_reference front() const {
    if (empty()) throw std::out_of_range("list::front(): list is empty");
    return head_->next_->data_;
  }

  reference front() {
    if (empty()) throw std::out_of_range("list::front(): list is empty");
    return head_->next_->data_;
  }

  const_reference back() const {
    if (empty()) throw std::out_of_range("list::back(): list is empty");
    return tail_->prev_->data_;
  }

  reference back() {
    if (empty()) throw std::out_of_range("list::back(): list is empty");
    return tail_->prev_->data_;
  }

  bool empty() const { return list_size_ == 0; }
  size_type size() const { return list_size_; }
  size_type max_size() const noexcept {
    return node_alloc_traits::max_size(node_alloc_);
  }

  // Вставка
  void push_back(const_reference value) { insert(end(), value); }
  void push_front(const_reference value) { insert(begin(), value); }

  // Удаление
  void pop_back() { erase(--end()); }
  void pop_front() { erase(begin()); }

  void clear() {
    while (!empty()) pop_front();
  }

  void swap(list& other) noexcept {
    using std::swap;
    swap(head_, other.head_);
    swap(tail_, other.tail_);
    swap(list_size_, other.list_size_);
    swap(node_alloc_, other.node_alloc_);
  }

  // Вставка элемента
  iterator insert(iterator pos, const_reference value) {
    Node* old_node = pos.ptr_;
    Node* new_node = create_node(value);

    new_node->prev_ = old_node->prev_;
    new_node->next_ = old_node;

    old_node->prev_->next_ = new_node;
    old_node->prev_ = new_node;

    ++list_size_;
    return iterator(new_node);
  }

  // Удаление элемента
  iterator erase(iterator pos) {
    Node* rm_node = pos.ptr_;

    if (rm_node == head_ || rm_node == tail_ || list_size_ == 0) {
      throw std::out_of_range("Invalid iterator: cannot erase sentinel node");
    }

    Node* next_node = rm_node->next_;
    Node* prev_node = rm_node->prev_;

    prev_node->next_ = next_node;
    next_node->prev_ = prev_node;

    destroy_node(rm_node);
    --list_size_;

    return iterator(next_node);
  }

  void reverse() {
    if (list_size_ <= 1) return;

    Node* curr = head_->next_;
    while (curr != tail_) {
      std::swap(curr->next_, curr->prev_);
      curr = curr->prev_;
    }

    std::swap(head_->next_, tail_->prev_);
    head_->next_->prev_ = head_;
    tail_->prev_->next_ = tail_;
  }

  void unique() {
    if (list_size_ < 2) return;

    iterator curr = begin();
    while (curr != end()) {
      iterator next = curr;
      ++next;
      while (next != end() && *curr == *next) {
        next = erase(next);
      }
      curr = next;
    }
  }

  void splice(const_iterator pos, list& other) {
    if (other.empty() || this == &other) return;

    Node* first = other.head_->next_;
    Node* last = other.tail_->prev_;
    Node* position = const_cast<Node*>(pos.ptr_);

    other.head_->next_ = other.tail_;
    other.tail_->prev_ = other.head_;

    first->prev_ = position->prev_;
    position->prev_->next_ = first;

    last->next_ = position;
    position->prev_ = last;

    list_size_ += other.list_size_;
    other.list_size_ = 0;
  }

  void sort() {
    if (size() <= 1) return;

    for (Node* i = head_->next_; i != tail_->prev_; i = i->next_) {
      Node* min = i;
      for (Node* j = i->next_; j != tail_; j = j->next_) {
        if (j->data_ < min->data_) min = j;
      }

      if (min != i) {
        std::swap(i->data_, min->data_);
      }
    }
  }

  void merge(list& other) {
    if (this == &other || other.empty()) return;

    Node* this_curr = head_->next_;
    Node* other_curr = other.head_->next_;

    while (other_curr != other.tail_) {
      Node* next_other = other_curr->next_;

      while (this_curr != tail_ && this_curr->data_ <= other_curr->data_) {
        this_curr = this_curr->next_;
      }

      other_curr->prev_->next_ = other_curr->next_;
      other_curr->next_->prev_ = other_curr->prev_;

      other_curr->next_ = this_curr;
      other_curr->prev_ = this_curr->prev_;
      this_curr->prev_->next_ = other_curr;
      this_curr->prev_ = other_curr;

      ++list_size_;
      --other.list_size_;

      other_curr = next_other;
    }

    other.head_->next_ = other.tail_;
    other.tail_->prev_ = other.head_;
  }

  template <typename... Args>
  iterator insert_many(const_iterator pos, Args&&... args) {
    if constexpr (sizeof...(args) == 0)
      return iterator(const_cast<Node*>(pos.ptr_));

    s21::vector<value_type> values = {std::forward<Args>(args)...};
    iterator tmp_pos = iterator(const_cast<Node*>(pos.ptr_));

    for (const auto& value : values) {
      tmp_pos = insert(tmp_pos, value);
      ++tmp_pos;
    }

    return --tmp_pos;
  }

  template <typename... Args>
  void insert_many_front(Args&&... args) {
    insert_many(begin(), std::forward<Args>(args)...);
  }

  template <typename... Args>
  void insert_many_back(Args&&... args) {
    insert_many(end(), std::forward<Args>(args)...);
  }

  allocator_type get_allocator() const { return allocator_type(node_alloc_); }
};

}  // namespace s21

#endif