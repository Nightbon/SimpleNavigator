#include <gtest/gtest.h>

#include <algorithm>
#include <climits>
#include <cmath>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

#include "Queue.h"
#include "Stack.h"
#include "graph.h"
#include "graph_algorithms.h"

namespace {

// ─── Вспомогательная функция для создания временного файла ─────────────────
std::string WriteTempGraph(const std::string& file_name,
                           const std::string& content) {
  const std::string path = ::testing::TempDir() + file_name;

  std::ofstream f(path, std::ios::out | std::ios::trunc);
  if (!f) {
    ADD_FAILURE() << "Не удалось открыть файл для записи: " << path;
    return path;
  }

  f << content;
  f.flush();

  if (!f) {
    ADD_FAILURE() << "Не удалось записать граф в файл: " << path;
  }

  return path;
}

// ─── Вспомогательная функция для чтения файла ──────────────────────────────
std::string ReadFile(const std::string& path) {
  std::ifstream f(path);
  if (!f) {
    ADD_FAILURE() << "Не удалось открыть файл для чтения: " << path;
    return {};
  }

  return std::string((std::istreambuf_iterator<char>(f)),
                     std::istreambuf_iterator<char>());
}

}  // namespace

// ─── Тест: stack (Push / Pop / Top) ─────────────────────────────────────────
TEST(StackTest, PushPopTop) {
  s21::Stack st;

  EXPECT_TRUE(st.Empty());
  EXPECT_EQ(st.Size(), 0u);

  st.Push(10);
  st.Push(20);
  st.Push(30);

  EXPECT_FALSE(st.Empty());
  EXPECT_EQ(st.Size(), 3u);

  EXPECT_EQ(st.Top(), 30);
  EXPECT_EQ(st.Pop(), 30);

  EXPECT_EQ(st.Top(), 20);
  EXPECT_EQ(st.Pop(), 20);

  EXPECT_EQ(st.Pop(), 10);
  EXPECT_TRUE(st.Empty());
}

// ─── Тест: queue (Push / Pop / Front / Back) ────────────────────────────────
TEST(QueueTest, PushPopFrontBack) {
  s21::Queue q;

  EXPECT_TRUE(q.Empty());
  EXPECT_EQ(q.Size(), 0u);

  q.Push(1);
  q.Push(2);
  q.Push(3);

  EXPECT_FALSE(q.Empty());
  EXPECT_EQ(q.Size(), 3u);

  EXPECT_EQ(q.Front(), 1);
  EXPECT_EQ(q.Back(), 3);

  EXPECT_EQ(q.Pop(), 1);
  EXPECT_EQ(q.Front(), 2);
  EXPECT_EQ(q.Pop(), 2);
  EXPECT_EQ(q.Pop(), 3);

  EXPECT_TRUE(q.Empty());
}

// ─── Тест: LoadGraphFromFile + GetNeighbors ─────────────────────────────────
TEST(GraphTest, LoadGraphFromFileAndGetNeighbors) {
  /*
    Граф:

    1 -- 2
    |    |
    3 -- 4
    |
    5
  */
  const std::string graph_data =
      "5\n"
      "0 1 1 0 0\n"
      "1 0 0 1 0\n"
      "1 0 0 1 1\n"
      "0 1 1 0 0\n"
      "0 0 1 0 0\n";

  const auto path = WriteTempGraph("load_graph.txt", graph_data);

  s21::Graph g;
  ASSERT_NO_THROW(g.LoadGraphFromFile(path));

  EXPECT_EQ(g.Size(), 5u);

  EXPECT_TRUE(g.HasEdge(1, 2));
  EXPECT_TRUE(g.HasEdge(1, 3));
  EXPECT_FALSE(g.HasEdge(1, 4));
  EXPECT_TRUE(g.HasEdge(3, 5));

  EXPECT_EQ(g.GetEdgeWeight(1, 2), 1);

  EXPECT_EQ(g.GetEdgeWeight(-1, 1), 0);
  EXPECT_EQ(g.GetEdgeWeight(6, 1), 0);
  EXPECT_EQ(g.GetEdgeWeight(1, -1), 0);
  EXPECT_EQ(g.GetEdgeWeight(1, 6), 0);

  auto n1 = g.GetNeighbors(1);
  EXPECT_EQ(n1, (std::vector<int>{2, 3}));

  auto n3 = g.GetNeighbors(3);
  EXPECT_EQ(n3, (std::vector<int>{1, 4, 5}));

  auto n5 = g.GetNeighbors(5);
  EXPECT_EQ(n5, (std::vector<int>{3}));

  auto n6 = g.GetNeighbors(-6);
  EXPECT_TRUE(n6.empty());

  auto n7 = g.GetNeighbors(7);
  EXPECT_TRUE(n7.empty());
}

// ─── Тест: ExportGraphToDot ──────────────────────────────────────────────────
TEST(GraphTest, ExportUndirectedGraphToDot) {
  const std::string graph_data =
      "3\n"
      "0 1 1\n"
      "1 0 0\n"
      "1 0 0\n";

  const auto path = WriteTempGraph("export_dot_graph.txt", graph_data);

  s21::Graph g;
  ASSERT_NO_THROW(g.LoadGraphFromFile(path));

  const std::string dot_path = ::testing::TempDir() + "test_graph.dot";
  ASSERT_NO_THROW(g.ExportGraphToDot(dot_path));

  const std::string dot_content = ReadFile(dot_path);

  EXPECT_NE(dot_content.find("graph G"), std::string::npos);
  EXPECT_NE(dot_content.find("1 -- 2"), std::string::npos);
  EXPECT_NE(dot_content.find("1 -- 3"), std::string::npos);
  EXPECT_EQ(dot_content.find("2 -- 3"), std::string::npos);
}

// ─── Тест: DFS ──────────────────────────────────────────────────────────────
TEST(SearchTest, DepthFirstSearch) {
  /*
    Граф (дерево):

        1
       / \
      2   3
     / \   \
    4   5   6
  */
  const std::string graph_data =
      "6\n"
      "0 1 1 0 0 0\n"
      "1 0 0 1 1 0\n"
      "1 0 0 0 0 1\n"
      "0 1 0 0 0 0\n"
      "0 1 0 0 0 0\n"
      "0 0 1 0 0 0\n";

  const auto path = WriteTempGraph("dfs_graph.txt", graph_data);

  s21::Graph g;
  ASSERT_NO_THROW(g.LoadGraphFromFile(path));

  auto dfs1 = s21::GraphAlgorithms::DepthFirstSearch(g, 1);

  // Ожидаем: {1, 2, 4, 5, 3, 6}
  ASSERT_EQ(dfs1.size(), 6u);
  EXPECT_EQ(dfs1, (std::vector<int>{1, 2, 4, 5, 3, 6}));

  auto dfs4 = s21::GraphAlgorithms::DepthFirstSearch(g, 4);
  EXPECT_EQ(dfs4, (std::vector<int>{4, 2, 1, 3, 6, 5}));
}

// ─── Тест: BFS ──────────────────────────────────────────────────────────────
TEST(SearchTest, BreadthFirstSearch) {
  /*
    Граф:

    1 -- 2 -- 4
    |    |
    3 -- 5
  */
  const std::string graph_data =
      "5\n"
      "0 1 1 0 0\n"
      "1 0 0 1 1\n"
      "1 0 0 0 1\n"
      "0 1 0 0 0\n"
      "0 1 1 0 0\n";

  const auto path = WriteTempGraph("bfs_graph.txt", graph_data);

  s21::Graph g;
  ASSERT_NO_THROW(g.LoadGraphFromFile(path));

  auto bfs1 = s21::GraphAlgorithms::BreadthFirstSearch(g, 1);
  EXPECT_EQ(bfs1, (std::vector<int>{1, 2, 3, 4, 5}));

  auto bfs4 = s21::GraphAlgorithms::BreadthFirstSearch(g, 4);
  EXPECT_EQ(bfs4, (std::vector<int>{4, 2, 1, 5, 3}));
}

// ─── Тест: взвешенный ориентированный граф → DOT ────────────────────────────
TEST(GraphTest, ExportWeightedDirectedGraphToDot) {
  const std::string graph_data =
      "3\n"
      "0 5 0\n"
      "0 0 3\n"
      "7 0 0\n";

  const auto path = WriteTempGraph("weighted_directed_graph.txt", graph_data);

  s21::Graph g;
  ASSERT_NO_THROW(g.LoadGraphFromFile(path));

  const std::string dot_path =
      ::testing::TempDir() + "test_weighted_directed.dot";

  ASSERT_NO_THROW(g.ExportGraphToDot(dot_path));

  const std::string dot_content = ReadFile(dot_path);

  EXPECT_NE(dot_content.find("digraph G"), std::string::npos);
  EXPECT_NE(dot_content.find("1 -> 2 [label=5]"), std::string::npos);
  EXPECT_NE(dot_content.find("2 -> 3 [label=3]"), std::string::npos);
  EXPECT_NE(dot_content.find("3 -> 1 [label=7]"), std::string::npos);
}

// ─── Тест: Дейкстра (невзвешенный граф) ────────────────────────────────────
TEST(ShortestPathsTest, DijkstraUnweighted) {
  /*
    Граф:

    1 -- 2 -- 4
    |    |
    3 -- 5
  */
  const std::string graph_data =
      "5\n"
      "0 1 1 0 0\n"
      "1 0 0 1 1\n"
      "1 0 0 0 1\n"
      "0 1 0 0 0\n"
      "0 1 1 0 0\n";

  const auto path = WriteTempGraph("dijkstra_unweighted_graph.txt", graph_data);

  s21::Graph g;
  ASSERT_NO_THROW(g.LoadGraphFromFile(path));

  EXPECT_EQ(s21::GraphAlgorithms::GetShortestPathBetweenVertices(g, 1, 1), 0);
  EXPECT_EQ(s21::GraphAlgorithms::GetShortestPathBetweenVertices(g, 1, 2), 1);
  EXPECT_EQ(s21::GraphAlgorithms::GetShortestPathBetweenVertices(g, 1, 4), 2);
  EXPECT_EQ(s21::GraphAlgorithms::GetShortestPathBetweenVertices(g, 1, 5), 2);
  EXPECT_EQ(s21::GraphAlgorithms::GetShortestPathBetweenVertices(g, 4, 3), 3);
}

// ─── Тест: Дейкстра (взвешенный граф) ───────────────────────────────────────
TEST(ShortestPathsTest, DijkstraWeighted) {
  /*
        1
       / \
      10  3
     /     \
    2 --6-- 3
  */
  const std::string graph_data =
      "3\n"
      "0 10 3\n"
      "10 0 6\n"
      "3 6 0\n";

  const auto path = WriteTempGraph("dijkstra_weighted_graph.txt", graph_data);

  s21::Graph g;
  ASSERT_NO_THROW(g.LoadGraphFromFile(path));

  // Прямой путь 1→2 = 10, через 3 = 3 + 6 = 9.
  EXPECT_EQ(s21::GraphAlgorithms::GetShortestPathBetweenVertices(g, 1, 2), 9);

  // Прямой путь 1→3 = 3.
  EXPECT_EQ(s21::GraphAlgorithms::GetShortestPathBetweenVertices(g, 1, 3), 3);

  EXPECT_EQ(s21::GraphAlgorithms::GetShortestPathBetweenVertices(g, 2, 3), 6);
}

// ─── Тест: Дейкстра (недостижимая вершина) ──────────────────────────────────
TEST(ShortestPathsTest, DijkstraUnreachable) {
  // Два изолированных компонента: {1-2} и {3}.
  const std::string graph_data =
      "3\n"
      "0 1 0\n"
      "1 0 0\n"
      "0 0 0\n";

  const auto path =
      WriteTempGraph("dijkstra_unreachable_graph.txt", graph_data);

  s21::Graph g;
  ASSERT_NO_THROW(g.LoadGraphFromFile(path));

  EXPECT_EQ(s21::GraphAlgorithms::GetShortestPathBetweenVertices(g, 1, 2), 1);

  // В исходных тестах ожидалось 0 для недостижимых вершин.
  EXPECT_EQ(s21::GraphAlgorithms::GetShortestPathBetweenVertices(g, 1, 3), 0);
  EXPECT_EQ(s21::GraphAlgorithms::GetShortestPathBetweenVertices(g, 3, 1), 0);
}

// ─── Тест: Флойд-Уоршелл (невзвешенный граф) ───────────────────────────────
TEST(ShortestPathsTest, FloydWarshallUnweighted) {
  /*
    Граф:

    1 -- 2 -- 4
    |    |
    3 -- 5
  */
  const std::string graph_data =
      "5\n"
      "0 1 1 0 0\n"
      "1 0 0 1 1\n"
      "1 0 0 0 1\n"
      "0 1 0 0 0\n"
      "0 1 1 0 0\n";

  const auto path =
      WriteTempGraph("floyd_warshall_unweighted_graph.txt", graph_data);

  s21::Graph g;
  ASSERT_NO_THROW(g.LoadGraphFromFile(path));

  auto dist = s21::GraphAlgorithms::GetShortestPathsBetweenAllVertices(g);

  ASSERT_EQ(dist.size(), 5u);

  EXPECT_EQ(dist[0][0], 0);  // 1→1
  EXPECT_EQ(dist[0][1], 1);  // 1→2
  EXPECT_EQ(dist[0][2], 1);  // 1→3
  EXPECT_EQ(dist[0][3], 2);  // 1→4
  EXPECT_EQ(dist[0][4], 2);  // 1→5

  EXPECT_EQ(dist[3][0], 2);  // 4→1
  EXPECT_EQ(dist[3][2], 3);  // 4→3
}

// ─── Тест: Флойд-Уоршелл (взвешенный граф) ─────────────────────────────────
TEST(ShortestPathsTest, FloydWarshallWeighted) {
  /*
        1
       / \
      10  3
     /     \
    2 --6-- 3
  */
  const std::string graph_data =
      "3\n"
      "0 10 3\n"
      "10 0 6\n"
      "3 6 0\n";

  const auto path =
      WriteTempGraph("floyd_warshall_weighted_graph.txt", graph_data);

  s21::Graph g;
  ASSERT_NO_THROW(g.LoadGraphFromFile(path));

  auto dist = s21::GraphAlgorithms::GetShortestPathsBetweenAllVertices(g);

  EXPECT_EQ(dist[0][0], 0);
  EXPECT_EQ(dist[0][1], 9);  // 1→2 через 3
  EXPECT_EQ(dist[0][2], 3);  // 1→3

  EXPECT_EQ(dist[1][0], 9);  // 2→1 через 3
  EXPECT_EQ(dist[1][2], 6);  // 2→3

  EXPECT_EQ(dist[2][0], 3);  // 3→1
  EXPECT_EQ(dist[2][1], 6);  // 3→2
}

// ─── Тест: Флойд-Уоршелл (недостижимые вершины) ────────────────────────────
TEST(ShortestPathsTest, FloydWarshallUnreachable) {
  // Два изолированных компонента: {1-2} и {3}.
  const std::string graph_data =
      "3\n"
      "0 1 0\n"
      "1 0 0\n"
      "0 0 0\n";

  const auto path =
      WriteTempGraph("floyd_warshall_unreachable_graph.txt", graph_data);

  s21::Graph g;
  ASSERT_NO_THROW(g.LoadGraphFromFile(path));

  auto dist = s21::GraphAlgorithms::GetShortestPathsBetweenAllVertices(g);

  EXPECT_EQ(dist[0][0], 0);
  EXPECT_EQ(dist[0][1], 1);
  EXPECT_EQ(dist[0][2], INT_MAX);  // 1→3 недостижима

  EXPECT_EQ(dist[1][0], 1);
  EXPECT_EQ(dist[2][0], INT_MAX);  // 3→1 недостижима
  EXPECT_EQ(dist[2][2], 0);
}

// ─── Тест: MST Прима (невзвешенный — все рёбра = 1) ────────────────────────
TEST(PrimTest, UnweightedGraphReturnsTreeWithFourEdges) {
  /*
    Граф:

    1 -- 2
    |    |
    3 -- 4
    |
    5
  */
  const std::string graph_data =
      "5\n"
      "0 1 1 0 0\n"
      "1 0 0 1 0\n"
      "1 0 0 1 1\n"
      "0 1 1 0 0\n"
      "0 0 1 0 0\n";

  const auto path = WriteTempGraph("prim_unweighted_graph.txt", graph_data);

  s21::Graph g;
  ASSERT_NO_THROW(g.LoadGraphFromFile(path));

  auto mst = s21::GraphAlgorithms::GetLeastSpanningTree(g);

  ASSERT_EQ(mst.size(), 5u);

  for (const auto& row : mst) {
    ASSERT_EQ(row.size(), mst.size());
  }

  int edge_count = 0;
  int total_weight = 0;

  for (std::size_t i = 0; i < mst.size(); ++i) {
    for (std::size_t j = i + 1; j < mst.size(); ++j) {
      if (mst[i][j] != 0) {
        ++edge_count;
        total_weight += mst[i][j];
      }
    }
  }

  // MST должно содержать ровно 4 ребра (n - 1).
  EXPECT_EQ(edge_count, 4);

  // Все веса = 1.
  EXPECT_EQ(total_weight, 4);

  // Матрица симметрична.
  for (std::size_t i = 0; i < mst.size(); ++i) {
    for (std::size_t j = 0; j < mst.size(); ++j) {
      EXPECT_EQ(mst[i][j], mst[j][i]);
    }
  }
}

// ─── Тест: MST Прима (взвешенный граф) ─────────────────────────────────────
TEST(PrimTest, WeightedGraphMinimumSpanningTreeWeightIsNine) {
  /*
        1
       /|\
      1 7 5
     /  |  \
    2   3---4
        3

    Остов: 1-2(1), 1-3(5), 3-4(3) = вес 9.
  */
  const std::string graph_data =
      "4\n"
      "0 1 5 7\n"
      "1 0 7 0\n"
      "5 7 0 3\n"
      "7 0 3 0\n";

  const auto path = WriteTempGraph("prim_weighted_graph.txt", graph_data);

  s21::Graph g;
  ASSERT_NO_THROW(g.LoadGraphFromFile(path));

  auto mst = s21::GraphAlgorithms::GetLeastSpanningTree(g);

  ASSERT_EQ(mst.size(), 4u);

  for (const auto& row : mst) {
    ASSERT_EQ(row.size(), mst.size());
  }

  int total_weight = 0;

  for (std::size_t i = 0; i < mst.size(); ++i) {
    for (std::size_t j = i + 1; j < mst.size(); ++j) {
      total_weight += mst[i][j];
    }
  }

  EXPECT_EQ(total_weight, 9);
}

// ─── Тест: MST Прима (одна вершина) ────────────────────────────────────────
TEST(PrimTest, SingleVertexGraphReturnsEmptyTree) {
  const std::string graph_data =
      "1\n"
      "0\n";

  const auto path = WriteTempGraph("prim_single_vertex_graph.txt", graph_data);

  s21::Graph g;
  ASSERT_NO_THROW(g.LoadGraphFromFile(path));

  auto mst = s21::GraphAlgorithms::GetLeastSpanningTree(g);

  ASSERT_EQ(mst.size(), 1u);
  ASSERT_EQ(mst[0].size(), 1u);

  EXPECT_EQ(mst[0][0], 0);
}

// ─── Тест: TSP — треугольник (оптимальный маршрут известен) ───────────────
TEST(TspTest, TriangleGraphOptimalDistanceIsSix) {
  /*
        1
       / \
      1   3
     /     \
    2---2---3

    Оптимум: 1→2→3→1 = 1 + 2 + 3 = 6
  */
  const std::string graph_data =
      "3\n"
      "0 1 3\n"
      "1 0 2\n"
      "3 2 0\n";

  const auto path = WriteTempGraph("tsp_triangle_graph.txt", graph_data);

  s21::Graph g;
  ASSERT_NO_THROW(g.LoadGraphFromFile(path));

  auto result = s21::GraphAlgorithms::SolveTravelingSalesmanProblem(g);

  EXPECT_NEAR(result.distance, 6.0, 1e-6);

  ASSERT_EQ(result.vertices.size(), 4u);  // n + 1 (с возвратом)

  EXPECT_EQ(result.vertices.front(), 1);
  EXPECT_EQ(result.vertices.back(), 1);
}

// ─── Тест: TSP — 4 вершины, полный граф ─────────────────────────────────────
TEST(TspTest, CompleteGraph4OptimalDistanceIsFour) {
  /*
    1---2
    | X |
    3---4

    Полный граф, все веса = 1.
    Любой гамильтонов цикл = 4.
  */
  const std::string graph_data =
      "4\n"
      "0 1 1 1\n"
      "1 0 1 1\n"
      "1 1 0 1\n"
      "1 1 1 0\n";

  const auto path = WriteTempGraph("tsp_complete4_graph.txt", graph_data);

  s21::Graph g;
  ASSERT_NO_THROW(g.LoadGraphFromFile(path));

  auto result = s21::GraphAlgorithms::SolveTravelingSalesmanProblem(g);

  EXPECT_NEAR(result.distance, 4.0, 1e-6);

  ASSERT_EQ(result.vertices.size(), 5u);
  EXPECT_EQ(result.vertices.front(), 1);
  EXPECT_EQ(result.vertices.back(), 1);

  // Все вершины посещены ровно по одному разу (кроме старта/финала).
  std::vector<int> inner(result.vertices.begin() + 1,
                         result.vertices.end() - 1);

  std::sort(inner.begin(), inner.end());

  EXPECT_EQ(inner, (std::vector<int>{2, 3, 4}));
}

// ─── Тест: TSP — взвешенный полный граф 4 вершины ──────────────────────────
TEST(TspTest, WeightedCompleteGraph4OptimalDistanceIsFive) {
  /*
        1
       /|\
      1 5 2
     /  |  \
    2   3---4
        1

    Оптимум:
      1→4→3→2→1 = 2 + 1 + 1 + 1 = 5
    или
      1→2→3→4→1 = 1 + 1 + 1 + 2 = 5
  */
  const std::string graph_data =
      "4\n"
      "0 1 5 2\n"
      "1 0 1 1\n"
      "5 1 0 1\n"
      "2 1 1 0\n";

  const auto path = WriteTempGraph("tsp_weighted4_graph.txt", graph_data);

  s21::Graph g;
  ASSERT_NO_THROW(g.LoadGraphFromFile(path));

  auto result = s21::GraphAlgorithms::SolveTravelingSalesmanProblem(g);

  EXPECT_NEAR(result.distance, 5.0, 1e-6);

  ASSERT_EQ(result.vertices.size(), 5u);
  EXPECT_EQ(result.vertices.front(), 1);
  EXPECT_EQ(result.vertices.back(), 1);
}

// ─── Тест: TSP — несвязный граф (ошибка) ────────────────────────────────────
TEST(TspTest, DisconnectedGraphThrowsRuntimeError) {
  const std::string graph_data =
      "4\n"
      "0 1 0 0\n"
      "1 0 0 0\n"
      "0 0 0 1\n"
      "0 0 1 0\n";

  const auto path = WriteTempGraph("tsp_disconnected_graph.txt", graph_data);

  s21::Graph g;
  ASSERT_NO_THROW(g.LoadGraphFromFile(path));

  auto result = s21::GraphAlgorithms::SolveTravelingSalesmanProblem(g);
  EXPECT_EQ(result.distance, -1);
}

// ─── Тест: TSP — нет гамильтонова цикла (орграф с пропущенным ребром) ─────
TEST(TspTest, GraphWithoutHamiltonianCycleThrowsRuntimeError) {
  // 1→2→3, но 3→1 отсутствует.
  const std::string graph_data =
      "3\n"
      "0 1 0\n"
      "0 0 1\n"
      "0 0 0\n";

  const auto path = WriteTempGraph("tsp_no_cycle_graph.txt", graph_data);

  s21::Graph g;
  ASSERT_NO_THROW(g.LoadGraphFromFile(path));

  auto result = s21::GraphAlgorithms::SolveTravelingSalesmanProblem(g);
  EXPECT_EQ(result.distance, -1);
}

TEST(TspTest, GraphWithoutHamiltonianCycleThrowsRuntimeErrorTwo) {
  const std::string graph_data =
      "Z\n"
      "0 1 0\n"
      "0 0 1\n"
      "0 0 0\n";

  const auto path = WriteTempGraph("tsp_no_cycle_graph.txt", graph_data);

  s21::Graph g;
  ASSERT_EQ(g.LoadGraphFromFile(path), -1);
}

TEST(TspTest, GraphWithoutHamiltonianCycleThrowsRuntimeErrorThree) {
  const std::string graph_data =
      "-1\n"
      "0 1 0\n"
      "0 0 1\n"
      "0 0 0\n";

  const auto path = WriteTempGraph("tsp_no_cycle_graph.txt", graph_data);

  s21::Graph g;
  ASSERT_EQ(g.LoadGraphFromFile(path), -1);
}

TEST(TspTest, GraphWithoutHamiltonianCycleThrowsRuntimeErrorFour) {
  const std::string graph_data =
      "3\n"
      "0 -5 0\n"
      "0 0 1\n"
      "0 0 0\n";

  const auto path = WriteTempGraph("tsp_no_cycle_graph.txt", graph_data);

  s21::Graph g;
  ASSERT_EQ(g.LoadGraphFromFile(path), -1);
}

TEST(TspTest, SolveGreedyGraph4OptimalDistanceIsFour) {
  /*
    1---2
    | X |
    3---4

    Полный граф, все веса = 1.
    Любой гамильтонов цикл = 4.
  */
  const std::string graph_data =
      "4\n"
      "0 1 1 1\n"
      "1 0 1 1\n"
      "1 1 0 1\n"
      "1 1 1 0\n";

  const auto path = WriteTempGraph("tsp_complete4_graph.txt", graph_data);

  s21::Graph g;
  ASSERT_NO_THROW(g.LoadGraphFromFile(path));

  auto result = s21::GraphAlgorithms::SolveGreedy(g);

  EXPECT_NEAR(result.distance, 4.0, 1e-6);

  ASSERT_EQ(result.vertices.size(), 5u);
  EXPECT_EQ(result.vertices.front(), 1);
  EXPECT_EQ(result.vertices.back(), 1);

  // Все вершины посещены ровно по одному разу (кроме старта/финала).
  std::vector<int> inner(result.vertices.begin() + 1,
                         result.vertices.end() - 1);

  std::sort(inner.begin(), inner.end());

  EXPECT_EQ(inner, (std::vector<int>{2, 3, 4}));
}

TEST(TspTest, SolveBruteForceGraph4OptimalDistanceIsFour) {
  /*
    1---2
    | X |
    3---4

    Полный граф, все веса = 1.
    Любой гамильтонов цикл = 4.
  */
  const std::string graph_data =
      "4\n"
      "0 1 1 1\n"
      "1 0 1 1\n"
      "1 1 0 1\n"
      "1 1 1 0\n";

  const auto path = WriteTempGraph("tsp_complete4_graph.txt", graph_data);

  s21::Graph g;
  ASSERT_NO_THROW(g.LoadGraphFromFile(path));

  auto result = s21::GraphAlgorithms::SolveBruteForce(g);

  EXPECT_NEAR(result.distance, 4.0, 1e-6);

  ASSERT_EQ(result.vertices.size(), 5u);
  EXPECT_EQ(result.vertices.front(), 1);
  EXPECT_EQ(result.vertices.back(), 1);

  // Все вершины посещены ровно по одному разу (кроме старта/финала).
  std::vector<int> inner(result.vertices.begin() + 1,
                         result.vertices.end() - 1);

  std::sort(inner.begin(), inner.end());

  EXPECT_EQ(inner, (std::vector<int>{2, 3, 4}));
}

TEST(TspTest, BenchmarkTsp) {
  /*
    1---2
    | X |
    3---4

    Полный граф, все веса = 1.
    Любой гамильтонов цикл = 4.
  */
  const std::string graph_data =
      "4\n"
      "0 1 1 1\n"
      "1 0 1 1\n"
      "1 1 0 1\n"
      "1 1 1 0\n";

  const auto path = WriteTempGraph("tsp_complete4_graph.txt", graph_data);

  s21::Graph g;
  ASSERT_NO_THROW(g.LoadGraphFromFile(path));

  auto results = s21::GraphAlgorithms::BenchmarkTsp(g, 10);

  ASSERT_EQ(results.size(), 3u);

  for (auto res : results) {
    EXPECT_NEAR(res.best_distance, 4.0, 1e-6);
  }
}