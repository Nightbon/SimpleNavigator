#include "view.h"

#include <algorithm>
#include <format>
#include <iomanip>
#include <iostream>
#include <limits>
#include <ranges>

namespace {
int ComputeColumnWidth(const std::vector<std::vector<int>>& data) {
  const int kInf = std::numeric_limits<int>::max();
  int max_val = 0;
  for (const auto& v : data | std::views::join) {
    if (v != kInf && v > max_val) {
      max_val = v;
    }
  }
  int width = 1;
  int tmp = max_val / 10;
  while (tmp > 0) {
    ++width;
    tmp /= 10;
  }
  return width;
}

void ShowBenchmark(const std::vector<s21::BenchmarkResultView>& results,
                   int iterations) {
  size_t name_width = 0;
  for (const auto& r : results) {
    name_width = std::max(name_width, r.algorithm_name.size());
  }

  constexpr int num_width = 14;
  int total_width = static_cast<int>(name_width) + 2 + num_width * 3;

  std::cout << std::format("  {:<{}}{:>{}}{:>{}}{:>{}}\n", "Algorithm",
                           name_width + 2, "Total (ms)", num_width, "Avg (ms)",
                           num_width, "Best dist", num_width);

  std::cout << std::format("  {:-<{}}\n", "", total_width);

  for (const auto& r : results) {
    double avg = r.time_us / static_cast<double>(iterations);

    std::cout << std::format("  {:<{}}{:>{}.3f}{:>{}.3f}{:>{}.0f}\n",
                             r.algorithm_name, name_width + 2, r.time_us,
                             num_width, avg, num_width, r.best_distance,
                             num_width);
  }
}

void ShowMatrix(const std::vector<std::vector<int>>& data) {
  if (data.empty()) {
    std::cout << std::format("\033[31m: (empty)\n\033[0m");
    return;
  }

  constexpr int kInf = std::numeric_limits<int>::max();
  int width = ComputeColumnWidth(data) + 2;

  std::cout << "    ";
  for (auto j : std::views::iota(0u, data[0].size())) {
    std::cout << std::format("\033[32m{:>{}}\033[0m", j + 1, width);
  }
  std::cout << "\n";

  for (auto i : std::views::iota(0u, data.size())) {
    std::cout << std::format(" \033[32m{:>3}\033[0m ", i + 1);

    for (auto j : std::views::iota(0u, data[i].size())) {
      int val = data[i][j];

      if (val == kInf) {
        std::cout << std::format("{:>{}}", "inf", width);
      } else {
        std::cout << std::format("{:>{}}", val, width);
      }
    }
    std::cout << "\n";
  }
}

void ShowMenu() {
  std::cout << R"(  
  ----------------------------------------
  Graph Algorithms Console
  ----------------------------------------
  1. Load graph from file
  2. Breadth-First Search (BFS)
  3. Depth-First Search (DFS)
  4. Shortest path between two vertices (Dijkstra)
  5. Shortest paths between all pairs (Floyd-Warshall)
  6. Minimum Spanning Tree (Prim)
  7. Traveling Salesman Problem (Ant Colony)
  8. TSP Benchmark (Ant Colony, Greedy, Brute Force)
  9. Save graph to file
  0. Exit
  ----------------------------------------
> )";
  std::cout.flush();
}

void ShowMessage(const std::string& msg) { std::cout << msg << "\n"; }

void ShowError(const std::string& msg) {
  std::cerr << "  [ERROR] " << msg << "\n";
}

void ShowVector(const std::vector<int>& data) {
  if (data.empty()) {
    std::cout << "\033[31m" << ": (empty)\n"
              << "\033[0m";
  } else {
    for (auto i : std::views::iota(0u, data.size())) {
      if (i > 0) std::cout << "-";
      std::cout << data[i];
    }
  }
  std::cout << "\n";
}

void ShowTspResult(const s21::TsmResultView& result) {
  std::cout << "  sequence of traversing vertices: ";
  ShowVector(result.vertices);
  std::cout << "  route length: " << result.distance << "\n";
}

void ShowGraphInfo(int vertex_count) {
  std::cout << "  Graph loaded: " << vertex_count << " vertices\n";
}

}  // namespace

namespace s21 {

View::View(Controller& controller) : controller_(controller) {}

void View::Run() const {
  while (true) {
    ShowMenu();
    int choice = 0;
    std::cin >> choice;

    switch (choice) {
      case 1:
        HandleLoad();
        break;
      case 2:
        HandleBfsDfs("Bfs");
        break;
      case 3:
        HandleBfsDfs("Dfs");
        break;
      case 4:
        HandleShortestPath();
        break;
      case 5:
        HandleAllShortestPaths();
        break;
      case 6:
        HandleMst();
        break;
      case 7:
        HandleTsp();
        break;
      case 8:
        HandleBenchmark();
        break;
      case 9:
        HandleExport();
        break;
      case 0:
        ShowMessage("Goodbye!");
        return;
    }
  }
}

bool View::EnsureGraphLoaded() const {
  if (!controller_.IsGraphLoaded()) {
    ShowError("No graph loaded");
    return false;
  }
  return true;
}

void View::HandleLoad() const {
  std::cout << " Enter file path: ";
  std::cout.flush();
  std::string path{};
  std::cin >> path;

  if (controller_.LoadGraph(path)) {
    ShowGraphInfo(controller_.GetGraphSize());
  } else {
    ShowError("Loading false");
  }
}

void View::HandleExport() const {
  if (!EnsureGraphLoaded()) return;

  std::cout << " Enter file path: ";
  std::cout.flush();
  std::string path{};
  std::cin >> path;

  if (controller_.ExportGraph(path)) {
    ShowMessage("Export is done");
  } else {
    ShowError("Exporting false");
  }
}

void View::HandleBfsDfs(const std::string& alg) const {
  if (!EnsureGraphLoaded()) return;

  std::cout << "  Enter start vertex: ";
  std::cout.flush();
  int start = 0;
  std::cin >> start;
  std::vector<int> result;
  if (alg == "Bfs") {
    result = controller_.Bfs(start);
  } else {
    result = controller_.Dfs(start);
  }

  ShowVector(result);
}

void View::HandleShortestPath() const {
  if (!EnsureGraphLoaded()) return;

  std::cout << "  Enter start vertex: ";
  std::cout.flush();
  int v1 = 0;
  std::cin >> v1;
  std::cout << "  Enter end vertex: ";
  std::cout.flush();
  int v2 = 0;
  std::cin >> v2;

  int dist = controller_.ShortestPath(v1, v2);

  if (dist == 0 && v1 != v2) {
    ShowMessage("  No path exists between " + std::to_string(v1) + " and " +
                std::to_string(v2));
  } else {
    ShowMessage("  Shortest path from " + std::to_string(v1) + " to " +
                std::to_string(v2) + ": " + std::to_string(dist));
  }
}

void View::HandleAllShortestPaths() const {
  if (!EnsureGraphLoaded()) return;

  auto matrix = controller_.AllShortestPaths();
  ShowMatrix(matrix);
}

void View::HandleMst() const {
  if (!EnsureGraphLoaded()) return;

  auto mst = controller_.LeastSpanningTree();
  ShowMatrix(mst);
}

void View::HandleTsp() const {
  if (!EnsureGraphLoaded()) return;

  auto result = controller_.SolveTsp();

  if (result.distance == -1) {
    ShowError("TspError");
  } else {
    ShowTspResult(result);
  }
}

void View::HandleBenchmark() const {
  if (!EnsureGraphLoaded()) return;

  std::cout << "  Enter number of iterations: ";
  std::cout.flush();
  int iter = 0;
  std::cin >> iter;

  ShowMessage("  Running benchmark...");
  ShowMessage("");

  auto results = controller_.BenchmarkTsp(iter);

  if (std::ranges::any_of(
          results, [](const auto& v) { return v == -1; },
          &BenchmarkResultView::best_distance)) {
    ShowError("TspError");
    return;
  }

  ShowBenchmark(results, iter);
  ShowMessage("");
}

}  // namespace s21