#ifndef VIEW_TYPES_H
#define VIEW_TYPES_H

namespace s21 {

struct TsmResultView {
  std::vector<int> vertices;
  double distance = 0.0;

  template <typename T>
  TsmResultView(const T& other)
      : vertices(other.vertices), distance(other.distance) {}
};

struct BenchmarkResultView {
  std::string algorithm_name;
  double time_us = 0.0;
  double best_distance = 0.0;

  template <typename T>
  BenchmarkResultView(const T& other)
      : algorithm_name(other.algorithm_name),
        time_us(other.time_us),
        best_distance(other.best_distance) {}
};

}  // namespace s21

#endif  // VIEW_TYPES_H