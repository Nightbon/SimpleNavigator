#include "graph.h"

#include <algorithm>
#include <format>
#include <fstream>
#include <ranges>
#include <sstream>

namespace s21 {

int Graph::LoadGraphFromFile(const std::string& filename) {
  std::ifstream file(filename);

  if (!file.is_open() || !(file >> size_) || size_ <= 0) return -1;

  adjacency_matrix_ = std::vector(size_, std::vector<int>(size_, 0));

  for (auto& weight : adjacency_matrix_ | std::views::join) {
    if (!(file >> weight) || weight < 0) return -1;
  }

  return 0;
}

int Graph::ExportGraphToDot(const std::string& filename) const {
  std::ofstream file(filename);
  if (!file.is_open()) return -1;

  auto indices = std::views::iota(0, size_);
  bool directed = std::ranges::any_of(indices, [this, indices](int i) {
    return std::ranges::any_of(
        indices | std::views::drop(i + 1), [this, i](int j) {
          return adjacency_matrix_[i][j] != adjacency_matrix_[j][i];
        });
  });

  bool weighted = std::ranges::any_of(indices, [&](int i) {
    int start_j = directed ? 0 : i + 1;

    return std::ranges::any_of(indices | std::views::drop(start_j), [&](int j) {
      return adjacency_matrix_[i][j] > 1;
    });
  });

  file << std::format("{} G {{\n", directed ? "digraph" : "graph");
  std::string_view arrow = directed ? "->" : "--";

  for (int i : indices) {
    int start_j = directed ? 0 : i + 1;
    for (int j : indices | std::views::drop(start_j)) {
      if (int w = adjacency_matrix_[i][j]; w != 0) {
        if (weighted) {
          file << std::format("  {} {} {} [label={}];\n", i + 1, arrow, j + 1,
                              w);
        } else {
          file << std::format("  {} {} {};\n", i + 1, arrow, j + 1);
        }
      }
    }
  }

  file << "}\n";

  return 0;
}

bool Graph::HasEdge(int from, int to) const noexcept {
  return GetEdgeWeight(from, to) != 0;
}

int Graph::GetEdgeWeight(int from, int to) const noexcept {
  if (from < 1 || from > size_ || to < 1 || to > size_) {
    return 0;
  }
  return adjacency_matrix_[from - 1][to - 1];
}

std::vector<int> Graph::GetNeighbors(int vertex) const {
  if (vertex < 1 || vertex > size_) {
    return {};
  }

  const auto& row = adjacency_matrix_[vertex - 1];

  auto neighbor_view =
      std::views::iota(0, size_) |
      std::views::filter([&row](int idx) { return row[idx] != 0; }) |
      std::views::transform([](int idx) { return idx + 1; });

  return std::vector<int>(neighbor_view.begin(), neighbor_view.end());
}

}  // namespace s21