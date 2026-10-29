#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>

namespace s21 {

class Graph {
 public:
  Graph() noexcept = default;
  ~Graph() noexcept = default;

  int LoadGraphFromFile(const std::string& filename);
  int ExportGraphToDot(const std::string& filename) const;

  int Size() const noexcept { return size_; }
  bool HasEdge(int from, int to) const noexcept;
  int GetEdgeWeight(int from, int to) const noexcept;
  std::vector<int> GetNeighbors(int vertex) const;

 private:
  std::vector<std::vector<int>> adjacency_matrix_;
  int size_ = 0;
};

}  // namespace s21

#endif  // GRAPH_H