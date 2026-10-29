#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "graph.h"
#include "graph_algorithms.h"
#include "view_types.h"

namespace s21 {

class Controller {
 public:
  Controller() = default;
  ~Controller() = default;

  bool IsGraphLoaded() const noexcept;
  int GetGraphSize() const noexcept;

  bool LoadGraph(const std::string& filename);
  bool ExportGraph(const std::string& filename) const;

  std::vector<int> Bfs(int start_vertex) const;
  std::vector<int> Dfs(int start_vertex) const;
  int ShortestPath(int vertex1, int vertex2) const;
  std::vector<std::vector<int>> AllShortestPaths() const;
  std::vector<std::vector<int>> LeastSpanningTree() const;
  TsmResultView SolveTsp() const;
  std::vector<BenchmarkResultView> BenchmarkTsp(int iterations) const;

 private:
  Graph graph_;
};

}  // namespace s21

#endif  // CONTROLLER_H