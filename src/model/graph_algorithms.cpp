#include "graph_algorithms.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <limits>
#include <random>
#include <ranges>

#include "Queue.h"
#include "Stack.h"

namespace s21 {

std::vector<int> GraphAlgorithms::BreadthFirstSearch(const Graph& graph,
                                                     int start_vertex) {
  return Traverse<Queue>(graph, start_vertex);
}

std::vector<int> GraphAlgorithms::DepthFirstSearch(const Graph& graph,
                                                   int start_vertex) {
  return Traverse<Stack>(graph, start_vertex,
                         [](auto&& r) { return r | std::views::reverse; });
}

template <typename Container, typename NeighborTransform>
std::vector<int> GraphAlgorithms::Traverse(const Graph& graph, int start_vertex,
                                           NeighborTransform transform) {
  const size_t graph_size = graph.Size();
  auto graph_index_size = graph_size + 1;
  if (start_vertex < 1 || start_vertex > graph_size) return {};

  std::vector<bool> visited(graph_index_size, false);
  std::vector<int> result;
  result.reserve(graph_size);

  Container container;
  container.Push(start_vertex);
  visited[start_vertex] = true;

  while (!container.Empty()) {
    int current = container.Pop();
    result.push_back(current);

    for (auto neighbors = graph.GetNeighbors(current);
         int neighbor : transform(neighbors)) {
      if (!visited[neighbor]) {
        visited[neighbor] = true;
        container.Push(neighbor);
      }
    }
  }
  return result;
}

int GraphAlgorithms::GetShortestPathBetweenVertices(const Graph& graph,
                                                    int vertex1, int vertex2) {
  const int graph_size = graph.Size();
  auto graph_index_size = graph_size + 1;
  if (vertex1 < 1 || vertex1 > graph_size || vertex2 < 1 ||
      vertex2 > graph_size) {
    return 0;
  }
  if (vertex1 == vertex2) {
    return 0;
  }

  constexpr int kInf = std::numeric_limits<int>::max();

  std::vector<int> dist(graph_index_size, kInf);
  std::vector<bool> visited(graph_index_size, false);

  dist[vertex1] = 0;

  for (int _ : std::views::iota(0, graph_size)) {
    auto unvisited =
        std::views::iota(1, graph_index_size) |
        std::views::filter([&visited](int v) { return !visited[v]; });

    auto it = std::ranges::min_element(unvisited, std::less<>{},
                                       [&dist](int v) { return dist[v]; });

    if (it == unvisited.end()) {
      break;
    }

    int u = *it;
    if (u == vertex2) {
      break;
    }

    visited[u] = true;

    for (auto neighbors = graph.GetNeighbors(u); int neighbor : neighbors) {
      if (!visited[neighbor]) {
        int w = graph.GetEdgeWeight(u, neighbor);
        int du = dist[u];
        int& dn = dist[neighbor];
        if (du != kInf && du + w < dn) {
          dn = du + w;
        }
      }
    }
  }

  int result = dist[vertex2];
  return (result == kInf) ? 0 : result;
}

std::vector<std::vector<int>>
GraphAlgorithms::GetShortestPathsBetweenAllVertices(const Graph& graph) {
  const int graph_size = graph.Size();
  auto graph_index_size = graph_size + 1;
  if (graph_size == 0) {
    return {};
  }

  constexpr int kInf = std::numeric_limits<int>::max();

  std::vector<std::vector<int>> dist(graph_index_size,
                                     std::vector<int>(graph_index_size, kInf));

  for (int i : std::views::iota(1, graph_index_size)) {
    dist[i][i] = 0;
    for (int j : std::views::iota(1, graph_index_size)) {
      if (i != j && graph.HasEdge(i, j)) {
        dist[i][j] = graph.GetEdgeWeight(i, j);
      }
    }
  }

  for (int k : std::views::iota(1, graph_index_size)) {
    for (int i : std::views::iota(1, graph_index_size)) {
      for (int j : std::views::iota(1, graph_index_size)) {
        int dik = dist[i][k];
        int dkj = dist[k][j];
        if (dik != kInf && dkj != kInf && (dik + dkj < dist[i][j])) {
          dist[i][j] = dik + dkj;
        }
      }
    }
  }

  std::vector<std::vector<int>> result(graph_size,
                                       std::vector<int>(graph_size));
  for (int i : std::views::iota(0, graph_size)) {
    for (int j : std::views::iota(0, graph_size)) {
      result[i][j] = dist[i + 1][j + 1];
    }
  }

  return result;
}

std::vector<std::vector<int>> GraphAlgorithms::GetLeastSpanningTree(
    const Graph& graph) {
  const int graph_size = graph.Size();
  auto graph_index_size = graph_size + 1;
  auto graph_steps = graph_size - 1;
  if (graph_size == 0) {
    return {};
  }

  std::vector<std::vector<int>> mst(graph_size,
                                    std::vector<int>(graph_size, 0));
  std::vector<bool> in_mst(graph_index_size, false);
  in_mst[1] = true;

  for (int step : std::views::iota(0, graph_steps)) {
    int best_from = -1;
    int best_to = -1;
    int best_weight = std::numeric_limits<int>::max();

    for (int u :
         std::views::iota(1, graph_index_size) |
             std::views::filter([&in_mst](int v) { return in_mst[v]; })) {
      auto neighbors = graph.GetNeighbors(u);
      for (int neighbor : neighbors | std::views::filter([&in_mst](int v) {
                            return !in_mst[v];
                          })) {
        int w = graph.GetEdgeWeight(u, neighbor);
        if (w < best_weight) {
          best_weight = w;
          best_from = u;
          best_to = neighbor;
        }
      }
    }

    if (best_from == -1) {
      break;
    }

    mst[best_from - 1][best_to - 1] = best_weight;
    mst[best_to - 1][best_from - 1] = best_weight;

    in_mst[best_to] = true;
  }

  return mst;
}

TsmResult GraphAlgorithms::SolveTravelingSalesmanProblem(const Graph& graph) {
  const int graph_size = graph.Size();
  auto graph_index_size = graph_size + 1;
  auto graph_steps = graph_size - 1;

  if (graph_size < 2) return {{}, -1.0};

  auto reachable = BreadthFirstSearch(graph, 1);
  if (reachable.size() != graph_size) return {{}, -1.0};

  constexpr double kAlpha = 1.0;   // Влияние феромона.
  constexpr double kBeta = 0.5;    // Влияние эвристики.
  constexpr double kEvap = 0.01;   // Коэффициент испарения.
  constexpr double kQ = 250.0;     // Константа отложения.
  constexpr int kIterations = 50;  // Число итераций.
  constexpr int kAnts = 30;  // Число муравьев на итерацию.
  constexpr int kStartVertex = 1;  // Стартовая вершина.

  std::mt19937 rng(std::random_device{}());
  std::uniform_real_distribution<double> prob_dist(0.0, 1.0);

  std::vector weight(graph_index_size, std::vector<int>(graph_index_size, 0));
  std::vector pheromone(graph_index_size,
                        std::vector<double>(graph_index_size, 0.0));
  std::vector eta_pow_beta(graph_index_size,
                           std::vector<double>(graph_index_size, 0.0));
  std::vector delta(graph_index_size,
                    std::vector<double>(graph_index_size, 0.0));
  std::vector<bool> visited(graph_index_size, false);
  std::vector<int> tour(graph_index_size, 0);

  for (int i : std::views::iota(1, graph_index_size)) {
    for (int j : std::views::iota(1, graph_index_size)) {
      if (i != j && graph.HasEdge(i, j)) {
        int w = graph.GetEdgeWeight(i, j);
        weight[i][j] = w;
        pheromone[i][j] = 1.0;
        eta_pow_beta[i][j] = std::pow(1.0 / w, kBeta);
      }
    }
  }

  TsmResult best;
  best.distance = std::numeric_limits<double>::max();

  for (int iter : std::views::iota(0, kIterations)) {
    std::ranges::for_each(delta,
                          [](auto& row) { std::ranges::fill(row, 0.0); });

    for (int ant : std::views::iota(0, kAnts)) {
      std::fill(visited.begin(), visited.end(), false);
      tour.clear();

      tour.push_back(kStartVertex);
      visited[kStartVertex] = true;
      int current = kStartVertex;
      bool stuck = false;

      for (int step : std::views::iota(0, graph_steps)) {
        std::vector<std::pair<int, double>> candidates;
        double total = 0.0;

        for (int next : std::views::iota(1, graph_index_size)) {
          if (visited[next] || weight[current][next] == 0) continue;
          double tau = std::pow(pheromone[current][next], kAlpha);
          double p = tau * eta_pow_beta[current][next];
          candidates.emplace_back(next, p);
          total += p;
        }

        if (candidates.empty()) {
          stuck = true;
          break;
        }

        double r = prob_dist(rng) * total;
        double sum_p = 0.0;
        int chosen = candidates.back().first;
        for (const auto& [v, p] : candidates) {
          sum_p += p;
          if (r <= sum_p) {
            chosen = v;
            break;
          }
        }

        tour.push_back(chosen);
        visited[chosen] = true;
        current = chosen;
      }

      if (stuck || weight[current][kStartVertex] == 0) continue;

      int tour_len = 0;
      for (int i : std::views::iota(0, graph_steps)) {
        tour_len += weight[tour[i]][tour[i + 1]];
      }
      tour_len += weight[tour[graph_steps]][kStartVertex];

      double deposit = kQ / tour_len;
      for (int i : std::views::iota(0, graph_steps)) {
        delta[tour[i]][tour[i + 1]] += deposit;
      }
      delta[tour[graph_steps]][kStartVertex] += deposit;

      if (tour_len < best.distance) {
        best.distance = tour_len;
        best.vertices = tour;
        best.vertices.push_back(kStartVertex);
      }
    }

    for (int i : std::views::iota(1, graph_index_size)) {
      for (int j : std::views::iota(1, graph_index_size)) {
        pheromone[i][j] = pheromone[i][j] * (1.0 - kEvap) + delta[i][j];
      }
    }
  }

  if (best.vertices.empty()) return {{}, -1.0};

  return best;
}

TsmResult GraphAlgorithms::SolveGreedy(const Graph& graph) {
  const int graph_size = graph.Size();
  auto graph_index_size = graph_size + 1;
  auto graph_steps = graph_size - 1;

  if (graph_size < 2) return {{}, -1.0};

  auto reachable = BreadthFirstSearch(graph, 1);
  if (reachable.size() != graph_size) return {{}, -1.0};

  constexpr size_t kStartVertex = 1;

  std::vector<bool> visited(graph_index_size, false);

  std::vector weight(graph_index_size, std::vector<int>(graph_index_size, 0));
  for (auto i : std::views::iota(1, graph_index_size)) {
    for (auto j : std::views::iota(1, graph_index_size)) {
      if (i != j && graph.HasEdge(i, j)) {
        weight[i][j] = (graph.GetEdgeWeight(i, j));
      }
    }
  }

  TsmResult best;
  best.distance = 0.0;
  best.vertices.reserve(graph_index_size);

  best.vertices.push_back(kStartVertex);
  visited[kStartVertex] = true;
  int current = kStartVertex;

  for (auto step : std::views::iota(0, graph_steps)) {
    int next_vertex = 0;
    int min_weight = std::numeric_limits<int>::max();

    for (auto next : std::views::iota(1, graph_index_size)) {
      if (!visited[next] && weight[current][next] > 0 &&
          weight[current][next] < min_weight) {
        min_weight = weight[current][next];
        next_vertex = next;
      }
    }

    if (next_vertex == 0) return {{}, -1.0};

    best.distance += min_weight;
    best.vertices.push_back(next_vertex);
    visited[next_vertex] = true;
    current = next_vertex;
  }

  if (weight[current][kStartVertex] == 0) return {{}, -1.0};

  best.distance += weight[current][kStartVertex];
  best.vertices.push_back(kStartVertex);

  return best;
}

TsmResult GraphAlgorithms::SolveBruteForce(const Graph& graph) {
  const int graph_size = graph.Size();
  auto graph_index_size = graph_size + 1;
  auto graph_steps = graph_size - 1;

  if (graph_size < 2) return {{}, -1.0};

  auto reachable = BreadthFirstSearch(graph, 1);
  if (reachable.size() != graph_size) return {{}, -1.0};

  constexpr int kStartVertex = 1;

  std::vector weight(graph_index_size, std::vector<int>(graph_index_size, 0));
  for (auto i : std::views::iota(1, graph_index_size)) {
    for (auto j : std::views::iota(1, graph_index_size)) {
      if (i != j && graph.HasEdge(i, j)) {
        weight[i][j] = graph.GetEdgeWeight(i, j);
      }
    }
  }

  std::vector<int> perm;
  perm.reserve(graph_index_size);
  perm.push_back(kStartVertex);
  std::ranges::copy(std::views::iota(2, graph_index_size),
                    std::back_inserter(perm));
  perm.push_back(kStartVertex);

  TsmResult best;
  best.distance = std::numeric_limits<double>::max();

  do {
    int current_len = 0;
    bool stuck = false;

    for (auto i : std::views::iota(0u, perm.size() - 1)) {
      if (weight[perm[i]][perm[i + 1]] == 0) {
        stuck = true;
        break;
      }
      current_len += weight[perm[i]][perm[i + 1]];
    }

    if (stuck) continue;

    if (current_len < best.distance) {
      best.distance = current_len;
      best.vertices = perm;
    }

  } while (
      std::ranges::next_permutation(perm.begin() + 1, perm.end() - 1).found);

  if (best.vertices.empty()) return {{}, -1.0};

  return best;
}

std::vector<BenchmarkResult> GraphAlgorithms::BenchmarkTsp(const Graph& graph,
                                                           int iterations) {
  std::vector<BenchmarkResult> results;
  results.reserve(3);

  struct Algorithm {
    std::string name;
    std::function<TsmResult(const Graph&)> solve;
  };

  std::vector<Algorithm> algos = {
      {"Greedy",
       [](const Graph& g) { return GraphAlgorithms::SolveGreedy(g); }},
      {"Ant Colony",
       [](const Graph& g) {
         return GraphAlgorithms::SolveTravelingSalesmanProblem(g);
       }},
      {"Brute Force",
       [](const Graph& g) { return GraphAlgorithms::SolveBruteForce(g); }},
  };

  for (auto& algo : algos) {
    TsmResult best;
    best.distance = std::numeric_limits<double>::max();

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int _ : std::views::iota(0, iterations)) {
      TsmResult result = algo.solve(graph);
      if (result.distance < best.distance) {
        best = result;
      }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double, std::milli>(end_time - start_time)
                    .count();

    results.push_back({algo.name, ms, best.distance});
  }

  return results;
}

}  // namespace s21