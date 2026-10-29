#ifndef GRAPH_ALGORITHMS_H
#define GRAPH_ALGORITHMS_H

#include <functional>
#include <vector>

#include "Queue.h"
#include "Stack.h"
#include "graph.h"

namespace s21 {

struct TsmResult {
  std::vector<int> vertices;
  double distance = 0.0;
};

struct BenchmarkResult {
  std::string algorithm_name;
  double time_us = 0.0;
  double best_distance = 0.0;
};

class GraphAlgorithms {
 public:
  GraphAlgorithms() = delete;

  static std::vector<int> DepthFirstSearch(const Graph& graph,
                                           int start_vertex);
  static std::vector<int> BreadthFirstSearch(const Graph& graph,
                                             int start_vertex);
  static int GetShortestPathBetweenVertices(const Graph& graph, int vertex1,
                                            int vertex2);
  static std::vector<std::vector<int>> GetShortestPathsBetweenAllVertices(
      const Graph& graph);
  static std::vector<std::vector<int>> GetLeastSpanningTree(const Graph& graph);
  static TsmResult SolveTravelingSalesmanProblem(const Graph& graph);
  static TsmResult SolveBruteForce(const Graph& graph);
  static TsmResult SolveGreedy(const Graph& graph);
  static std::vector<BenchmarkResult> BenchmarkTsp(const Graph& graph,
                                                   int iterations);

 private:
  template <typename Container, typename NeighborTransform = std::identity>
  static std::vector<int> Traverse(
      const Graph& graph, int start_vertex,
      NeighborTransform transform = std::identity{});
};

}  // namespace s21

#endif  // GRAPH_ALGORITHMS_H