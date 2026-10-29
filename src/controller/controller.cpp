#include "controller.h"

namespace s21 {

bool Controller::IsGraphLoaded() const noexcept { return graph_.Size() > 0; }

int Controller::GetGraphSize() const noexcept { return graph_.Size(); }

bool Controller::LoadGraph(const std::string& filename) {
  return graph_.LoadGraphFromFile(filename) == 0;
}

bool Controller::ExportGraph(const std::string& filename) const {
  return graph_.ExportGraphToDot(filename) == 0;
}

std::vector<int> Controller::Bfs(int start_vertex) const {
  return GraphAlgorithms::BreadthFirstSearch(graph_, start_vertex);
}

std::vector<int> Controller::Dfs(int start_vertex) const {
  return GraphAlgorithms::DepthFirstSearch(graph_, start_vertex);
}

int Controller::ShortestPath(int vertex1, int vertex2) const {
  return GraphAlgorithms::GetShortestPathBetweenVertices(graph_, vertex1,
                                                         vertex2);
}

std::vector<std::vector<int>> Controller::AllShortestPaths() const {
  return GraphAlgorithms::GetShortestPathsBetweenAllVertices(graph_);
}

std::vector<std::vector<int>> Controller::LeastSpanningTree() const {
  return GraphAlgorithms::GetLeastSpanningTree(graph_);
}

TsmResultView Controller::SolveTsp() const {
  return GraphAlgorithms::SolveTravelingSalesmanProblem(graph_);
}

std::vector<BenchmarkResultView> Controller::BenchmarkTsp(
    int iterations) const {
  auto res = GraphAlgorithms::BenchmarkTsp(graph_, iterations);
  return std::vector<BenchmarkResultView>(res.begin(), res.end());
}

}  // namespace s21